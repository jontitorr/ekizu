#ifndef EKIZU_GATEWAY_MESSAGE_REACTION_ADD_HPP
#define EKIZU_GATEWAY_MESSAGE_REACTION_ADD_HPP

#include <ekizu/emoji.hpp>
#include <ekizu/guild_member.hpp>

namespace ekizu {
struct MessageReactionAdd {
	/// ID of the user.
	Snowflake user_id;
	/// ID of the channel.
	Snowflake channel_id;
	/// ID of the message.
	Snowflake message_id;
	/// ID of the guild (if applicable).
	std::optional<Snowflake> guild_id;
	/// Member who reacted (if it happened in a guild).
	std::optional<GuildMember> member;
	/// Emoji used to react.
	PartialEmoji emoji;
	/// ID of the user who authored the reacted message.
	Snowflake message_author_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const MessageReactionAdd &e);
EKIZU_EXPORT void from_json(const nlohmann::json &j, MessageReactionAdd &e);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_MESSAGE_REACTION_ADD_HPP
