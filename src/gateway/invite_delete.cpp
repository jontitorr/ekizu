#include <ekizu/gateway/invite_delete.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const InviteDelete &invite) {
	serialize(j, "channel_id", invite.channel_id);
	serialize(j, "code", invite.code);
	serialize(j, "guild_id", invite.guild_id);
}

void from_json(const nlohmann::json &j, InviteDelete &invite) {
	deserialize(j, "channel_id", invite.channel_id);
	deserialize(j, "code", invite.code);
	deserialize(j, "guild_id", invite.guild_id);
}
}  // namespace ekizu
