#include <ekizu/gateway/message_reaction_add.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const MessageReactionAdd &e) {
	serialize(j, "user_id", e.user_id);
	serialize(j, "channel_id", e.channel_id);
	serialize(j, "message_id", e.message_id);
	serialize(j, "guild_id", e.guild_id);
	serialize(j, "member", e.member);
	serialize(j, "emoji", e.emoji);
	serialize(j, "message_author_id", e.message_author_id);
}

void from_json(const nlohmann::json &j, MessageReactionAdd &e) {
	deserialize(j, "user_id", e.user_id);
	deserialize(j, "channel_id", e.channel_id);
	deserialize(j, "message_id", e.message_id);
	deserialize(j, "guild_id", e.guild_id);
	deserialize(j, "member", e.member);
	deserialize(j, "emoji", e.emoji);
	deserialize(j, "message_author_id", e.message_author_id);
}
}  // namespace ekizu
