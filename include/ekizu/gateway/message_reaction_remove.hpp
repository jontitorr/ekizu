#ifndef EKIZU_GATEWAY_MESSAGE_REACTION_REMOVE_HPP
#define EKIZU_GATEWAY_MESSAGE_REACTION_REMOVE_HPP

#include <ekizu/emoji.hpp>

namespace ekizu
{
struct MessageReactionRemove {
	/// ID of the user.
	Snowflake user_id;
	/// ID of the channel.
	Snowflake channel_id;
	/// ID of the message.
	Snowflake message_id;
	/// ID of the guild (if applicable).
	std::optional<Snowflake> guild_id;
	/// Emoji used to react.
	PartialEmoji emoji;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const MessageReactionRemove &e);
EKIZU_EXPORT void from_json(const nlohmann::json &j, MessageReactionRemove &e);
} // namespace ekizu

#endif // EKIZU_GATEWAY_MESSAGE_REACTION_REMOVE_HPP
