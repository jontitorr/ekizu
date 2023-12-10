#include <ekizu/gateway/channel_pins_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const ChannelPinsUpdate &u) {
	serialize(j, "guild_id", u.guild_id);
	serialize(j, "channel_id", u.channel_id);
	serialize(j, "last_pin_timestamp", u.last_pin_timestamp);
}

void from_json(const nlohmann::json &j, ChannelPinsUpdate &u) {
	deserialize(j, "guild_id", u.guild_id);
	deserialize(j, "channel_id", u.channel_id);
	deserialize(j, "last_pin_timestamp", u.last_pin_timestamp);
}
}  // namespace ekizu