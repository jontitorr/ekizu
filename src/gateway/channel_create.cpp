#include <ekizu/gateway/channel_create.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
void to_json(nlohmann::json &j, const ChannelCreate &c) { j = c.channel; }

void from_json(const nlohmann::json &j, ChannelCreate &c) {
	json_util::detail::deserialize_impl(j, c.channel);
}
}  // namespace ekizu
