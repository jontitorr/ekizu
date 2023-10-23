#include <ekizu/gateway/guild_integrations_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildIntegrationsUpdate &i)
{
	serialize(j, "guild_id", i.guild_id);
}

void from_json(const nlohmann::json &j, GuildIntegrationsUpdate &i)
{
	deserialize(j, "guild_id", i.guild_id);
}
} // namespace ekizu
