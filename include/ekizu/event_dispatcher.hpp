#ifndef EKIZU_EVENT_DISPATCHER_HPP
#define EKIZU_EVENT_DISPATCHER_HPP

#include <ekizu/event_handler.hpp>
#include <ekizu/thread_pool.hpp>
#include <optional>
#include <unordered_map>

namespace ekizu
{
struct EventDispatcherAttorney;
struct ShardManager;

/**
 * @brief Dispatches events to their respective handlers.
 */
struct EventDispatcher {
	struct CreateKey {
	    private:
		friend ShardManager;
		CreateKey() = default;
	};

	explicit EventDispatcher(CreateKey);

    private:
	friend EventDispatcherAttorney;

	void dispatch(GatewayEvent event, Client &client,
		      const nlohmann::json &data);

	std::unordered_map<GatewayEvent, std::shared_ptr<EventHandlerBase> >
		m_handlers;
	ThreadPool m_thread_pool;
};

struct EventDispatcherAttorney {
    private:
	friend Client;
	friend ShardManager;

	static void dispatch(EventDispatcher &dispatcher, GatewayEvent event,
			     Client &client, const nlohmann::json &data)
	{
		dispatcher.dispatch(event, client, data);
	}
};
} // namespace ekizu

#endif // EKIZU_EVENT_DISPATCHER_HPP
