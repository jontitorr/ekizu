#ifndef EKIZU_EVENT_HANDLER_HPP
#define EKIZU_EVENT_HANDLER_HPP

#include <ekizu/export.h>
#include <ekizu/gateway_event.hpp>
#include <nlohmann/json_fwd.hpp>

namespace ekizu
{
struct Client;

struct EventHandlerBase {
	EventHandlerBase() = default;
	EventHandlerBase(const EventHandlerBase &) = delete;
	EventHandlerBase &operator=(const EventHandlerBase &) = delete;
	EventHandlerBase(EventHandlerBase &&) = default;
	EventHandlerBase &operator=(EventHandlerBase &&) = delete;
	virtual ~EventHandlerBase() = default;

	virtual void handle(Client &client,
			    const nlohmann::json &data) const = 0;
};

/**
 * @brief Handler for a specific gateway event.
 *
 * @tparam E The gateway event to handle.
 */
template <GatewayEvent E> struct EventHandler : EventHandlerBase {
	/**
     * @brief Handles the gateway event.
     *
     * @param client The client that received the event.
     * @param data The data of the event.
     */
	EKIZU_EXPORT void handle(Client &client,
				 const nlohmann::json &data) const override;
};
} // namespace ekizu

#endif // EKIZU_EVENT_HANDLER_HPP
