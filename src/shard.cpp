#include <ekizu/shard.hpp>
#include <nlohmann/json.hpp>

namespace
{
// TODO: Add gateway url customization
constexpr const char *GATEWAY_URL =
	"wss://gateway.discord.gg/?v=10&encoding=json&compress=zlib-stream";

std::optional<ekizu::Event> event_from_str(const nlohmann::json &data)
{
	if (!data.contains("t") || !data.contains("d")) {
		return std::nullopt;
	}

	const std::string event_type = data["t"];
#define EKIZU_EVENT(s, v)                         \
	if (event_type == #s) {                   \
		return data["d"].get<ekizu::v>(); \
	}

	EKIZU_EVENT(CHANNEL_CREATE, ChannelCreate)
	EKIZU_EVENT(CHANNEL_DELETE, ChannelDelete)
	EKIZU_EVENT(CHANNEL_PINS_UPDATE, ChannelPinsUpdate)
	EKIZU_EVENT(CHANNEL_UPDATE, ChannelUpdate)
	EKIZU_EVENT(GUILD_CREATE, GuildCreate)
	EKIZU_EVENT(GUILD_DELETE, GuildDelete)
	EKIZU_EVENT(GUILD_EMOJIS_UPDATE, GuildEmojisUpdate)
	EKIZU_EVENT(GUILD_INTEGRATIONS_UPDATE, GuildIntegrationsUpdate)
	EKIZU_EVENT(GUILD_SCHEDULED_EVENT_CREATE, GuildScheduledEventCreate)
	EKIZU_EVENT(GUILD_SCHEDULED_EVENT_DELETE, GuildScheduledEventDelete)
	EKIZU_EVENT(GUILD_SCHEDULED_EVENT_UPDATE, GuildScheduledEventUpdate)
	EKIZU_EVENT(GUILD_SCHEDULED_EVENT_USER_ADD, GuildScheduledEventUserAdd)
	EKIZU_EVENT(GUILD_SCHEDULED_EVENT_USER_REMOVE,
		    GuildScheduledEventUserRemove)
	EKIZU_EVENT(GUILD_STICKERS_UPDATE, GuildStickersUpdate)
	EKIZU_EVENT(GUILD_UPDATE, GuildUpdate)
	EKIZU_EVENT(MESSAGE_CREATE, MessageCreate)
	EKIZU_EVENT(READY, Ready)

	if (event_type == "LOG") {
		const auto &d = data["d"];

		return ekizu::Log{ d["level"], d["message"] };
	}

	// Special case: Resumed has no data so we return a dumm y object.
	if (event_type == "RESUMED") {
		return ekizu::Resumed{};
	}

	return std::nullopt;
}
} // namespace

