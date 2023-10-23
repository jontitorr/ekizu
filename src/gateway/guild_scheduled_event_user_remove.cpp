#include <ekizu/gateway/guild_scheduled_event_user_remove.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildScheduledEventUserRemove &e)
{
	serialize(j, "guild_id", e.guild_id);
	serialize(j, "guild_scheduled_event_id", e.guild_scheduled_event_id);
	serialize(j, "user_id", e.user_id);
}

void from_json(const nlohmann::json &j, GuildScheduledEventUserRemove &e)
{
	deserialize(j, "guild_id", e.guild_id);
	deserialize(j, "guild_scheduled_event_id", e.guild_scheduled_event_id);
	deserialize(j, "user_id", e.user_id);
}
} // namespace ekizu
