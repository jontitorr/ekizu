#ifndef EKIZU_SHARD_HPP
#define EKIZU_SHARD_HPP

#include <boost/asio/deadline_timer.hpp>
#include <ekizu/event.hpp>
#include <ekizu/inflater.hpp>
#include <ekizu/intents.hpp>
#include <ekizu/log.hpp>
#include <ekizu/ws.hpp>

namespace ekizu {
struct Client;
struct ShardAttorney;
struct ShardManager;

namespace detail {
struct CloseFrame {
	constexpr CloseFrame(std::uint16_t code_, std::string_view reason_)
		: code{code_}, reason{reason_.begin(), reason_.end()} {}

	std::uint16_t code;
	net::ws::reason_string reason;
};

struct CloseFrameConstants {
	static constexpr CloseFrame NORMAL{1000, "closing connection"};
	static constexpr CloseFrame RESUME{4000, "resuming connection"};
	static constexpr CloseFrame SESSION_EXPIRED{4009, "session expired"};
};
}  // namespace detail

struct CloseFrame : detail::CloseFrame, detail::CloseFrameConstants {
	using detail::CloseFrame::CloseFrame;
	constexpr CloseFrame(const detail::CloseFrame &base)
		: detail::CloseFrame(base) {}
};

/**
 * @brief The Discord gateway opcodes.
 */
enum class GatewayOpcode : uint8_t {
	Dispatch = 0,
	Heartbeat = 1,
	Identify = 2,
	PresenceUpdate = 3,
	VoiceStateUpdate = 4,
	Resume = 6,
	Reconnect = 7,
	RequestGuildMembers = 8,
	InvalidSession = 9,
	Hello = 10,
	HeartbeatAck = 11,
};

struct ShardId {
	/// The shard ID.
	uint64_t id;
	/// The total number of shards.
	uint64_t total;

	static const ShardId ONE;
};

inline const ShardId ShardId::ONE{0, 1};

struct UpdatePresence {
	std::optional<uint64_t> idle_since{};
	std::vector<Activity> activities{};
	Status status{};
	bool afk{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const UpdatePresence &p);

/**
 * @brief Represents a Discord shard, which is a connection to the Discord
 * gateway.
 */
struct Shard {
	struct Config {
		std::string token;
		std::optional<Intents> intents;
		uint64_t shard_count{1};
		bool compression{true};
		bool is_bot{true};
	};

	EKIZU_EXPORT Shard(ShardId id, std::string_view token, Intents intents);

	[[nodiscard]] uint64_t id() const { return m_id; }

	EKIZU_EXPORT void attach_logger(std::function<void(Log)> on_log);
	EKIZU_EXPORT Result<> close(CloseFrame reason,
								const boost::asio::yield_context &yield);
	EKIZU_EXPORT Result<> join_voice_channel(
		Snowflake guild_id, Snowflake channel_id,
		const boost::asio::yield_context &yield);
	EKIZU_EXPORT Result<> leave_voice_channel(
		Snowflake guild_id, const boost::asio::yield_context &yield);
	[[nodiscard]] EKIZU_EXPORT Result<Event> next_event(
		const boost::asio::yield_context &yield);
	EKIZU_EXPORT Result<> update_presence(
		UpdatePresence presence, const boost::asio::yield_context &yield);

   private:
	struct Session {
		Session(std::string_view id_, uint64_t sequence_)
			: id{id_}, sequence{sequence_} {}

		std::string id;
		uint64_t sequence{};
	};

	Result<Event> handle_event(std::string_view data,
							   const boost::asio::yield_context &yield);
	Result<Event> handle_dispatch(const nlohmann::json &data);
	Result<> handle_reconnect(const boost::asio::yield_context &yield);
	Result<> handle_invalid_session(const nlohmann::json &data,
									const boost::asio::yield_context &yield);
	Result<> handle_hello(const nlohmann::json &data,
						  const boost::asio::yield_context &yield);
	void handle_heartbeat_ack();

	void log(std::string_view msg, LogLevel level = LogLevel::Debug) const;
	Result<net::WebSocketMessage> next_message(
		const boost::asio::yield_context &yield);
	Result<> reconnect(const boost::asio::yield_context &yield);
	Result<> start_heartbeat(uint32_t heartbeat_interval,
							 const boost::asio::any_io_executor &executor);
	Result<> send_heartbeat(const boost::asio::yield_context &yield);
	Result<> send_identify(const boost::asio::yield_context &yield);
	Result<> send_resume(const boost::asio::yield_context &yield);

	std::optional<boost::asio::deadline_timer> m_timer;
	uint64_t m_id;
	Config m_config;
	bool m_last_heartbeat_acked{true};
	uint32_t m_heartbeat_interval{};
	std::function<void(Log)> m_on_log;
	/// May or may not be used based on runtime options.
	std::optional<Inflater> m_inflater;
	std::optional<Session> m_session;
	std::optional<std::string> m_resume_gateway_url;
	std::optional<net::WebSocketClient> m_ws;
	uint64_t m_reconnect_attempts{};
};
}  // namespace ekizu

#endif	// EKIZU_SHARD_HPP
