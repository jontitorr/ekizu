#ifndef EKIZU_SHARD_HPP
#define EKIZU_SHARD_HPP

#include <ekizu/event.hpp>
#include <ekizu/inflater.hpp>
#include <ekizu/intents.hpp>
#include <ekizu/mutex.hpp>
#include <ekizu/timer_queue.hpp>
#include <net/ws.hpp>

namespace ekizu
{
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

inline const ShardId ShardId::ONE{ 0, 1 };

/**
 * @brief Represents a Discord shard, which is a connection to the Discord
 * gateway.
 */
struct Shard {
	Shard(ShardId id, std::string_view token, Intents intents);

	static const net::WebSocketCloseCode DISCORD_CLOSE_CODE =
		static_cast<net::WebSocketCloseCode>(4000);
	static const net::WebSocketCloseCode DISCORD_SESSION_EXPIRED_CODE =
		static_cast<net::WebSocketCloseCode>(4009);

	[[nodiscard]] uint64_t id() const
	{
		return m_id;
	}

	[[nodiscard]] Result<void> run(const std::function<void(Event)> &cb);
	[[nodiscard]] Result<Event> next_event();

    private:
	static TimerQueue &get_timer_queue();

	Result<void> set_auto_reconnect(bool auto_reconnect);

	struct Config {
		std::string token;
		std::optional<Intents> intents;
		uint64_t shard_count{ 1 };
		bool compression{ true };
		bool is_bot{ true };
	};

	void handle_event(tcb::span<const std::byte> data,
			  const std::function<void(Event)> &cb);
	void handle_dispatch(const nlohmann::json &data,
			     const std::function<void(Event)> &cb);
	void handle_reconnect();
	void handle_invalid_session(const nlohmann::json &data);
	void handle_hello(const nlohmann::json &data);
	void handle_heartbeat_ack();

	void reset_session()
	{
		m_sequence->store(0);
		auto lk = m_session_id->lock();
		static_cast<std::string &>(lk).clear();
	}

	[[nodiscard]] Result<void>
	connect(const std::function<void(tcb::span<const std::byte>)>
			&on_payload);

	[[nodiscard]] Result<void> reconnect(net::WebSocketCloseCode code,
					     bool reset);

	[[nodiscard]] Result<void> start_heartbeat(TimerQueue &timer_queue,
						   uint32_t heartbeat_interval);

	[[nodiscard]] Result<void> send_heartbeat();

	[[nodiscard]] Result<void> send_identify();

	[[nodiscard]] Result<void> disconnect(net::WebSocketCloseCode code);

	size_t m_heartbeat_id{};
	uint64_t m_id;
	Config m_config;
	std::unique_ptr<std::atomic_bool> m_last_heartbeat_acked{
		std::make_unique<std::atomic_bool>(false)
	};
	std::unique_ptr<Mutex<std::string> > m_session_id{
		std::make_unique<Mutex<std::string> >()
	};
	std::unique_ptr<std::atomic_uint64_t> m_sequence{
		std::make_unique<std::atomic_uint64_t>()
	};

	enum class ConnectionState : uint8_t {
		Disconnected,
		Connecting,
		Connected,
	};

	std::unique_ptr<std::atomic<ConnectionState> > m_state{
		std::make_unique<std::atomic<ConnectionState> >(
			ConnectionState::Disconnected)
	};

	/// May or may not be used based on runtime options.
	std::unique_ptr<Inflater> m_inflater;
	std::function<void(Event)> m_on_event;
	std::optional<net::WebSocketClient> m_websocket;
};
} // namespace ekizu

#endif // EKIZU_SHARD_HPP
