#include <ekizu/gateway/channel_delete.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
void to_json(nlohmann::json &j, const ChannelDelete &payload)
{
	j = payload.channel;
}

void from_json(const nlohmann::json &j, ChannelDelete &payload)
{
	json_util::detail::deserialize_impl(j, payload.channel);
}
} // namespace ekizu
