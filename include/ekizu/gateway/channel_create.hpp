#ifndef EKIZU_GATEWAY_CHANNEL_CREATE_HPP
#define EKIZU_GATEWAY_CHANNEL_CREATE_HPP

#include <ekizu/channel.hpp>

namespace ekizu
{
struct ChannelCreate {
	Channel channel;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ChannelCreate &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ChannelCreate &c);
} // namespace ekizu

#endif // EKIZU_GATEWAY_CHANNEL_CREATE_HPP
