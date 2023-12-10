#include <ekizu/gateway/thread_delete.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const ThreadDelete &t) {
	serialize(j, "id", t.id);
	serialize(j, "guild_id", t.guild_id);
	serialize(j, "parent_id", t.parent_id);
	serialize(j, "type", t.type);
}

void from_json(const nlohmann::json &j, ThreadDelete &t) {
	deserialize(j, "id", t.id);
	deserialize(j, "guild_id", t.guild_id);
	deserialize(j, "parent_id", t.parent_id);
	deserialize(j, "type", t.type);
}
}  // namespace ekizu
