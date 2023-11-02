#include <ekizu/gateway/message_reaction_remove.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const MessageReactionRemove &e)
{
	serialize(j, "user_id", e.user_id);
	serialize(j, "channel_id", e.channel_id);
	serialize(j, "message_id", e.message_id);
	serialize(j, "guild_id", e.guild_id);
	serialize(j, "emoji", e.emoji);
}

void from_json(const nlohmann::json &j, MessageReactionRemove &e)
{
	deserialize(j, "user_id", e.user_id);
	deserialize(j, "channel_id", e.channel_id);
	deserialize(j, "message_id", e.message_id);
	deserialize(j, "guild_id", e.guild_id);
	deserialize(j, "emoji", e.emoji);
}
} // namespace ekizu
