#include <ekizu/json_util.hpp>
#include <ekizu/role.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const RoleTags &t) {
	serialize(j, "bot_id", t.bot_id);
	serialize(j, "integration_id", t.integration_id);
	serialize(j, "premium_subscriber", t.premium_subscriber);
	serialize(j, "subscription_listing_id", t.subscription_listing_id);
	serialize(j, "available_for_purchase", t.available_for_purchase);
	serialize(j, "guild_connections", t.guild_connections);
}

void from_json(const nlohmann::json &j, RoleTags &t) {
	deserialize(j, "bot_id", t.bot_id);
	deserialize(j, "integration_id", t.integration_id);
	deserialize(j, "premium_subscriber", t.premium_subscriber);
	deserialize(j, "subscription_listing_id", t.subscription_listing_id);
	deserialize(j, "available_for_purchase", t.available_for_purchase);
	deserialize(j, "guild_connections", t.guild_connections);
}

void to_json(nlohmann::json &j, const Role &r) {
	serialize(j, "id", r.id);
	serialize(j, "name", r.name);
	serialize(j, "color", r.color);
	serialize(j, "hoist", r.hoist);
	serialize(j, "icon", r.icon);
	serialize(j, "unicode_emoji", r.unicode_emoji);
	serialize(j, "position", r.position);
	serialize(j, "permissions", r.permissions);
	serialize(j, "managed", r.managed);
	serialize(j, "mentionable", r.mentionable);
	serialize(j, "tags", r.tags);
	serialize(j, "flags", r.flags);
}

void from_json(const nlohmann::json &j, Role &r) {
	deserialize(j, "id", r.id);
	deserialize(j, "name", r.name);
	deserialize(j, "color", r.color);
	deserialize(j, "hoist", r.hoist);
	deserialize(j, "icon", r.icon);
	deserialize(j, "unicode_emoji", r.unicode_emoji);
	deserialize(j, "position", r.position);
	deserialize(j, "permissions", r.permissions);
	deserialize(j, "managed", r.managed);
	deserialize(j, "mentionable", r.mentionable);
	deserialize(j, "tags", r.tags);
	deserialize(j, "flags", r.flags);
}
}  // namespace ekizu
