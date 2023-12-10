#include <ekizu/gateway/guild_member_add.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildMemberAdd &m) {
	serialize(j, "guild_id", m.guild_id);
	serialize(j, "member", m.member);
}

void from_json(const nlohmann::json &j, GuildMemberAdd &m) {
	deserialize(j, "guild_id", m.guild_id);
	deserialize(j, "member", m.member);
}
}  // namespace ekizu
