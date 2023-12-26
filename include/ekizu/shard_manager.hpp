#ifndef EKIZU_SHARD_MANAGER_HPP
#define EKIZU_SHARD_MANAGER_HPP

#include <ekizu/client_options.hpp>
#include <ekizu/event_dispatcher.hpp>
#include <ekizu/shard.hpp>
#include <ekizu/timer_queue.hpp>

namespace ekizu {
struct DiscordApi;
struct EventManager;
struct ShardManagerAttorney;

/**
 * @brief Manages the connections or shards to the Discord gateway. This will
 * route payloads to their respective handlers using the event dispatcher, and
 * will also provide the heartbeats responsible for keeping the connection
 * alive.
 */
struct ShardManager {
	struct CreateKey {
	   private:
		friend Client;
		CreateKey() = default;
	};

	explicit ShardManager(
		CreateKey, std::function<void(LogLevel, std::string_view)> log_fn,
		std::function<void(EventDispatcher &, GatewayEvent event,
						   const nlohmann::json &data)>
			dispatch_fn);

	ShardManager(const ShardManager &) = delete;
	ShardManager &operator=(const ShardManager &) = delete;
	ShardManager(ShardManager &&) = delete;
	ShardManager &operator=(ShardManager &&) = delete;

	EKIZU_EXPORT ~ShardManager();

   private:
	friend ShardManagerAttorney;

	[[nodiscard]] Result<> connect(
		FunctionView<Result<net::HttpResponse>()> get_gateway,
		const ClientOptions &options, std::string_view token);

	[[nodiscard]] Result<> handle_payload(boost::span<const std::byte> payload,
										  Shard &shard);
	[[nodiscard]] Result<> handle_dispatch(const nlohmann::json &json,
										   Shard &shard);

	std::function<void(LogLevel, std::string_view)> m_log_fn;
	std::function<void(
		EventDispatcher &, GatewayEvent event, const nlohmann::json &data)>
		m_dispatch_fn;
	std::string m_gateway;
	EventDispatcher m_dispatcher;
	std::vector<Shard> m_shards;
	std::vector<std::thread> m_shard_threads;
	TimerQueue m_heartbeat_queue;
};

struct ShardManagerAttorney {
   private:
	friend Client;

	static Result<> connect(
		ShardManager &shard_manager,
		FunctionView<Result<net::HttpResponse>()> get_gateway,
		const ClientOptions &options, std::string_view token) {
		return shard_manager.connect(get_gateway, options, token);
	}
};
}  // namespace ekizu

#endif	// EKIZU_SHARD_MANAGER_HPP
