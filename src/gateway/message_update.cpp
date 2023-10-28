#include <ekizu/gateway/message_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const MessageUpdate &m)
{
	serialize(j, "id", m.id);
	serialize(j, "channel_id", m.channel_id);
	serialize(j, "author", m.author);
	serialize(j, "content", m.content);
	serialize(j, "timestamp", m.timestamp);
	serialize(j, "edited_timestamp", m.edited_timestamp);
	serialize(j, "tts", m.tts);
	serialize(j, "mention_everyone", m.mention_everyone);
	serialize(j, "mentions", m.mentions);
	serialize(j, "mention_roles", m.mention_roles);
	serialize(j, "attachments", m.attachments);
	serialize(j, "embeds", m.embeds);
	serialize(j, "pinned", m.pinned);
	serialize(j, "type", m.type);
}

void from_json(const nlohmann::json &j, MessageUpdate &m)
{
	deserialize(j, "id", m.id);
	deserialize(j, "channel_id", m.channel_id);
	deserialize(j, "author", m.author);
	deserialize(j, "content", m.content);
	deserialize(j, "timestamp", m.timestamp);
	deserialize(j, "edited_timestamp", m.edited_timestamp);
	deserialize(j, "tts", m.tts);
	deserialize(j, "mention_everyone", m.mention_everyone);
	deserialize(j, "mentions", m.mentions);
	deserialize(j, "mention_roles", m.mention_roles);
	deserialize(j, "attachments", m.attachments);
	deserialize(j, "embeds", m.embeds);
	deserialize(j, "pinned", m.pinned);
	deserialize(j, "type", m.type);
}
} // namespace ekizu
