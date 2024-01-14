#include <opus.h>
#include <sodium.h>

#include <boost/asio/detached.hpp>
#include <boost/endian/arithmetic.hpp>
#include <ekizu/json_util.hpp>
#include <ekizu/voice_connection.hpp>

namespace {
using boost::asio::spawn;
using namespace ekizu;

constexpr uint16_t MAX_PACKET_SIZE{3 * 1276};
constexpr uint8_t RTP_HEADER_SIZE{12};
constexpr uint8_t CHANNEL_COUNT{2};
constexpr uint16_t FRAME_COUNT{960};
constexpr uint32_t SAMPLE_RATE{48'000};
constexpr std::array<std::byte, 3> SILENCE_FRAME{
	std::byte{0xF8}, std::byte{0xFF}, std::byte{0xFE}};

template <typename T>
struct DeleterOf;

template <>
struct DeleterOf<OpusDecoder> {
	void operator()(OpusDecoder *decoder) const {
		opus_decoder_destroy(decoder);
	}
};

template <>
struct DeleterOf<OpusEncoder> {
	void operator()(OpusEncoder *encoder) const {
		opus_encoder_destroy(encoder);
	}
};

using UniqueOpusDecoder = std::unique_ptr<OpusDecoder, DeleterOf<OpusDecoder>>;
using UniqueOpusEncoder = std::unique_ptr<OpusEncoder, DeleterOf<OpusEncoder>>;

Result<UniqueOpusDecoder> create_decoder() {
	int err{};
	auto decoder = UniqueOpusDecoder{opus_decoder_create(SAMPLE_RATE, 2, &err)};

	if (err != OPUS_OK) {
		return boost::system::error_code{err, boost::system::system_category()};
	}

	return decoder;
}

Result<UniqueOpusEncoder> create_encoder() {
	int err{};
	auto encoder = UniqueOpusEncoder{
		opus_encoder_create(SAMPLE_RATE, 2, OPUS_APPLICATION_VOIP, &err)};

	if (err != OPUS_OK) {
		return boost::system::error_code{err, boost::system::system_category()};
	}

	return encoder;
}
}  // namespace

namespace ekizu {
struct VoiceConnection::Codec {
	Codec(UniqueOpusDecoder decoder, UniqueOpusEncoder encoder)
		: m_decoder{std::move(decoder)}, m_encoder{std::move(encoder)} {}

	[[nodiscard]] Result<opus_int32> encode(boost::span<const std::byte> pcm,
											boost::span<std::byte> opus) const {
		auto err = opus_encode(
			m_encoder.get(), reinterpret_cast<const int16_t *>(pcm.data()),
			FRAME_COUNT, reinterpret_cast<uint8_t *>(opus.data()),
			static_cast<int>(opus.size()));

		if (err < 0) {
			return boost::system::error_code{
				err, boost::system::system_category()};
		}

		return err;
	}

