#include <ekizu/gateway/presence_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
void to_json(nlohmann::json &j, const PresenceUpdate &payload) {
	j = payload.presence;
}

void from_json(const nlohmann::json &j, PresenceUpdate &payload) {
	json_util::detail::deserialize_impl(j, payload.presence);
}
}  // namespace ekizu
