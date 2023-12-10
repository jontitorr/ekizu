#include <ekizu/gateway/interaction_create.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
void to_json(nlohmann::json &j, const InteractionCreate &c) {
	j = c.interaction;
}

void from_json(const nlohmann::json &j, InteractionCreate &c) {
	json_util::detail::deserialize_impl(j, c.interaction);
}
}  // namespace ekizu
