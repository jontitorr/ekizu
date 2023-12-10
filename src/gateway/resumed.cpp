#include <ekizu/gateway/resumed.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
void to_json(nlohmann::json &j, [[maybe_unused]] const Resumed &r) {
	j = "resumed";
}
}  // namespace ekizu
