#ifndef EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_USER_ADD_HPP
#define EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_USER_ADD_HPP

#include <ekizu/snowflake.hpp>

namespace ekizu
{
struct GuildScheduledEventUserAdd {
	/// Guild ID of the scheduled event.
	Snowflake guild_id;
	/// ID of the guild scheduled event.
	Snowflake guild_scheduled_event_id;
	/// ID of the user who has subscribed to the guild scheduled event.
	Snowflake user_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j,
			  const GuildScheduledEventUserAdd &e);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
			    GuildScheduledEventUserAdd &e);
} // namespace ekizu

#endif // EKIZU_GATEWAY_GUILD_SCHEDULED_EVENT_USER_ADD_HPP
