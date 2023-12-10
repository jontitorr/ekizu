#include <ekizu/json_util.hpp>
#include <ekizu/partial_application.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const PartialApplication &a) {
	serialize(j, "id", a.id);
	serialize(j, "flags", a.flags);
}

void from_json(const nlohmann::json &j, PartialApplication &a) {
	deserialize(j, "id", a.id);
	deserialize(j, "flags", a.flags);
}
}  // namespace ekizu
