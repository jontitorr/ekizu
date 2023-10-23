#include <ekizu/ban.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const Ban &b)
{
	serialize(j, "reason", b.reason);
	serialize(j, "user", b.user);
}

void from_json(const nlohmann::json &j, Ban &b)
{
	deserialize(j, "reason", b.reason);
	deserialize(j, "user", b.user);
}
} // namespace ekizu
