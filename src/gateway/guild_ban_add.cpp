#include <ekizu/gateway/guild_ban_add.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildBanAdd &b) {
	serialize(j, "guild_id", b.guild_id);
	serialize(j, "user", b.user);
}

void from_json(const nlohmann::json &j, GuildBanAdd &b) {
	deserialize(j, "guild_id", b.guild_id);
	deserialize(j, "user", b.user);
}
}  // namespace ekizu