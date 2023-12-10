#include <ekizu/gateway/user_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
void to_json(nlohmann::json &j, const UserUpdate &u) { j = u.user; }

void from_json(const nlohmann::json &j, UserUpdate &u) {
	json_util::detail::deserialize_impl(j, u.user);
}
}  // namespace ekizu
