#ifndef EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_USER_REMOVE_HPP
#define EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_USER_REMOVE_HPP

#include <ekizu/snowflake.hpp>

namespace ekizu {
struct GuildScheduledEventUserRemove {
	/// Guild ID of the scheduled event.
	Snowflake guild_id;
	/// ID of the guild scheduled event.
	Snowflake guild_scheduled_event_id;
	/// ID of the user who has unsubscribed from the guild scheduled event.
	Snowflake user_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j,
						  const GuildScheduledEventUserRemove &e);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
							GuildScheduledEventUserRemove &e);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_USER_REMOVE_HPP
