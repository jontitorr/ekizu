#include <ekizu/gateway/thread_members_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const ThreadMembersUpdate &t) {
	serialize(j, "id", t.id);
	serialize(j, "guild_id", t.guild_id);
	serialize(j, "member_count", t.member_count);
	serialize(j, "added_members", t.added_members);
	serialize(j, "removed_member_ids", t.removed_member_ids);
}

void from_json(const nlohmann::json &j, ThreadMembersUpdate &t) {
	deserialize(j, "id", t.id);
	deserialize(j, "guild_id", t.guild_id);
	deserialize(j, "member_count", t.member_count);
	deserialize(j, "added_members", t.added_members);
	deserialize(j, "removed_member_ids", t.removed_member_ids);
}
}  // namespace ekizu
