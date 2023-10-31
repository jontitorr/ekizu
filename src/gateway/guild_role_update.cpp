#include <ekizu/gateway/guild_role_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildRoleUpdate &e)
{
	serialize(j, "guild_id", e.guild_id);
	serialize(j, "role", e.role);
}

void from_json(const nlohmann::json &j, GuildRoleUpdate &e)
{
	deserialize(j, "guild_id", e.guild_id);
	deserialize(j, "role", e.role);
}
} // namespace ekizu
