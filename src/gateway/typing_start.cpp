#include <ekizu/gateway/typing_start.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const TypingStart &t) {
	serialize(j, "channel_id", t.channel_id);
	serialize(j, "guild_id", t.guild_id);
	serialize(j, "user_id", t.user_id);
	serialize(j, "timestamp", t.timestamp);
	serialize(j, "member", t.member);
}

void from_json(const nlohmann::json &j, TypingStart &t) {
	deserialize(j, "channel_id", t.channel_id);
	deserialize(j, "guild_id", t.guild_id);
	deserialize(j, "user_id", t.user_id);
	deserialize(j, "timestamp", t.timestamp);
	deserialize(j, "member", t.member);
}
}  // namespace ekizu
