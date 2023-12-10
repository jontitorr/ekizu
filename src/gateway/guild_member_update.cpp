#include <ekizu/gateway/guild_member_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildMemberUpdate &m) {
	serialize(j, "guild_id", m.guild_id);
	serialize(j, "roles", m.roles);
	serialize(j, "user", m.user);
	serialize(j, "nick", m.nick);
	serialize(j, "avatar", m.avatar);
	serialize(j, "joined_at", m.joined_at);
	serialize(j, "premium_since", m.premium_since);
	serialize(j, "deaf", m.deaf);
	serialize(j, "mute", m.mute);
	serialize(j, "pending", m.pending);
	serialize(
		j, "communication_disabled_until", m.communication_disabled_until);
}

void from_json(const nlohmann::json &j, GuildMemberUpdate &m) {
	deserialize(j, "guild_id", m.guild_id);
	deserialize(j, "roles", m.roles);
	deserialize(j, "user", m.user);
	deserialize(j, "nick", m.nick);
	deserialize(j, "avatar", m.avatar);
	deserialize(j, "joined_at", m.joined_at);
	deserialize(j, "premium_since", m.premium_since);
	deserialize(j, "deaf", m.deaf);
	deserialize(j, "mute", m.mute);
	deserialize(j, "pending", m.pending);
	deserialize(
		j, "communication_disabled_until", m.communication_disabled_until);
}
}  // namespace ekizu
