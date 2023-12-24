#ifndef EKIZU_SHARD_HPP
#define EKIZU_SHARD_HPP

#include <boost/asio/deadline_timer.hpp>
#include <ekizu/event.hpp>
#include <ekizu/inflater.hpp>
#include <ekizu/intents.hpp>
#include <ekizu/mutex.hpp>
#include <ekizu/ws.hpp>

namespace ekizu {
struct Client;
struct ShardAttorney;
struct ShardManager;

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

/**
 * @brief Represents a Discord shard, which is a connection to the Discord
 * gateway.
 */
struct Shard {
	Shard(boost::asio::io_context &ctx, ShardId id, std::string_view token,
		  Intents intents);

	static const net::ws::close_reason RESUME;
	static const net::ws::close_reason SESSION_EXPIRED;

	[[nodiscard]] uint64_t id() const { return m_id; }

	[[nodiscard]] Result<void> connect(const std::function<void(Event)> &cb);

   private:
	Result<void> set_auto_reconnect(bool auto_reconnect);

	struct Config {
		std::string token;
		std::optional<Intents> intents;
		uint64_t shard_count{1};
		bool compression{true};
		bool is_bot{true};
	};

	void handle_event(boost::span<const std::byte> data);
	void handle_dispatch(const nlohmann::json &data);
	void handle_reconnect();
	void handle_invalid_session(const nlohmann::json &data);
	void handle_hello(const nlohmann::json &data);
	void handle_heartbeat_ack();
	void log(std::string_view msg, LogLevel level = LogLevel::Debug) const;

	void reset_session() {
		m_sequence = 0;
		m_session_id.clear();
	}

	[[nodiscard]] Result<void> reconnect(net::ws::close_reason reason,
										 bool reset);

	[[nodiscard]] Result<void> start_heartbeat(uint32_t heartbeat_interval);

	[[nodiscard]] Result<void> send_heartbeat(
		const boost::system::error_code &ec);

	[[nodiscard]] Result<void> send_identify();

	[[nodiscard]] Result<void> disconnect(net::ws::close_reason reason);

	boost::asio::io_context &m_ctx;
	std::optional<boost::asio::deadline_timer> m_timer;
	uint64_t m_id;
	Config m_config;
	bool m_last_heartbeat_acked{true};
	std::string m_session_id;
	uint64_t m_sequence{};
	uint32_t m_heartbeat_interval{};

	enum class ConnectionState : uint8_t {
		Disconnected,
		Connecting,
		Connected,
	};

	ConnectionState m_state{ConnectionState::Disconnected};

	/// May or may not be used based on runtime options.
	std::unique_ptr<Inflater> m_inflater;
	std::function<void(Event)> m_on_event;
	std::optional<net::WebSocketClient> m_ws;
};

inline const net::ws::close_reason Shard::RESUME = {
	static_cast<net::ws::close_code>(4000), "resuming connection"};
inline const net::ws::close_reason Shard::SESSION_EXPIRED = {
	static_cast<net::ws::close_code>(4009), "session expired"};
}  // namespace ekizu

#endif	// EKIZU_SHARD_HPP
