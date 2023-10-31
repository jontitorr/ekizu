#include <ekizu/gateway/message_reaction_remove_all.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const MessageReactionRemoveAll &m)
{
	serialize(j, "channel_id", m.channel_id);
	serialize(j, "message_id", m.message_id);
	serialize(j, "guild_id", m.guild_id);
}

void from_json(const nlohmann::json &j, MessageReactionRemoveAll &m)
{
	deserialize(j, "channel_id", m.channel_id);
	deserialize(j, "message_id", m.message_id);
	deserialize(j, "guild_id", m.guild_id);
}
} // namespace ekizu
