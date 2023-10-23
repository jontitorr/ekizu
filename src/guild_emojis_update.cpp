#include <ekizu/gateway/guild_emojis_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildEmojisUpdate &e)
{
	serialize(j, "guild_id", e.guild_id);
	serialize(j, "emojis", e.emojis);
}

void from_json(const nlohmann::json &j, GuildEmojisUpdate &e)
{
	deserialize(j, "guild_id", e.guild_id);
	deserialize(j, "emojis", e.emojis);
}
} // namespace ekizu
