#include <ekizu/json_util.hpp>
#include <ekizu/message.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const AllowedMentions &m) {
	serialize(j, "parse", m.parse);
	serialize(j, "roles", m.roles);
	serialize(j, "users", m.users);
	serialize(j, "replied_user", m.replied_user);
}

void from_json(const nlohmann::json &j, AllowedMentions &m) {
	deserialize(j, "parse", m.parse);
	deserialize(j, "roles", m.roles);
	deserialize(j, "users", m.users);
	deserialize(j, "replied_user", m.replied_user);
}

void to_json(nlohmann::json &j, const ChannelMention &m) {
	serialize(j, "id", m.id);
	serialize(j, "guild_id", m.guild_id);
	serialize(j, "type", m.type);
	serialize(j, "name", m.name);
}

void from_json(const nlohmann::json &j, ChannelMention &m) {
	deserialize(j, "id", m.id);
	deserialize(j, "guild_id", m.guild_id);
	deserialize(j, "type", m.type);
	deserialize(j, "name", m.name);
}

void to_json(nlohmann::json &j, const MessageActivity &a) {
	serialize(j, "type", a.type);
	serialize(j, "party_id", a.party_id);
}

void from_json(const nlohmann::json &j, MessageActivity &a) {
	deserialize(j, "type", a.type);
	deserialize(j, "party_id", a.party_id);
}

void to_json(nlohmann::json &j, const MessageApplication &a) {
	serialize(j, "id", a.id);
	serialize(j, "name", a.name);
	serialize(j, "icon", a.icon);
	serialize(j, "description", a.description);
	serialize(j, "cover_image", a.cover_image);
}

void from_json(const nlohmann::json &j, MessageApplication &a) {
	deserialize(j, "id", a.id);
	deserialize(j, "name", a.name);
	deserialize(j, "icon", a.icon);
	deserialize(j, "description", a.description);
	deserialize(j, "cover_image", a.cover_image);
}

void to_json(nlohmann::json &j, const MessageInteraction &i) {
	serialize(j, "id", i.id);
	serialize(j, "type", i.type);
	serialize(j, "name", i.name);
	serialize(j, "user", i.user);
	serialize(j, "member", i.member);
}

void from_json(const nlohmann::json &j, MessageInteraction &i) {
	deserialize(j, "id", i.id);
	deserialize(j, "type", i.type);
	deserialize(j, "name", i.name);
	deserialize(j, "user", i.user);
	deserialize(j, "member", i.member);
}

void to_json(nlohmann::json &j, const MessageReference &r) {
	serialize(j, "message_id", r.message_id);
	serialize(j, "channel_id", r.channel_id);
	serialize(j, "guild_id", r.guild_id);
	serialize(j, "fail_if_not_exists", r.fail_if_not_exists);
}

void from_json(const nlohmann::json &j, MessageReference &r) {
	deserialize(j, "message_id", r.message_id);
	deserialize(j, "channel_id", r.channel_id);
	deserialize(j, "guild_id", r.guild_id);
	deserialize(j, "fail_if_not_exists", r.fail_if_not_exists);
}

void to_json(nlohmann::json &j, const Message &m) {
	serialize(j, "id", m.id);
	serialize(j, "channel_id", m.channel_id);
	serialize(j, "guild_id", m.guild_id);
	serialize(j, "author", m.author);
	serialize(j, "content", m.content);
	serialize(j, "timestamp", m.timestamp);
	serialize(j, "edited_timestamp", m.edited_timestamp);
	serialize(j, "tts", m.tts);
	serialize(j, "mention_everyone", m.mention_everyone);
	serialize(j, "mentions", m.mentions);
	serialize(j, "mention_roles", m.mention_roles);
	serialize(j, "mention_channels", m.mention_channels);
	serialize(j, "attachments", m.attachments);
	serialize(j, "embeds", m.embeds);
	serialize(j, "nonce", m.nonce);
	serialize(j, "pinned", m.pinned);
	serialize(j, "webhook_id", m.webhook_id);
	serialize(j, "type", m.type);
	serialize(j, "activity", m.activity);
	serialize(j, "application", m.application);
	serialize(j, "application_id", m.application_id);
	serialize(j, "message_reference", m.message_reference);
	serialize(j, "flags", m.flags);
	serialize(j, "referenced_message", m.referenced_message);
	serialize(j, "interaction", m.interaction);
	serialize(j, "components", m.components);
}

void from_json(const nlohmann::json &j, Message &m) {
	deserialize(j, "id", m.id);
	deserialize(j, "channel_id", m.channel_id);
	deserialize(j, "guild_id", m.guild_id);
	deserialize(j, "author", m.author);
	deserialize(j, "content", m.content);
	deserialize(j, "timestamp", m.timestamp);
	deserialize(j, "edited_timestamp", m.edited_timestamp);
	deserialize(j, "tts", m.tts);
	deserialize(j, "mention_everyone", m.mention_everyone);
	deserialize(j, "mentions", m.mentions);
	deserialize(j, "mention_roles", m.mention_roles);
	deserialize(j, "mention_channels", m.mention_channels);
	deserialize(j, "attachments", m.attachments);
	deserialize(j, "embeds", m.embeds);
	deserialize(j, "nonce", m.nonce);
	deserialize(j, "pinned", m.pinned);
	deserialize(j, "webhook_id", m.webhook_id);
	deserialize(j, "type", m.type);
	deserialize(j, "activity", m.activity);
	deserialize(j, "application", m.application);
	deserialize(j, "application_id", m.application_id);
	deserialize(j, "message_reference", m.message_reference);
	deserialize(j, "flags", m.flags);
	deserialize(j, "referenced_message", m.referenced_message);
	deserialize(j, "interaction", m.interaction);
	deserialize(j, "components", m.components);
}
}  // namespace ekizu
