#ifndef EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_CREATE_HPP
#define EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_CREATE_HPP

#include <ekizu/guild_scheduled_event.hpp>

namespace ekizu {
struct GuildScheduledEventCreate {
	GuildScheduledEvent event;
};

EKIZU_EXPORT void to_json(nlohmann::json &j,
						  const GuildScheduledEventCreate &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
							GuildScheduledEventCreate &c);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_CREATE_HPP
