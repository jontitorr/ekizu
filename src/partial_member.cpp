#include <ekizu/json_util.hpp>
#include <ekizu/partial_member.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const PartialMember &m) {
	serialize(j, "user", m.user);
	serialize(j, "nick", m.nick);
	serialize(j, "avatar", m.avatar);
	serialize(j, "roles", m.roles);
	serialize(j, "joined_at", m.joined_at);
	serialize(j, "premium_since", m.premium_since);
	serialize(
		j, "communication_disabled_until", m.communication_disabled_until);
	serialize(j, "deaf", m.deaf);
	serialize(j, "mute", m.mute);
	serialize(j, "pending", m.pending);
	serialize(j, "permissions", m.permissions);
	serialize(
		j, "communication_disabled_until", m.communication_disabled_until);
}

void from_json(const nlohmann::json &j, PartialMember &m) {
	deserialize(j, "user", m.user);
	deserialize(j, "nick", m.nick);
	deserialize(j, "avatar", m.avatar);
	deserialize(j, "roles", m.roles);
	deserialize(j, "joined_at", m.joined_at);
	deserialize(j, "premium_since", m.premium_since);
	deserialize(
		j, "communication_disabled_until", m.communication_disabled_until);
	deserialize(j, "deaf", m.deaf);
	deserialize(j, "mute", m.mute);
	deserialize(j, "pending", m.pending);
	deserialize(j, "permissions", m.permissions);
	deserialize(
		j, "communication_disabled_until", m.communication_disabled_until);
}
}  // namespace ekizu
