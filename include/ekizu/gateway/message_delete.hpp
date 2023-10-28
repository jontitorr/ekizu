#ifndef EKIZU_GATEWAY_MESSAGE_DELETE_HPP
#define EKIZU_GATEWAY_MESSAGE_DELETE_HPP

#include <ekizu/snowflake.hpp>
#include <optional>

namespace ekizu
{
struct MessageDelete {
	/// ID of the message.
	Snowflake id;
	/// ID of the channel.
	Snowflake channel_id;
	/// ID of the guild (optional).
	std::optional<Snowflake> guild_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const MessageDelete &m);
EKIZU_EXPORT void from_json(const nlohmann::json &j, MessageDelete &m);
} // namespace ekizu

#endif // EKIZU_GATEWAY_MESSAGE_DELETE_HPP
