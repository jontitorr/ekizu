#ifndef EKIZU_GATEWAY_MESSAGE_REACTION_REMOVE_EMOJI_HPP
#define EKIZU_GATEWAY_MESSAGE_REACTION_REMOVE_EMOJI_HPP

#include <ekizu/emoji.hpp>

namespace ekizu {
/**
 * @brief Represents an event that occurs when a bot removes all instances of a
 * given emoji from the reactions of a message.
 */
struct MessageReactionRemoveEmoji {
	/// ID of the channel.
	Snowflake channel_id;
	/// ID of the guild (optional).
	std::optional<Snowflake> guild_id;
	/// ID of the message.
	Snowflake message_id;
	/// Emoji that was removed.
	Emoji emoji;
};

EKIZU_EXPORT void to_json(nlohmann::json &j,
						  const MessageReactionRemoveEmoji &r);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
							MessageReactionRemoveEmoji &r);

}  // namespace ekizu

#endif	// EKIZU_GATEWAY_MESSAGE_REACTION_REMOVE_EMOJI_HPP
