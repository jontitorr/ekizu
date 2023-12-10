#ifndef EKIZU_GATEWAY_CHANNEL_DELETE_HPP
#define EKIZU_GATEWAY_CHANNEL_DELETE_HPP

#include <ekizu/channel.hpp>

namespace ekizu {
struct ChannelDelete {
	Channel channel;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ChannelDelete &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ChannelDelete &c);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_CHANNEL_DELETE_HPP
