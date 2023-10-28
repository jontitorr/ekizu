#ifndef EKIZU_GATEWAY_MESSAGE_DELETE_BULK_HPP
#define EKIZU_GATEWAY_MESSAGE_DELETE_BULK_HPP

#include <ekizu/snowflake.hpp>
#include <optional>

namespace ekizu
{
struct MessageDeleteBulk {
	/// IDs of the messages.
	std::vector<Snowflake> ids;
	/// ID of the channel.
	Snowflake channel_id;
	/// ID of the guild (optional).
	std::optional<Snowflake> guild_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const MessageDeleteBulk &m);
EKIZU_EXPORT void from_json(const nlohmann::json &j, MessageDeleteBulk &m);
} // namespace ekizu

#endif // EKIZU_GATEWAY_MESSAGE_DELETE_BULK_HPP
