#include <ekizu/gateway/guild_delete.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildDelete &g)
{
	serialize(j, "guild_id", g.guild_id);
	serialize(j, "unavailable", g.unavailable);
}

void from_json(const nlohmann::json &j, GuildDelete &g)
{
	deserialize(j, "guild_id", g.guild_id);
	deserialize(j, "unavailable", g.unavailable);
}
} // namespace ekizu
