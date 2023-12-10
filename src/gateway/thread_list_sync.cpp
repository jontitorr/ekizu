#include <ekizu/gateway/thread_list_sync.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const ThreadListSync &t) {
	serialize(j, "guild_id", t.guild_id);
	serialize(j, "channel_ids", t.channel_ids);
	serialize(j, "threads", t.threads);
	serialize(j, "members", t.members);
}

void from_json(const nlohmann::json &j, ThreadListSync &t) {
	deserialize(j, "guild_id", t.guild_id);
	deserialize(j, "channel_ids", t.channel_ids);
	deserialize(j, "threads", t.threads);
	deserialize(j, "members", t.members);
}
}  // namespace ekizu
