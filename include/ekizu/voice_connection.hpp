#ifndef EKIZU_VOICE_CONNECTION_HPP
#define EKIZU_VOICE_CONNECTION_HPP

#include <boost/asio/experimental/channel.hpp>
#include <boost/asio/steady_timer.hpp>
#include <ekizu/log.hpp>
#include <ekizu/udp.hpp>
#include <ekizu/voice_state.hpp>
#include <ekizu/ws.hpp>

namespace ekizu {
/// An RTP packet the server send to the client containing the voice data.
struct Packet {
	std::array<uint8_t, 2> type;
	uint16_t sequence;
	uint32_t timestamp;
	uint32_t ssrc;
	std::vector<std::byte> opus;
};

enum class SpeakerFlag : uint8_t {
	None = 0,
	Microphone = 1 << 0,
	Soundshared = 1 << 1,
	Priority = 1 << 2
};

enum class VoiceOpcode : uint8_t {
	Identify = 0,			 // client
	SelectProtocol = 1,		 // client
	Ready = 2,				 // server
	Heartbeat = 3,			 // client
	SessionDescription = 4,	 // server
	Speaking = 5,			 // client and server
	HeartbeatAck = 6,		 // server
	Resume = 7,				 // client
	Hello = 8,				 // server
	Resumed = 9,			 // server
	ClientDisconnect = 13,	 // server
};

struct VoiceConnection {
	VoiceConnection(const VoiceConnection &) = delete;
	VoiceConnection &operator=(const VoiceConnection &) = delete;
	EKIZU_EXPORT VoiceConnection(VoiceConnection &&) noexcept;
	VoiceConnection &operator=(VoiceConnection &&) noexcept = delete;
	EKIZU_EXPORT ~VoiceConnection();

	std::optional<
		asio::experimental::channel<void(boost::system::error_code, Packet)>> &
	recv_chan() {
		return m_recv_chan;
	}

	EKIZU_EXPORT void attach_logger(std::function<void(Log)> on_log);
	EKIZU_EXPORT Result<> close(const asio::yield_context &yield);
	EKIZU_EXPORT Result<> reconnect(const asio::yield_context &yield);
	EKIZU_EXPORT Result<> run(const asio::yield_context &yield);
	EKIZU_EXPORT Result<> send_opus(boost::span<const std::byte> data,
									const asio::yield_context &yield);
	EKIZU_EXPORT Result<> send_raw(boost::span<const std::byte> data,
								   const asio::yield_context &yield);
	EKIZU_EXPORT Result<> silence(const asio::yield_context &yield);
	EKIZU_EXPORT Result<> speak(SpeakerFlag flags,
								const asio::yield_context &yield);

   private:
	friend struct VoiceConnectionConfig;
	struct Codec;

	VoiceConnection(net::WebSocketClient ws, VoiceState state, std::string url,
					std::string_view token, std::unique_ptr<Codec> codec);

	Result<> handle_session_description(const nlohmann::json &data,
										const asio::yield_context &yield);
	Result<> setup_heartbeat(const nlohmann::json &data,
							 const asio::yield_context &yield);
	Result<> send_heartbeat(const asio::yield_context &yield);
	Result<> opus_receiver(const asio::yield_context &yield);
	Result<> opus_sender(const asio::yield_context &yield);
	Result<> setup_udp(const nlohmann::json &data,
					   const asio::yield_context &yield);
	Result<> ws_listen(const asio::yield_context &yield);
	void log(std::string_view msg, LogLevel level = LogLevel::Debug) const;

	struct AudioPacket {
		std::vector<std::byte> encoded{};
		size_t frame_count{};
	};

	std::optional<net::WebSocketClient> m_ws;
	VoiceState m_state;
	std::string m_url;
	std::string m_token;
	std::optional<asio::experimental::channel<void(
		boost::system::error_code, AudioPacket)>>
		m_channel;
	std::optional<asio::experimental::channel<void(
		boost::system::error_code, boost::blank)>>
		m_ready_chan;
	std::optional<
		asio::experimental::channel<void(boost::system::error_code, Packet)>>
		m_recv_chan;
	std::optional<asio::steady_timer> m_heartbeat_timer;
	bool m_last_heartbeat_acked{true};
	uint32_t m_ssrc{};
	std::optional<net::UdpClient> m_udp;
	std::array<uint8_t, 32> m_secret_key{};
	std::unique_ptr<Codec> m_codec;
	bool m_disconnected{};
	bool m_speaking{};
	std::function<void(Log)> m_on_log;
};

struct VoiceConnectionConfig {
	[[nodiscard]] EKIZU_EXPORT Result<VoiceConnection> connect(
		const asio::yield_context &yield) const;

	std::optional<VoiceState> state{};
	std::optional<std::string> endpoint{};
	std::optional<std::string> token{};
};
}  // namespace ekizu

#endif	// EKIZU_VOICE_CONNECTION_HPP
