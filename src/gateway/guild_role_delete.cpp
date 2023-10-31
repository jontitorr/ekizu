#include <ekizu/gateway/guild_role_delete.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildRoleDelete &e)
{
	serialize(j, "guild_id", e.guild_id);
	serialize(j, "role_id", e.role_id);
}

void from_json(const nlohmann::json &j, GuildRoleDelete &e)
{
	deserialize(j, "guild_id", e.guild_id);
	deserialize(j, "role_id", e.role_id);
}
} // namespace ekizu
