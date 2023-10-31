#include <ekizu/gateway/message_reaction_remove_emoji.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const MessageReactionRemoveEmoji &r)
{
	serialize(j, "channel_id", r.channel_id);
	serialize(j, "guild_id", r.guild_id);
	serialize(j, "message_id", r.message_id);
	serialize(j, "emoji", r.emoji);
}

void from_json(const nlohmann::json &j, MessageReactionRemoveEmoji &r)
{
	deserialize(j, "channel_id", r.channel_id);
	deserialize(j, "guild_id", r.guild_id);
	deserialize(j, "message_id", r.message_id);
	deserialize(j, "emoji", r.emoji);
}
} // namespace ekizu
