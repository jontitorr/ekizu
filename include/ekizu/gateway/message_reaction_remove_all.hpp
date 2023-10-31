#ifndef EKIZU_GATEWAY_MESSAGE_REACTION_REMOVE_ALL_HPP
#define EKIZU_GATEWAY_MESSAGE_REACTION_REMOVE_ALL_HPP

#include <ekizu/snowflake.hpp>
#include <optional>

namespace ekizu
{
struct MessageReactionRemoveAll {
	/// ID of the channel
	Snowflake channel_id;
	/// ID of the message
	Snowflake message_id;
	/// ID of the guild
	std::optional<Snowflake> guild_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const MessageReactionRemoveAll &m);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
			    MessageReactionRemoveAll &m);
} // namespace ekizu

#endif // EKIZU_GATEWAY_MESSAGE_REACTION_REMOVE_ALL_HPP