namespace ekizu
{
Shard::Shard(ShardId id, std::string_view token, Intents intents)
	: m_id{ id.id }
	, m_config{ std::string{ token }, intents, id.total }
{
}

Result<void> Shard::run(const std::function<void(Event)> &cb)
{
	auto &tq = get_timer_queue();

	tq.set_repeat_mode(true);
	tq.start();

	if (const auto res = connect(cb); !res) {
		return res;
	}

	return {};
}

TimerQueue &Shard::get_timer_queue()
{
	static TimerQueue timer_queue;
	return timer_queue;
}

void Shard::handle_event(tcb::span<const std::byte> data)
{
	const auto json = nlohmann::json::parse(data, nullptr, false);

	if (json.is_discarded() || !json.contains("op") ||
	    !json["op"].is_number()) {
		return;
	}

	if (json.contains("s") && json["s"].is_number()) {
		m_sequence->store(json["s"]);
	}

	switch (static_cast<ekizu::GatewayOpcode>(json["op"].get<uint8_t>())) {
	case ekizu::GatewayOpcode::Dispatch:
		return handle_dispatch(json);
	case ekizu::GatewayOpcode::Heartbeat:
		// https://discord.com/developers/docs/topics/gateway#heartbeat-requests
		return (void)send_heartbeat();
	case ekizu::GatewayOpcode::Reconnect:
		return handle_reconnect();
	case ekizu::GatewayOpcode::InvalidSession:
		return handle_invalid_session(json);
	case ekizu::GatewayOpcode::Hello:
		return handle_hello(json);
	case ekizu::GatewayOpcode::HeartbeatAck:
		return handle_heartbeat_ack();
	default:
		break;
	}
}

void Shard::handle_dispatch(const nlohmann::json &data)
{
	// std::string type = data["t"];
	// const auto msg =
	// 	fmt::format("received dispatch | event_type={}, sequence={}",
	// 		    type, m_sequence->load());

	// log(msg);

	if (!m_on_event) {
		return;
	}

	auto lk = m_on_event->lock();
	auto &cb = static_cast<std::function<void(Event)> &>(lk);

	if (!cb) {
		return;
	}

	const auto event = event_from_str(data);

	if (!event) {
		return;
	}

	cb(*event);
}

void Shard::handle_reconnect()
{
	(void)reconnect(Shard::DISCORD_CLOSE_CODE, false);
}

void Shard::handle_invalid_session(const nlohmann::json &data)
{
	if (data.contains("d") && data["d"].is_boolean() &&
	    !data["d"].get<bool>()) {
		reset_session();
	}

	(void)send_identify();
}

void Shard::handle_hello(const nlohmann::json &data)
{
	if (!data.contains("d")) {
		return;
	}

	const auto &hello = data["d"];

	if (!hello.contains("heartbeat_interval")) {
		return;
	}

	const uint32_t heartbeat_interval = hello["heartbeat_interval"];

	log(fmt::format("received hello | heartbeat_interval={}",
			heartbeat_interval));
	(void)start_heartbeat(get_timer_queue(), heartbeat_interval);
	(void)send_identify();
}

void Shard::handle_heartbeat_ack()
{
	m_last_heartbeat_acked->store(true);
	log("received heartbeat ack");
}

void Shard::log(std::string_view msg, LogLevel level)
{
	handle_dispatch(nlohmann::json{
		{ "t", "LOG" },
		{ "d", { { "message", msg }, { "level", level } } } });
}

Result<void> Shard::set_auto_reconnect(bool auto_reconnect)
{
	if (!m_websocket) {
		return tl::make_unexpected(
			std::make_error_code(std::errc::bad_message));
	}

	m_websocket->set_auto_reconnect(auto_reconnect);

	return {};
}

Result<void> Shard::connect(const std::function<void(Event)> &cb)
{
	if (m_state->load() != ConnectionState::Disconnected) {
		return tl::make_unexpected(
			std::make_error_code(std::errc::already_connected));
	}

	if (m_websocket) {
		if (const auto res = disconnect(Shard::DISCORD_CLOSE_CODE);
		    !res) {
			return res;
		}
	}

	m_on_event = std::make_unique<Mutex<std::function<void(Event)> > >(
		std::function<void(Event)>{ cb });

	net::WebSocketClientBuilder builder;
	const auto compression_enabled = m_config.compression;

	builder.with_auto_reconnect(false)
		.with_on_close([this](net::WebSocketCloseCode code,
				      std::string_view reason) {
			m_inflater.reset();
			m_state->store(ConnectionState::Disconnected);
			log(fmt::format(
				"received close frame | code={}, reason={}",
				static_cast<uint16_t>(code), reason));
		})
		.with_on_connect([this, compression_enabled] {
			if (compression_enabled) {
				auto inflater = Inflater::create();

				if (!inflater) {
					set_auto_reconnect(false);
					(void)disconnect(DISCORD_CLOSE_CODE);
				}

				m_inflater = std::make_unique<Inflater>(
					std::move(*inflater));
			}

			m_state->store(ConnectionState::Connected);
			log("connected to gateway");
		})
		.with_url(GATEWAY_URL);

	if (compression_enabled) {
		builder.with_on_message([this](net::WebSocketMessage msg) {
			const auto is_msg_zlib_compressed =
				[](tcb::span<const std::byte> m) {
					const auto last = m.last(4);
					const auto expected =
						tcb::span<const std::byte, 4>{
							{ std::byte{ 0x00 },
							  std::byte{ 0x00 },
							  std::byte{ 0xFF },
							  std::byte{ 0xFF } }
						};

					return std::equal(last.begin(),
							  last.end(),
							  expected.begin());
				};

			if (!is_msg_zlib_compressed(msg.payload)) {
				handle_event(msg.payload);
				return;
			}

			const auto inflated = m_inflater->inflate(msg.payload);

			if (!inflated) {
				return;
			}

			handle_event(*inflated);
		});
	} else {
		builder.with_on_message([this](net::WebSocketMessage msg) {
			handle_event(msg.payload);
		});
	}

	auto client = builder.build();

	if (!client) {
		return tl::make_unexpected(client.error());
	}

	m_state->store(ConnectionState::Connecting);
	m_websocket = std::move(*client);

	if (const auto res = m_websocket->run(); !res) {
		return res;
	}

	return {};
}

Result<void> Shard::reconnect(net::WebSocketCloseCode code, bool reset)
{
	if (const auto res = disconnect(code); !res) {
		return tl::make_unexpected(res.error());
	}

	if (reset) {
		reset_session();
	}

	m_last_heartbeat_acked->store(false);

	// Websocket will be reconnected automatically, if enabled.
	return {};
}

Result<void> Shard::start_heartbeat(TimerQueue &timer_queue,
				    uint32_t heartbeat_interval)
{
	if (m_state->load() != ConnectionState::Connected) {
		return tl::make_unexpected(
			std::make_error_code(std::errc::not_connected));
	}

	if (m_heartbeat_id == 0) {
		m_heartbeat_id = timer_queue.add_task(
			[this] { (void)send_heartbeat(); },
			std::chrono::milliseconds{ heartbeat_interval });
	}

	return {};
}

Result<void> Shard::send_heartbeat()
{
	if (m_state->load() != ConnectionState::Connected || !m_websocket) {
		return tl::make_unexpected(
			std::make_error_code(std::errc::not_connected));
	}

	if (!m_last_heartbeat_acked->load()) {
		log("connection is failed or \"zombied\"");
		return reconnect(Shard::DISCORD_SESSION_EXPIRED_CODE, true);
	}

	m_last_heartbeat_acked->store(false);

	const nlohmann::json payload{
		{ "op", static_cast<uint8_t>(GatewayOpcode::Heartbeat) },
		{ "d", m_sequence->load() },
	};

	log(fmt::format("sending heartbeat | sequence={}", m_sequence->load()));

	return m_websocket->send(payload.dump());
}

Result<void> Shard::send_identify()
{
	if (m_state->load() != ConnectionState::Connected || !m_websocket) {
		return tl::make_unexpected(
			std::make_error_code(std::errc::not_connected));
	}

	const auto [identify, opcode] = [this] {
		{
			auto lk = m_session_id->lock();
			std::string &id = lk;

			if (!id.empty()) {
				return std::make_pair(
					nlohmann::json{
						{ "token", m_config.token },
						{ "session_id", id },
						{ "seq", m_sequence->load() },
					},
					GatewayOpcode::Resume);
			}
		}

		nlohmann::json j{
			{ "token", m_config.token },
			{ "compress", false },
			{ "shard", { m_id, m_config.shard_count } },
			{ "presence",
			  {
				  { "status", "online" },
				  { "since", 0 },
				  { "activities", {} },
				  { "afk", false },
			  } },
		};

		if (m_config.is_bot) {
			j.merge_patch(
				{ { "properties",
				    { { "$os", "Linux" },
				      { "$browser", "ekizu" },
				      { "$device", "ekizu" } } },
				  { "large_threshold", 250 }, // NOLINT
				  { "intents", static_cast<uint32_t>(
						       *m_config.intents) } });
		} else {
			j.merge_patch({
				{ "client_state",
				  {
					  { "guild_hashes", {} },
					  { "highest_last_message_id", "0" },
					  { "read_state_version", 0 },
					  { "user_guild_settings_version", -1 },
					  { "user_settings_version", -1 },
				  } },
				{ "properties",
				  { { "browser_user_agent",
				      "Mozilla/5.0 (Windows NT 10.0; Win64; "
				      "x64)"
				      "AppleWebKit/537.36 (KHTML, like Gecko) "
				      "Chrome/103.0.0.0 "
				      "Safari/537.36" },
				    { "browser_version", "103.0.0.0" },
				    { "client_build_number", 137095 }, // NOLINT
				    { "os", "Windows" },
				    { "device", "" },
				    { "os_version", "10" },
				    { "referrer", "" },
				    { "referrer_current", "" },
				    { "referring_domain", "" },
				    { "referring_domain_current", "" },
				    { "release_channel", "stable" },
				    { "system_locale", "en-US" } } },
			});
		}

		return std::make_pair(j, GatewayOpcode::Identify);
	}();

	const nlohmann::json payload = {
		{ "op", static_cast<uint8_t>(opcode) },
		{ "d", identify },
	};

	if (const auto res = m_websocket->send(payload.dump()); !res) {
		return tl::make_unexpected(res.error());
	}

	return {};
}

Result<void> Shard::disconnect(net::WebSocketCloseCode code)
{
	if (m_state->load() != ConnectionState::Connected || !m_websocket) {
		return tl::make_unexpected(
			std::make_error_code(std::errc::not_connected));
	}

	return m_websocket->close(code);
}
} // namespace ekizu
