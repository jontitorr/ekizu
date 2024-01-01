#include <ekizu/guild_preview.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void from_json(const nlohmann::json &j, GuildPreview &g) {
	deserialize(j, "id", g.id);
	deserialize(j, "name", g.name);
	deserialize(j, "icon", g.icon);
	deserialize(j, "splash", g.splash);
	deserialize(j, "discovery_splash", g.discovery_splash);
	deserialize(j, "emojis", g.emojis);
	deserialize(j, "features", g.features);
	deserialize(j, "approximate_member_count", g.approximate_member_count);
	deserialize(j, "approximate_presence_count", g.approximate_presence_count);
	deserialize(j, "description", g.description);
	deserialize(j, "stickers", g.stickers);
}

void to_json(nlohmann::json &j, const GuildPreview &g) {
	serialize(j, "id", g.id);
	serialize(j, "name", g.name);
	serialize(j, "icon", g.icon);
	serialize(j, "splash", g.splash);
	serialize(j, "discovery_splash", g.discovery_splash);
	serialize(j, "emojis", g.emojis);
	serialize(j, "features", g.features);
	serialize(j, "approximate_member_count", g.approximate_member_count);
	serialize(j, "approximate_presence_count", g.approximate_presence_count);
	serialize(j, "description", g.description);
	serialize(j, "stickers", g.stickers);
}
}  // namespace ekizu
