#ifndef EKIZU_GATEWAY_CHANNEL_PINS_UPDATE_HPP
#define EKIZU_GATEWAY_CHANNEL_PINS_UPDATE_HPP

#include <ekizu/snowflake.hpp>
#include <optional>

namespace ekizu
{
struct ChannelPinsUpdate {
	/// The ID of the guild.
	std::optional<Snowflake> guild_id;
	/// The ID of the channel.
	Snowflake channel_id;
	/// Time at which the most recent pinned message was pinned
	std::optional<std::string> last_pin_timestamp;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ChannelPinsUpdate &u);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ChannelPinsUpdate &u);
} // namespace ekizu

#endif // EKIZU_GATEWAY_CHANNEL_PINS_UPDATE_HPP
