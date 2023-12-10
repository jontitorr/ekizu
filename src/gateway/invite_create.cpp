#include <ekizu/gateway/invite_create.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const PartialUser &u) {
	serialize(j, "id", u.id);
	serialize(j, "username", u.username);
	serialize(j, "discriminator", u.discriminator);
	serialize(j, "avatar", u.avatar);
}

void from_json(const nlohmann::json &j, PartialUser &u) {
	deserialize(j, "id", u.id);
	deserialize(j, "username", u.username);
	deserialize(j, "discriminator", u.discriminator);
	deserialize(j, "avatar", u.avatar);
}

void to_json(nlohmann::json &j, const InviteCreate &i) {
	serialize(j, "channel_id", i.channel_id);
	serialize(j, "code", i.code);
	serialize(j, "created_at", i.created_at);
	serialize(j, "guild_id", i.guild_id);
	serialize(j, "inviter", i.inviter);
	serialize(j, "max_age", i.max_age);
	serialize(j, "max_uses", i.max_uses);
	serialize(j, "target_user_type", i.target_user_type);
	serialize(j, "target_user", i.target_user);
	serialize(j, "temporary", i.temporary);
	serialize(j, "uses", i.uses);
}

void from_json(const nlohmann::json &j, InviteCreate &i) {
	deserialize(j, "channel_id", i.channel_id);
	deserialize(j, "code", i.code);
	deserialize(j, "created_at", i.created_at);
	deserialize(j, "guild_id", i.guild_id);
	deserialize(j, "inviter", i.inviter);
	deserialize(j, "max_age", i.max_age);
	deserialize(j, "max_uses", i.max_uses);
	deserialize(j, "target_user_type", i.target_user_type);
	deserialize(j, "target_user", i.target_user);
	deserialize(j, "temporary", i.temporary);
	deserialize(j, "uses", i.uses);
}
}  // namespace ekizu
