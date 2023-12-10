#ifndef EKIZU_GATEWAY_CHANNEL_UPDATE_HPP
#define EKIZU_GATEWAY_CHANNEL_UPDATE_HPP

#include <ekizu/channel.hpp>

namespace ekizu {
struct ChannelUpdate {
	Channel channel;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ChannelUpdate &payload);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ChannelUpdate &payload);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_CHANNEL_UPDATE_HPP
