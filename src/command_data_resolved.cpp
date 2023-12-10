#include <ekizu/command_data_resolved.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const InteractionChannel &c) {
	serialize(j, "id", c.id);
	serialize(j, "type", c.type);
	serialize(j, "name", c.name);
	serialize(j, "parent_id", c.parent_id);
	serialize(j, "permissions", c.permissions);
	serialize(j, "thread_metadata", c.thread_metadata);
}

void from_json(const nlohmann::json &j, InteractionChannel &c) {
	deserialize(j, "id", c.id);
	deserialize(j, "type", c.type);
	deserialize(j, "name", c.name);
	deserialize(j, "parent_id", c.parent_id);
	deserialize(j, "permissions", c.permissions);
	deserialize(j, "thread_metadata", c.thread_metadata);
}

void to_json(nlohmann::json &j, const InteractionMember &m) {
	serialize(j, "avatar", m.avatar);
	serialize(
		j, "communication_disabled_until", m.communication_disabled_until);
	serialize(j, "flags", m.flags);
	serialize(j, "joined_at", m.joined_at);
	serialize(j, "nick", m.nick);
	serialize(j, "pending", m.pending);
	serialize(j, "permissions", m.permissions);
	serialize(j, "premium_since", m.premium_since);
	serialize(j, "roles", m.roles);
}

void from_json(const nlohmann::json &j, InteractionMember &m) {
	deserialize(j, "avatar", m.avatar);
	deserialize(
		j, "communication_disabled_until", m.communication_disabled_until);
	deserialize(j, "flags", m.flags);
	deserialize(j, "joined_at", m.joined_at);
	deserialize(j, "nick", m.nick);
	deserialize(j, "pending", m.pending);
	deserialize(j, "permissions", m.permissions);
	deserialize(j, "premium_since", m.premium_since);
	deserialize(j, "roles", m.roles);
}

void to_json(nlohmann::json &j, const CommandInteractionDataResolved &r) {
	serialize(j, "attachments", r.attachments);
	serialize(j, "channels", r.channels);
	serialize(j, "members", r.members);
	serialize(j, "messages", r.messages);
	serialize(j, "roles", r.roles);
	serialize(j, "users", r.users);
}

void from_json(const nlohmann::json &j, CommandInteractionDataResolved &r) {
	deserialize(j, "attachments", r.attachments);
	deserialize(j, "channels", r.channels);
	deserialize(j, "members", r.members);
	deserialize(j, "messages", r.messages);
	deserialize(j, "roles", r.roles);
	deserialize(j, "users", r.users);
}
}  // namespace ekizu
