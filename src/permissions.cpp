#include <ekizu/json_util.hpp>
#include <ekizu/permissions.hpp>

namespace ekizu {
void to_json(nlohmann::json &j, const Permissions &p) {
	j = std::to_string(static_cast<uint64_t>(p));
}

void from_json(const nlohmann::json &j, Permissions &p) {
	auto res = json_util::deserialize<uint64_t>(j);
	if (res) { p = static_cast<Permissions>(res.value()); }
}
}  // namespace ekizu
