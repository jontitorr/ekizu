#ifndef EKIZU_EVENT_MANAGER_HPP
#define EKIZU_EVENT_MANAGER_HPP

#include <ekizu/event_emitter.hpp>
#include <ekizu/event_payload.hpp>

namespace ekizu
{
struct Client;
struct EventManagerAttorney;

/**
 * @brief Manages the events of the client. If a user wants to listen to an
 * event, they must do so through this class.
 */
struct EventManager {
	struct CreateKey {
	    private:
		friend Client;
		CreateKey() = default;
	};

	explicit EventManager(CreateKey /*unused*/)
	{
	}

    private:
	friend EventManagerAttorney;

	template <GatewayEvent Event, typename Callback>
	void add_listener(Callback &&callback)
	{
		if constexpr (Event == GatewayEvent::Log) {
			m_log.add_listener(std::forward<Callback>(callback));
		} else if constexpr (Event == GatewayEvent::Ready) {
			m_ready.add_listener(std::forward<Callback>(callback));
		} else if constexpr (Event == GatewayEvent::MessageCreate) {
			m_message_create.add_listener(
				std::forward<Callback>(callback));
		} else {
			static_assert(sizeof(Callback) == 0,
				      "Unsupported event");
		}
	}

	template <GatewayEvent Event, typename... Args>
	void emit(Args &&...args)
	{
		if constexpr (Event == GatewayEvent::Log) {
			m_log.emit(std::forward<Args>(args)...);
		} else if constexpr (Event == GatewayEvent::Ready) {
			m_ready.emit(std::forward<Args>(args)...);
		} else if constexpr (Event == GatewayEvent::MessageCreate) {
			m_message_create.emit(std::forward<Args>(args)...);
		} else {
			static_assert(sizeof...(Args) == 0,
				      "Unsupported event");
		}
	}

	// TODO: Populate this with all events
	EventEmitter<EventPayload<GatewayEvent::Log> > m_log;
	EventEmitter<EventPayload<GatewayEvent::Ready> > m_ready;
	EventEmitter<EventPayload<GatewayEvent::MessageCreate> >
		m_message_create;
};

struct EventManagerAttorney {
    private:
	friend Client;

	template <GatewayEvent Event, typename Callback>
	static void add_listener(EventManager &manager, Callback &&callback)
	{
		manager.add_listener<Event>(std::forward<Callback>(callback));
	}

	template <GatewayEvent Event, typename... Args>
	static void emit(EventManager &manager, Args &&...args)
	{
		manager.emit<Event>(std::forward<Args>(args)...);
	}
};
} // namespace ekizu

#endif // EKIZU_EVENT_MANAGER_HPP
