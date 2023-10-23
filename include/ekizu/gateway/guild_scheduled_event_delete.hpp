#ifndef EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_DELETE_HPP
#define EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_DELETE_HPP

#include <ekizu/guild_scheduled_event.hpp>

namespace ekizu
{
struct GuildScheduledEventDelete {
	GuildScheduledEvent event;
};

EKIZU_EXPORT void to_json(nlohmann::json &j,
			  const GuildScheduledEventDelete &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
			    GuildScheduledEventDelete &c);
} // namespace ekizu

#endif // EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_DELETE_HPP
