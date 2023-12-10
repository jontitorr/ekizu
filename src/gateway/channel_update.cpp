#include <ekizu/gateway/channel_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
void to_json(nlohmann::json &j, const ChannelUpdate &payload) {
	j = payload.channel;
}

void from_json(const nlohmann::json &j, ChannelUpdate &payload) {
	json_util::detail::deserialize_impl(j, payload.channel);
}
}  // namespace ekizu
