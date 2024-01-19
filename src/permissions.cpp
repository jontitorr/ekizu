#include <ekizu/json_util.hpp>
#include <ekizu/permissions.hpp>

namespace ekizu {
void to_json(nlohmann::json &j, const Permissions &p) {
	j = std::to_string(static_cast<uint64_t>(p));
}

void from_json(const nlohmann::json &j, Permissions &p) {
	json_util::detail::deserialize_impl(j, p);
}
}  // namespace ekizu
