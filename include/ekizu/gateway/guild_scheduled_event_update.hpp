#ifndef EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_UPDATE_HPP
#define EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_UPDATE_HPP

#include <ekizu/guild_scheduled_event.hpp>

namespace ekizu {
struct GuildScheduledEventUpdate {
	GuildScheduledEvent event;
};

EKIZU_EXPORT void to_json(nlohmann::json &j,
						  const GuildScheduledEventUpdate &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
							GuildScheduledEventUpdate &c);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_UPDATE_HPP
