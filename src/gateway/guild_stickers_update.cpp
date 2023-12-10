#include <ekizu/gateway/guild_stickers_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildStickersUpdate &u) {
	serialize(j, "guild_id", u.guild_id);
	serialize(j, "stickers", u.stickers);
}

void from_json(const nlohmann::json &j, GuildStickersUpdate &u) {
	deserialize(j, "guild_id", u.guild_id);
	deserialize(j, "stickers", u.stickers);
}
}  // namespace ekizu
