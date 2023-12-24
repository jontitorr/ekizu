#include <ekizu/shard.hpp>
#include <nlohmann/json.hpp>

namespace {
// TODO: Add gateway url customization
constexpr const char *GATEWAY_URL =
	"wss://gateway.discord.gg/?v=10&encoding=json&compress=zlib-stream";

std::optional<ekizu::Event> event_from_str(const nlohmann::json &data) {
	if (!data.contains("t") || !data.contains("d")) { return std::nullopt; }

	const std::string event_type = data["t"];
#define EKIZU_EVENT(s, v) \
	if (event_type == #s) { return data["d"].get<ekizu::v>(); }

	EKIZU_EVENT(CHANNEL_CREATE, ChannelCreate)
	EKIZU_EVENT(CHANNEL_DELETE, ChannelDelete)
	EKIZU_EVENT(CHANNEL_PINS_UPDATE, ChannelPinsUpdate)
	EKIZU_EVENT(CHANNEL_UPDATE, ChannelUpdate)
	EKIZU_EVENT(GUILD_BAN_ADD, GuildBanAdd)
	EKIZU_EVENT(GUILD_BAN_REMOVE, GuildBanRemove)
	EKIZU_EVENT(GUILD_CREATE, GuildCreate)
	EKIZU_EVENT(GUILD_DELETE, GuildDelete)
	EKIZU_EVENT(GUILD_EMOJIS_UPDATE, GuildEmojisUpdate)
	EKIZU_EVENT(GUILD_INTEGRATIONS_UPDATE, GuildIntegrationsUpdate)
	EKIZU_EVENT(GUILD_MEMBER_ADD, GuildMemberAdd)
	EKIZU_EVENT(GUILD_MEMBER_REMOVE, GuildMemberRemove)
	EKIZU_EVENT(GUILD_MEMBER_UPDATE, GuildMemberUpdate)
	EKIZU_EVENT(GUILD_MEMBERS_CHUNK, GuildMembersChunk)
	EKIZU_EVENT(GUILD_ROLE_CREATE, GuildRoleCreate)
	EKIZU_EVENT(GUILD_ROLE_DELETE, GuildRoleDelete)
	EKIZU_EVENT(GUILD_ROLE_UPDATE, GuildRoleUpdate)
	EKIZU_EVENT(GUILD_SCHEDULED_EVENT_CREATE, GuildScheduledEventCreate)
	EKIZU_EVENT(GUILD_SCHEDULED_EVENT_DELETE, GuildScheduledEventDelete)
	EKIZU_EVENT(GUILD_SCHEDULED_EVENT_UPDATE, GuildScheduledEventUpdate)
	EKIZU_EVENT(GUILD_SCHEDULED_EVENT_USER_ADD, GuildScheduledEventUserAdd)
	EKIZU_EVENT(
		GUILD_SCHEDULED_EVENT_USER_REMOVE, GuildScheduledEventUserRemove)
	EKIZU_EVENT(GUILD_STICKERS_UPDATE, GuildStickersUpdate)
	EKIZU_EVENT(GUILD_UPDATE, GuildUpdate)
	EKIZU_EVENT(INTEGRATION_CREATE, IntegrationCreate)
	EKIZU_EVENT(INTEGRATION_DELETE, IntegrationDelete)
	EKIZU_EVENT(INTEGRATION_UPDATE, IntegrationUpdate)
	EKIZU_EVENT(INTERACTION_CREATE, InteractionCreate)
	EKIZU_EVENT(INVITE_CREATE, InviteCreate)
	EKIZU_EVENT(INVITE_DELETE, InviteDelete)
	EKIZU_EVENT(MESSAGE_CREATE, MessageCreate)
	EKIZU_EVENT(MESSAGE_DELETE, MessageDelete)
	EKIZU_EVENT(MESSAGE_DELETE_BULK, MessageDeleteBulk)
	EKIZU_EVENT(MESSAGE_REACTION_ADD, MessageReactionAdd)
	EKIZU_EVENT(MESSAGE_REACTION_REMOVE, MessageReactionRemove)
	EKIZU_EVENT(MESSAGE_REACTION_REMOVE_ALL, MessageReactionRemoveAll)
	EKIZU_EVENT(MESSAGE_REACTION_REMOVE_EMOJI, MessageReactionRemoveEmoji)
	EKIZU_EVENT(MESSAGE_UPDATE, MessageUpdate)
	EKIZU_EVENT(PRESENCE_UPDATE, PresenceUpdate)
	EKIZU_EVENT(READY, Ready)

	// Special case: Resumed has no data so we return a dummy object.
	if (event_type == "RESUMED") { return ekizu::Resumed{}; }

	EKIZU_EVENT(STAGE_INSTANCE_CREATE, StageInstanceCreate)
	EKIZU_EVENT(STAGE_INSTANCE_DELETE, StageInstanceDelete)
	EKIZU_EVENT(STAGE_INSTANCE_UPDATE, StageInstanceUpdate)
	EKIZU_EVENT(THREAD_CREATE, ThreadCreate)
	EKIZU_EVENT(THREAD_DELETE, ThreadDelete)
	EKIZU_EVENT(THREAD_LIST_SYNC, ThreadListSync)
	EKIZU_EVENT(THREAD_MEMBER_UPDATE, ThreadMemberUpdate)
	EKIZU_EVENT(THREAD_MEMBERS_UPDATE, ThreadMembersUpdate)
	EKIZU_EVENT(THREAD_UPDATE, ThreadUpdate)
	EKIZU_EVENT(TYPING_START, TypingStart)
	EKIZU_EVENT(USER_UPDATE, UserUpdate)
	EKIZU_EVENT(VOICE_SERVER_UPDATE, VoiceServerUpdate)
	EKIZU_EVENT(VOICE_STATE_UPDATE, VoiceStateUpdate)
	EKIZU_EVENT(WEBHOOKS_UPDATE, WebhooksUpdate)

	return std::nullopt;
}
}  // namespace

namespace ekizu {
Shard::Shard(boost::asio::io_context &ctx, ShardId id, std::string_view token,
			 Intents intents)
	: m_ctx{ctx},
	  m_id{id.id},
	  m_config{std::string{token}, intents, id.total} {}

Result<void> Shard::connect(const std::function<void(Event)> &cb) {
	if (m_state != ConnectionState::Disconnected) {
		return boost::system::errc::already_connected;
	}

	if (m_ws) {
		if (const auto res = disconnect(Shard::RESUME); !res) { return res; }
	}

	m_on_event = cb;

	net::WebSocketClientBuilder builder;
	const auto compression_enabled = m_config.compression;

	builder.with_auto_reconnect(true)
		.with_on_close([this](net::ws::close_reason reason) {
			m_timer.reset();
			m_state = ConnectionState::Disconnected;
			log(fmt::format("received close frame {{code: {}, reason: {}}}",
							reason.code, reason.reason.data()));
		})
		.with_on_connect([this, compression_enabled] {
			if (compression_enabled) {
				auto inflater = Inflater::create();

				if (!inflater) {
					(void)set_auto_reconnect(false);
					(void)disconnect(RESUME);
				}

				m_inflater =
					std::make_unique<Inflater>(std::move(inflater.value()));
			}

			m_state = ConnectionState::Connected;
			log("connected to gateway");
		})
		.with_on_error([this](auto ec, auto msg) {
			log(fmt::format("websocket error: {}-{}", msg, ec.message()),
				LogLevel::Error);
		})
		.with_url(GATEWAY_URL);

	if (compression_enabled) {
		builder.with_on_message([this](boost::span<const std::byte> msg) {
			if (!Inflater::is_compressed(msg)) { return handle_event(msg); }
			const auto inflated = m_inflater->inflate(msg);
			if (!inflated) { return; }
			handle_event(inflated.value());
		});
	} else {
		builder.with_on_message([this](boost::span<const std::byte> msg) {
			handle_event(msg);
		});
	}

	m_state = ConnectionState::Connecting;
	m_ws.emplace(builder.build());
	return m_ws->run(m_ctx);
}

void Shard::handle_event(boost::span<const std::byte> data) {
	const auto json = nlohmann::json::parse(data, nullptr, false);

	if (json.is_discarded() || !json.contains("op") ||
		!json["op"].is_number()) {
		return;
	}

	if (json.contains("s") && json["s"].is_number()) { m_sequence = json["s"]; }

	switch (static_cast<ekizu::GatewayOpcode>(json["op"].get<uint8_t>())) {
		case ekizu::GatewayOpcode::Dispatch: return handle_dispatch(json);
		case ekizu::GatewayOpcode::Heartbeat: {
			// https://discord.com/developers/docs/topics/gateway#heartbeat-requests
			boost::system::error_code ec;
			return (void)send_heartbeat(ec);
		}
		case ekizu::GatewayOpcode::Reconnect: return handle_reconnect();
		case ekizu::GatewayOpcode::InvalidSession:
			return handle_invalid_session(json);
		case ekizu::GatewayOpcode::Hello: return handle_hello(json);
		case ekizu::GatewayOpcode::HeartbeatAck: return handle_heartbeat_ack();
		default: break;
	}
}

void Shard::handle_dispatch(const nlohmann::json &data) {
	if (data.contains("t") && data["t"].is_string()) {
		log(fmt::format("received dispatch {{t: {}, s: {}, d: {}}}",
						data["t"].get<std::string>(), m_sequence, data.dump()));
	}

	if (!m_on_event) { return; }
	const auto event = event_from_str(data);
	if (!event) { return; }
	m_on_event(*event);
}

void Shard::handle_reconnect() {
	log("received reconnect");
	(void)reconnect(Shard::RESUME, false);
}

void Shard::handle_invalid_session(const nlohmann::json &data) {
	log("received invalid session");

	if (data.contains("d") && data["d"].is_boolean() &&
		!data["d"].get<bool>()) {
		reset_session();
	}

	(void)send_identify();
}

void Shard::handle_hello(const nlohmann::json &data) {
	if (!data.contains("d")) { return; }

	const auto &hello = data["d"];

	if (!hello.contains("heartbeat_interval") ||
		!hello["heartbeat_interval"].is_number_unsigned()) {
		return;
	}

	const uint32_t heartbeat_interval = hello["heartbeat_interval"];

	log(fmt::format(
		"received hello | heartbeat_interval={}", heartbeat_interval));
	(void)start_heartbeat(heartbeat_interval);
	(void)send_identify();
}

void Shard::handle_heartbeat_ack() {
	m_last_heartbeat_acked = true;
	log("received heartbeat ack");
}

void Shard::log(std::string_view msg, LogLevel level) const {
	if (!m_on_event) { return; }
	m_on_event(Log{
		level,
		fmt::format(
			"shard{{id=[{}, {}]}}: {}", m_id, m_config.shard_count, msg),
	});
}

Result<void> Shard::set_auto_reconnect(bool auto_reconnect) {
	if (!m_ws) { return boost::system::errc::not_connected; }

	m_ws->set_auto_reconnect(auto_reconnect);
	return outcome::success();
}

Result<void> Shard::reconnect(net::ws::close_reason reason, bool reset) {
	if (auto res = disconnect(std::move(reason)); !res) { return res; }
	if (reset) { reset_session(); }

	m_last_heartbeat_acked = false;
	// Websocket will be reconnected automatically, if enabled.
	return outcome::success();
}

Result<void> Shard::start_heartbeat(uint32_t heartbeat_interval) {
	if (m_state != ConnectionState::Connected) {
		return boost::system::errc::not_connected;
	}

	m_heartbeat_interval = heartbeat_interval;

	if (!m_timer) {
		m_timer = boost::asio::deadline_timer{
			m_ctx, boost::posix_time::milliseconds(heartbeat_interval)};
		m_timer->async_wait([this](const boost::system::error_code &ec) {
			(void)send_heartbeat(ec);
		});
		log("started heartbeat timer");
	}

	return outcome::success();
}

Result<void> Shard::send_heartbeat(const boost::system::error_code &ec) {
	if (ec) {
		log(fmt::format("failed to send heartbeat | ec={}", ec.message()),
			LogLevel::Error);
		return ec;
	}

	if (m_state != ConnectionState::Connected || !m_ws) {
		return boost::system::errc::not_connected;
	}

	if (!m_last_heartbeat_acked) {
		log("connection is failed or \"zombied\"");
		return reconnect(SESSION_EXPIRED, true);
	}

	m_last_heartbeat_acked = false;

	const nlohmann::json payload{
		{"op", static_cast<uint8_t>(GatewayOpcode::Heartbeat)},
		{"d", m_sequence},
	};

	log(fmt::format("sending heartbeat | sequence={}", m_sequence));

	m_ws->send(payload.dump());

	if (m_timer) {
		m_timer->expires_from_now(
			boost::posix_time::milliseconds(m_heartbeat_interval));
		m_timer->async_wait([this](const boost::system::error_code &errc) {
			(void)send_heartbeat(errc);
		});
	}

	return outcome::success();
}

Result<void> Shard::send_identify() {
	if (m_state != ConnectionState::Connected || !m_ws) {
		return boost::system::errc::not_connected;
	}

	const auto [identify, opcode] = [this] {
		if (!m_session_id.empty()) {
			return std::make_pair(
				nlohmann::json{
					{"token", m_config.token},
					{"session_id", m_session_id},
					{"seq", m_sequence},
				},
				GatewayOpcode::Resume);
		}

		nlohmann::json j{
			{"token", m_config.token},
			{"compress", false},
			{"shard", {m_id, m_config.shard_count}},
			{"presence",
			 {
				 {"status", "online"},
				 {"since", 0},
				 {"activities", {}},
				 {"afk", false},
			 }},
		};

		if (m_config.is_bot) {
			j.merge_patch(
				{{"properties",
				  {{"$os", "Linux"},
				   {"$browser", "ekizu"},
				   {"$device", "ekizu"}}},
				 {"large_threshold", 250},	// NOLINT
				 {"intents", static_cast<uint32_t>(*m_config.intents)}});
		} else {
			j.merge_patch({
				{"client_state",
				 {
					 {"guild_hashes", {}},
					 {"highest_last_message_id", "0"},
					 {"read_state_version", 0},
					 {"user_guild_settings_version", -1},
					 {"user_settings_version", -1},
				 }},
				{"properties",
				 {{"browser_user_agent",
				   "Mozilla/5.0 (Windows NT 10.0; Win64; "
				   "x64)"
				   "AppleWebKit/537.36 (KHTML, like Gecko) "
				   "Chrome/103.0.0.0 "
				   "Safari/537.36"},
				  {"browser_version", "103.0.0.0"},
				  {"client_build_number", 137095},	// NOLINT
				  {"os", "Windows"},
				  {"device", ""},
				  {"os_version", "10"},
				  {"referrer", ""},
				  {"referrer_current", ""},
				  {"referring_domain", ""},
				  {"referring_domain_current", ""},
				  {"release_channel", "stable"},
				  {"system_locale", "en-US"}}},
			});
		}

		return std::make_pair(j, GatewayOpcode::Identify);
	}();

	const nlohmann::json payload = {
		{"op", static_cast<uint8_t>(opcode)},
		{"d", identify},
	};

	m_ws->send(payload.dump());
	return outcome::success();
}

Result<void> Shard::disconnect(net::ws::close_reason reason) {
	if (m_state != ConnectionState::Connected || !m_ws) {
		return boost::system::errc::not_connected;
	}

	log(fmt::format("sending websocket close message | code={}, reason={}",
					reason.code, reason.reason.data()));
	m_ws->close(std::move(reason));
	return outcome::success();
}
}  // namespace ekizu