   private:
	UniqueOpusDecoder m_decoder;
	UniqueOpusEncoder m_encoder;
};

VoiceConnection::VoiceConnection(VoiceConnection &&) noexcept = default;
VoiceConnection::~VoiceConnection() = default;

EKIZU_EXPORT void VoiceConnection::attach_logger(
	std::function<void(Log)> on_log) {
	m_on_log = std::move(on_log);
}

Result<> VoiceConnection::close(const boost::asio::yield_context &yield) {
	m_channel.reset();
	m_heartbeat_timer.reset();
	m_ready_chan.reset();
	m_recv_chan.reset();

	if (m_udp) {
		EKIZU_TRY(m_udp->close());
		m_udp.reset();
	}

	if (m_ws) {
		EKIZU_TRY(m_ws->close(net::ws::close_code::normal, yield));
		// read to trigger actual disconnection.
		(void)m_ws->read(yield);
		m_ws.reset();
	}

	m_disconnected = true;
	return outcome::success();
}

Result<> VoiceConnection::reconnect(const boost::asio::yield_context &yield) {
	if (!m_ws) { return boost::system::errc::operation_not_permitted; }

	nlohmann::json payload{
		{"op", static_cast<uint8_t>(VoiceOpcode::Resume)},
		{"d",
		 {
			 {"server_id", *m_state.guild_id},
			 {"session_id", m_state.session_id},
			 {"token", m_token},
		 }},
	};

	return m_ws->send(payload.dump(), yield);
}

Result<> VoiceConnection::run(const boost::asio::yield_context &yield) {
	if (m_ready_chan) {
		// Already running.
		return outcome::success();
	}

	m_ready_chan.emplace(yield.get_executor());

	//  Identify ourselves.
	nlohmann::json payload{
		{"op", static_cast<uint8_t>(VoiceOpcode::Identify)},
		{"d",
		 {
			 {"server_id", *m_state.guild_id},
			 {"user_id", m_state.user_id},
			 {"session_id", m_state.session_id},
			 {"token", m_token},
		 }},
	};

	if (m_disconnected) {
		payload = {{"op", static_cast<uint8_t>(VoiceOpcode::Resume)},
				   {"d",
					{
						{"server_id", *m_state.guild_id},
						{"session_id", m_state.session_id},
						{"token", m_token},
					}}};
		m_disconnected = false;
	}

	if (!m_ws) {
		EKIZU_TRY(auto ws, net::WebSocketClient::connect(m_url, yield));
		m_ws.emplace(std::move(ws));
	}

	EKIZU_TRY(m_ws->send(payload.dump(), yield));

	spawn(
		yield,
		[this](auto y) {
			auto res = ws_listen(y);

			if (!res) {
				log(fmt::format("Failed to listen to gateway: {}",
								res.error().message()),
					LogLevel::Error);
			}
		},
		boost::asio::detached);

	m_ready_chan->async_receive(yield);
	return outcome::success();
}

Result<> VoiceConnection::send_opus(boost::span<const std::byte> data,
									const boost::asio::yield_context &yield) {
	if (data.empty()) { return boost::system::errc::invalid_argument; }

	const auto samples = opus_packet_get_samples_per_frame(
		reinterpret_cast<const uint8_t *>(data.data()), SAMPLE_RATE);

	if (samples < 0) {
		return boost::system::error_code{
			samples, boost::system::system_category()};
	}

	m_channel->async_send(
		boost::system::error_code{},
		AudioPacket{
			std::vector<std::byte>(data.begin(), data.end()),
			static_cast<size_t>(samples),
		},
		yield);

	return outcome::success();
}

Result<> VoiceConnection::send_raw(boost::span<const std::byte> data,
								   const boost::asio::yield_context &yield) {
	if (data.empty()) { return boost::system::errc::invalid_argument; }
	std::vector<std::byte> encoded_audio(data.size());
	EKIZU_TRY(auto encoded, m_codec->encode(data, encoded_audio));
	encoded_audio.resize(encoded);
	return send_opus(encoded_audio, yield);
}

Result<> VoiceConnection::silence(const boost::asio::yield_context &yield) {
	return send_opus(SILENCE_FRAME, yield);
}

Result<> VoiceConnection::speak(SpeakerFlag flags,
								const boost::asio::yield_context &yield) {
	if (!m_ws) { return boost::system::errc::operation_not_permitted; }

	nlohmann::json payload{
		{"op", static_cast<uint8_t>(VoiceOpcode::Speaking)},
		{"d",
		 {
			 {"speaking", static_cast<uint8_t>(flags)},
			 {"delay", 0},
			 {"ssrc", m_ssrc},
		 }},
	};

	m_speaking = static_cast<uint8_t>(flags) != 0;

	return m_ws->send(payload.dump(), yield);
}

VoiceConnection::VoiceConnection(net::WebSocketClient ws, VoiceState state,
								 std::string url, std::string_view token,
								 std::unique_ptr<Codec> codec)
	: m_ws{std::move(ws)},
	  m_state{std::move(state)},
	  m_url{std::move(url)},
	  m_token{token},
	  m_codec{std::move(codec)} {}

Result<> VoiceConnection::handle_session_description(
	const nlohmann::json &data, const boost::asio::yield_context &yield) {
	m_secret_key = data["d"]["secret_key"];
	m_channel.emplace(yield.get_executor());

	spawn(
		yield, [this](auto y) { (void)opus_sender(y); }, boost::asio::detached);
	spawn(
		yield, [this](auto y) { (void)opus_receiver(y); },
		boost::asio::detached);

	return outcome::success();
}

Result<> VoiceConnection::setup_heartbeat(
	const nlohmann::json &data, const boost::asio::yield_context &yield) {
	if (m_heartbeat_timer) { return outcome::success(); }

	uint32_t heartbeat_interval = data["d"]["heartbeat_interval"];

	m_heartbeat_timer.emplace(
		yield.get_executor(),
		boost::posix_time::milliseconds(heartbeat_interval));

	spawn(
		yield,
		[this, heartbeat_interval](auto y) {
			boost::system::error_code ec;

			while (m_heartbeat_timer) {
				m_heartbeat_timer->async_wait(y[ec]);
				if (ec) { return; }

				m_last_heartbeat_acked = false;
				m_heartbeat_timer->expires_from_now(
					boost::posix_time::milliseconds(heartbeat_interval));

				if (auto r = send_heartbeat(y); !r) {
					log(fmt::format("Failed to send heartbeat: {}",
									r.error().message()),
						LogLevel::Error);
					(void)close(y);
				}
			}
		},
		boost::asio::detached);

	return outcome::success();
}

Result<> VoiceConnection::send_heartbeat(
	const boost::asio::yield_context &yield) {
	if (!m_ws) { return outcome::success(); }

	nlohmann::json payload{
		{"op", static_cast<uint8_t>(VoiceOpcode::Heartbeat)},
		{"d", rand()},
	};

	return m_ws->send(payload.dump(), yield);
}

Result<> VoiceConnection::opus_receiver(
	const boost::asio::yield_context &yield) {
	m_recv_chan.emplace(yield.get_executor());

	std::array<std::byte, MAX_PACKET_SIZE> decrypyted{};
	std::array<std::byte, 24> nonce{};

	// In order to start receiving packets we need to send a silence frame.
	EKIZU_TRY(silence(yield));

	while (m_udp) {
		EKIZU_TRY(std::string res, m_udp->receive(yield));
		auto msg = boost::span<const uint8_t>{
			reinterpret_cast<const uint8_t *>(res.data()), res.size()};

		if (msg.size() < RTP_HEADER_SIZE ||
			(msg[0] != 0x80 && msg[0] != 0x90)) {
			continue;
		}

		Packet p{};
		p.type = {msg[0], msg[1]};
		p.sequence = boost::endian::big_to_native(
			*reinterpret_cast<const uint16_t *>(&msg[2]));
		p.timestamp = boost::endian::big_to_native(
			*reinterpret_cast<const uint32_t *>(&msg[4]));
		p.ssrc = boost::endian::big_to_native(
			*reinterpret_cast<const uint32_t *>(&msg[8]));

		std::memcpy(nonce.data(), msg.data(), RTP_HEADER_SIZE);

		if (auto err = crypto_secretbox_open_easy(
				reinterpret_cast<uint8_t *>(decrypyted.data()),
				msg.data() + RTP_HEADER_SIZE, msg.size() - RTP_HEADER_SIZE,
				reinterpret_cast<const uint8_t *>(nonce.data()),
				reinterpret_cast<const uint8_t *>(m_secret_key.data()));
			err != 0) {
			log(fmt::format("Failed to decrypt packet: {}", err),
				LogLevel::Error);
			continue;
		}

		auto decrypted_span =
			boost::span{decrypyted}.subspan(0, msg.size() - RTP_HEADER_SIZE);

		p.opus = std::vector(decrypted_span.begin(), decrypted_span.end());

		// extension bit set, and not a RTCP packet
		if (((msg[0] & 0x10) == 0x10) && ((msg[1] & 0x80) == 0)) {
			// get extended header length
			auto extlen = boost::endian::big_to_native(
				*reinterpret_cast<const uint16_t *>(&p.opus[2]));
			// 4 bytes (ext header header) + 4*extlen (ext header data)
			auto shift = 4 + extlen * 4;

			if (p.opus.size() > shift) {
				p.opus.erase(p.opus.begin(), p.opus.begin() + shift);
				p.opus.shrink_to_fit();
			}
		}

		m_recv_chan->async_send(
			boost::system::error_code{}, std::move(p), yield);
	}

	return outcome::success();
}

Result<> VoiceConnection::opus_sender(const boost::asio::yield_context &yield) {
	m_ready_chan->async_send(boost::system::error_code{}, {}, yield);

	std::array<std::byte,
			   RTP_HEADER_SIZE + crypto_secretbox_MACBYTES + MAX_PACKET_SIZE>
		payload{std::byte{0x80}, std::byte{0x78}};
	uint16_t sequence{};
	uint32_t timestamp{};
	std::array<std::byte, 24> nonce{};
	boost::endian::big_uint32_t ssrc{m_ssrc};
	std::memcpy(payload.data() + 8, ssrc.data(), 4);

	while (m_channel && m_udp) {
		boost::system::error_code ec;
		auto [packet, frame_count] = m_channel->async_receive(yield[ec]);
		if (ec) { return ec; }

		boost::endian::big_uint16_t seq{sequence};
		std::memcpy(payload.data() + 2, seq.data(), 2);
		boost::endian::big_uint32_t ts{timestamp};
		std::memcpy(payload.data() + 4, ts.data(), 4);
		std::memcpy(nonce.data(), payload.data(), RTP_HEADER_SIZE);

		if (auto err = crypto_secretbox_easy(
				reinterpret_cast<uint8_t *>(payload.data() + RTP_HEADER_SIZE),
				reinterpret_cast<const uint8_t *>(packet.data()), packet.size(),
				reinterpret_cast<const uint8_t *>(nonce.data()),
				m_secret_key.data());
			err != 0) {
			log(fmt::format("Failed to encrypt packet: {}", err),
				LogLevel::Error);
			return boost::system::errc::io_error;
		}

		if (ec) { return ec; }
		if (!m_udp) { return boost::system::errc::io_error; }

		auto payload_span = boost::span{payload}.subspan(
			0, RTP_HEADER_SIZE + crypto_secretbox_MACBYTES + packet.size());
		size_t sent{};

		do {
			EKIZU_TRY(
				auto more, m_udp->send(payload_span.subspan(sent), yield));
			sent += more;
		} while (sent < payload_span.size());

		sequence = (sequence + 1) % (std::numeric_limits<uint16_t>::max() + 1U);
		timestamp = (timestamp + frame_count) %
					(std::numeric_limits<uint32_t>::max() + 1ULL);
	}

	return outcome::success();
}

Result<> VoiceConnection::setup_udp(const nlohmann::json &data,
									const boost::asio::yield_context &yield) {
	m_ssrc = data["d"]["ssrc"];
	std::string ip = data["d"]["ip"];
	uint16_t port = data["d"]["port"];

	EKIZU_TRY(m_udp, net::UdpClient::create(ip, std::to_string(port), yield));

	// https://discord.com/developers/docs/topics/voice-connections#ip-discovery
	std::string packet(74, '\0');
	boost::endian::big_uint16_buf_t type(0x1);
	boost::endian::big_uint16_buf_t length(70);
	boost::endian::big_uint32_buf_t ssrc(m_ssrc);
	std::memcpy(packet.data(), &type, sizeof(type));
	std::memcpy(&packet[2], &length, sizeof(length));
	std::memcpy(&packet[4], &ssrc, sizeof(ssrc));

	EKIZU_TRY(
		auto sent, m_udp->send(boost::as_bytes(boost::span{packet}), yield));
	EKIZU_TRY(auto res, m_udp->receive(yield));
	if (res.length() < 8) { return boost::system::errc::message_size; }

	auto external_ip = std::string_view{res}.substr(8, 64);
	auto external_port = boost::endian::big_to_native(
		*reinterpret_cast<const uint16_t *>(&res[res.length() - 2]));

	// https://discord.com/developers/docs/topics/voice-connections#establishing-a-voice-udp-connection-example-select-protocol-payload
	nlohmann::json payload{
		{"op", static_cast<uint8_t>(VoiceOpcode::SelectProtocol)},
		{"d",
		 {
			 {"protocol", "udp"},
			 {"data",
			  {
				  {"address", external_ip},
				  {"port", external_port},
				  {"mode", "xsalsa20_poly1305"},
			  }},
		 }},
	};

	return m_ws->send(payload.dump(), yield);
}

Result<> VoiceConnection::ws_listen(const boost::asio::yield_context &yield) {
	while (m_ws) {
		EKIZU_TRY(auto msg, m_ws->read(yield));
		EKIZU_TRY(auto data, json_util::try_parse(msg.payload));

		switch (data["op"].get<VoiceOpcode>()) {
			// TODO: Handle other opcodes
			case VoiceOpcode::Ready: {
				EKIZU_TRY(setup_udp(data, yield));
				break;
			}
			case VoiceOpcode::SessionDescription: {
				EKIZU_TRY(handle_session_description(data, yield));
				break;
			}
			case VoiceOpcode::HeartbeatAck: {
				m_last_heartbeat_acked = true;
				break;
			}
			case VoiceOpcode::Hello: {
				EKIZU_TRY(setup_heartbeat(data, yield));
				break;
			}
		}
	}

	return outcome::success();
}

void VoiceConnection::log(std::string_view msg, LogLevel level) const {
	if (m_on_log) {
		m_on_log(Log{level, fmt::format("voice_connection{{ssrc={}}}: {}",
										m_ssrc, msg)});
	}
}

Result<VoiceConnection> VoiceConnectionConfig::connect(
	const boost::asio::yield_context &yield) const {
	if (!state || !state->guild_id || !endpoint || !token) {
		return boost::system::errc::invalid_argument;
	}

	if (sodium_init() < 0) { return boost::system::errc::io_error; }
	EKIZU_TRY(auto decoder, create_decoder());
	EKIZU_TRY(auto encoder, create_encoder());

	auto url = fmt::format("wss://{}/?v=4", *endpoint);
	EKIZU_TRY(auto ws, net::WebSocketClient::connect(url, yield));

	return VoiceConnection{std::move(ws), *state, std::move(url), *token,
						   std::make_unique<VoiceConnection::Codec>(
							   std::move(decoder), std::move(encoder))};
}
}  // namespace ekizu
