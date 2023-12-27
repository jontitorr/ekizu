#include <boost/asio/detached.hpp>
#include <boost/outcome/try.hpp>
#include <ekizu/shard.hpp>
#include <nlohmann/json.hpp>

namespace {
// TODO: Add gateway url customization
constexpr const char *GATEWAY_URL =
	"wss://gateway.discord.gg/?v=10&encoding=json&compress=zlib-stream";
constexpr const char *GATEWAY_JSON_ZLIB_QUERY =
	"?v=10&encoding=json&compress=zlib-stream";

ekizu::Result<ekizu::Event> event_from_str(std::string_view event_type,
										   const nlohmann::json &data) {
#define EKIZU_EVENT(s, v) \
	if (event_type == #s) { return data.get<ekizu::v>(); }

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

	return boost::system::errc::invalid_argument;
}
}  // namespace

namespace ekizu {
Shard::Shard(ShardId id, std::string_view token, Intents intents)
	: m_id{id.id}, m_config{std::string{token}, intents, id.total} {}

void Shard::attach_logger(std::function<void(Log)> on_log) {
	m_on_log = std::move(on_log);
}

Result<> Shard::close(CloseFrame reason,
					  const boost::asio::yield_context &yield) {
	if (!m_ws) { return boost::system::errc::not_connected; }
	if (m_timer) { m_timer.reset(); }

	if (reason.code == net::ws::close_code::normal ||
		reason.code == net::ws::close_code::going_away) {
		m_resume_gateway_url.reset();
		m_session.reset();
	}

	log(fmt::format("sending websocket close message | code={}, reason={}",
					reason.code, reason.reason.data()));
	return m_ws->close(
		net::ws::close_reason{static_cast<net::ws::close_code>(reason.code),
							  boost::string_view{
								  reason.reason,
							  }},
		yield);
}

Result<Event> Shard::next_event(const boost::asio::yield_context &yield) {
	BOOST_OUTCOME_TRY(auto msg, next_message(yield));

	if (m_inflater && msg.is_binary) {
		BOOST_OUTCOME_TRY(auto inflated, m_inflater->inflate(msg.payload));
		msg.payload = inflated;
	}

	return handle_event(msg.payload, yield);
}

Result<Event> Shard::handle_event(std::string_view data,
								  const boost::asio::yield_context &yield) {
	const auto json = nlohmann::json::parse(data, nullptr, false);

	if (json.is_discarded() || !json.contains("op") ||
		!json["op"].is_number()) {
		return boost::system::errc::invalid_argument;
	}

	switch (static_cast<ekizu::GatewayOpcode>(json["op"].get<uint8_t>())) {
		case ekizu::GatewayOpcode::Dispatch: return handle_dispatch(json);
		case ekizu::GatewayOpcode::Heartbeat: {
			// https://discord.com/developers/docs/topics/gateway#heartbeat-requests
			BOOST_OUTCOME_TRY(auto r, send_heartbeat(yield));
			break;
		}
		case ekizu::GatewayOpcode::Reconnect: {
			BOOST_OUTCOME_TRY(auto r, handle_reconnect(yield));
			break;
		}
		case ekizu::GatewayOpcode::InvalidSession: {
			BOOST_OUTCOME_TRY(auto r, handle_invalid_session(json, yield));
			break;
		}
		case ekizu::GatewayOpcode::Hello: {
			BOOST_OUTCOME_TRY(auto r, handle_hello(json, yield));
			break;
		}
		case ekizu::GatewayOpcode::HeartbeatAck: {
			handle_heartbeat_ack();
			break;
		}
		default: break;
	}

	// Not a failure, but not a success either.
	return boost::system::error_code{};
}

Result<Event> Shard::handle_dispatch(const nlohmann::json &data) {
	if (!data.contains("t") || !data["t"].is_string() || !data.contains("d") ||
		!data["d"].is_object()) {
		return boost::system::errc::invalid_argument;
	}

	std::optional<uint64_t> sequence;
	if (data.contains("s") && data["s"].is_number()) { sequence = data["s"]; }

	const std::string event_type = data["t"];
	const auto &event = data["d"];

	log(fmt::format(
		"received dispatch {{t: {}, s: {}, d: {}}}", event_type,
		sequence ? boost::to_string(*sequence) : "null", event.dump()));

	if (event_type == "READY") {
		if (!event.contains("resume_gateway_url") ||
			!event["resume_gateway_url"].is_string() ||
			!event.contains("session_id") || !event["session_id"].is_string()) {
			return boost::system::errc::invalid_argument;
		}

		m_session.emplace(
			event["session_id"].get<std::string>(), sequence.value_or(0));
		m_resume_gateway_url = event["resume_gateway_url"].get<std::string>();

		log(fmt::format("received ready | resume_gateway_url={}, session_id={}",
						*m_resume_gateway_url, m_session->id));
	}

	if (m_session && sequence) {
		// TODO: Handle out of order sequences.
		m_session->sequence = *sequence;
	}

	return event_from_str(event_type, event);
}

Result<> Shard::handle_reconnect(const boost::asio::yield_context &yield) {
	log("received reconnect");
	return close(CloseFrame::RESUME, yield);
}

Result<> Shard::handle_invalid_session(
	const nlohmann::json &data, const boost::asio::yield_context &yield) {
	if (!data.contains("d") || !data["d"].is_boolean()) {
		return boost::system::errc::invalid_argument;
	}

	bool resumable = data["d"];
	log(fmt::format("received invalid session | resumable={}", resumable));
	return close(resumable ? CloseFrame::RESUME : CloseFrame::NORMAL, yield);
}

Result<> Shard::handle_hello(const nlohmann::json &data,
							 const boost::asio::yield_context &yield) {
	m_last_heartbeat_acked = true;
	if (!data.contains("d")) { return boost::system::errc::invalid_argument; }

	const auto &hello = data["d"];

	if (!hello.contains("heartbeat_interval") ||
		!hello["heartbeat_interval"].is_number_unsigned()) {
		return boost::system::errc::invalid_argument;
	}

	const uint32_t heartbeat_interval = hello["heartbeat_interval"];

	log(fmt::format(
		"received hello | heartbeat_interval={}", heartbeat_interval));

	BOOST_OUTCOME_TRY(
		auto r, start_heartbeat(heartbeat_interval, yield.get_executor()));

	return m_session ? send_resume(yield) : send_identify(yield);
}

void Shard::handle_heartbeat_ack() {
	m_last_heartbeat_acked = true;
	log("received heartbeat ack");
}

void Shard::log(std::string_view msg, LogLevel level) const {
	if (!m_on_log) { return; }
	m_on_log(Log{
		level,
		fmt::format(
			"shard{{id=[{}, {}]}}: {}", m_id, m_config.shard_count, msg),
	});
}

Result<net::WebSocketMessage> Shard::next_message(
	const boost::asio::yield_context &yield) {
	static uint8_t reconnect_attempts{};
	static constexpr uint8_t max_reconnect_attempts{5};

	while (true) {
		if (reconnect_attempts >= max_reconnect_attempts) {
			return boost::system::errc::not_connected;
		}

		if (!m_ws || !m_ws->is_open()) {
			BOOST_OUTCOME_TRY(auto r, reconnect(yield));
			reconnect_attempts++;
		}

		auto res = m_ws->read(yield);

		if (res) { return res.value(); }
		const auto ec = res.error().value();

		log(fmt::format(
				"read error | ec={}, msg={}", ec, res.error().message()),
			LogLevel::Error);

		if (ec != boost::asio::error::operation_aborted &&
			ec != boost::asio::error::eof &&
			ec != boost::asio::error::connection_reset) {
			return res.error();
		}
	}
}

Result<> Shard::reconnect(const boost::asio::yield_context &yield) {
	auto url = m_resume_gateway_url
				   ? fmt::format("{}/{}", *m_resume_gateway_url,
								 GATEWAY_JSON_ZLIB_QUERY)
				   : GATEWAY_URL;
	log(fmt::format(
		"{}connecting to {}", m_resume_gateway_url ? "re" : "", url));

	BOOST_OUTCOME_TRY(auto ws, net::WebSocketClient::connect(url, yield));
	m_ws.emplace(std::move(ws));

	if (m_config.compression) {
		BOOST_OUTCOME_TRY(auto inflater, Inflater::create());
		m_inflater.emplace(std::move(inflater));
	}

	return outcome::success();
}

Result<> Shard::start_heartbeat(uint32_t heartbeat_interval,
								const boost::asio::any_io_executor &executor) {
	if (!m_ws) { return boost::system::errc::not_connected; }

	m_heartbeat_interval = heartbeat_interval;

	if (!m_timer) {
		m_timer = boost::asio::deadline_timer{
			executor, boost::posix_time::milliseconds(heartbeat_interval)};

		boost::asio::spawn(
			executor,
			[this](auto yield) {
				boost::system::error_code ec;

				while (true) {
					m_timer->async_wait(yield[ec]);

					if (ec == boost::asio::error::operation_aborted) { return; }

					if (ec) {
						return log(
							fmt::format("failed to send heartbeat | ec={}",
										ec.message()),
							LogLevel::Error);
					}

					if (!m_ws) { return; }

					if (!m_last_heartbeat_acked) {
						log("connection is failed or \"zombied\"");
						return boost::ignore_unused(
							close(CloseFrame::SESSION_EXPIRED, yield));
					}

					m_last_heartbeat_acked = false;
					m_timer->expires_from_now(
						boost::posix_time::milliseconds(m_heartbeat_interval));

					if (auto r = send_heartbeat(yield); !r) {
						return log(
							fmt::format("failed to send heartbeat | ec={}",
										r.error().message()),
							LogLevel::Error);
					}
				}
			},
			boost::asio::detached);
		log("started heartbeat timer");
	}

	return outcome::success();
}

Result<> Shard::send_heartbeat(const boost::asio::yield_context &yield) {
	if (!m_ws) { return boost::system::errc::not_connected; }
	nlohmann::json d{nullptr};

	if (m_session) { d = m_session->sequence; }

	const nlohmann::json payload{
		{"op", static_cast<uint8_t>(GatewayOpcode::Heartbeat)},
		{"d", d},
	};

	log(fmt::format(
		"sending heartbeat | sequence={}",
		m_session ? boost::to_string(m_session->sequence) : "null"));
	return m_ws->send(payload.dump(), yield);
}

Result<> Shard::send_identify(const boost::asio::yield_context &yield) {
	if (!m_ws) { return boost::system::errc::not_connected; }

	nlohmann::json d{
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
		d.merge_patch(
			{{"properties",
			  {{"$os", "Linux"}, {"$browser", "ekizu"}, {"$device", "ekizu"}}},
			 {"large_threshold", 250},	// NOLINT
			 {"intents", static_cast<uint32_t>(*m_config.intents)}});
	} else {
		d.merge_patch({
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

	const nlohmann::json payload = {
		{"op", static_cast<uint8_t>(GatewayOpcode::Identify)},
		{"d", d},
	};

	return m_ws->send(payload.dump(), yield);
}

Result<> Shard::send_resume(const boost::asio::yield_context &yield) {
	if (!m_ws) { return boost::system::errc::not_connected; }
	if (!m_session) { return boost::system::errc::operation_not_permitted; }

	const nlohmann::json payload = {
		{"op", static_cast<uint8_t>(GatewayOpcode::Resume)},
		{"d",
		 {
			 {"token", m_config.token},
			 {"session_id", m_session->id},
			 {"seq", m_session->sequence},
		 }},
	};

	log(fmt::format("sending resume | session_id={}, sequence={}",
					m_session->id, m_session->sequence));

	return m_ws->send(payload.dump(), yield);
}
}  // namespace ekizu
