#include <ekizu/json_util.hpp>
#include <ekizu/log.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const Log &l) {
	serialize(j, "level", l.level);
	serialize(j, "message", l.message);
}

void from_json(const nlohmann::json &j, Log &l) {
	deserialize(j, "level", l.level);
	deserialize(j, "message", l.message);
}
}  // namespace ekizu
