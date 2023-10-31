#include <ekizu/gateway/guild_role_create.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildRoleCreate &r)
{
	serialize(j, "guild_id", r.guild_id);
	serialize(j, "role", r.role);
}

void from_json(const nlohmann::json &j, GuildRoleCreate &r)
{
	deserialize(j, "guild_id", r.guild_id);
	deserialize(j, "role", r.role);
}
} // namespace ekizu
