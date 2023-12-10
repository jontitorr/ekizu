#include <ekizu/gateway/message_delete.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const MessageDelete &m) {
	serialize(j, "id", m.id);
	serialize(j, "channel_id", m.channel_id);
	if (m.guild_id.has_value()) {
		serialize(j, "guild_id", m.guild_id.value());
	}
}

void from_json(const nlohmann::json &j, MessageDelete &m) {
	deserialize(j, "id", m.id);
	deserialize(j, "channel_id", m.channel_id);
	deserialize(j, "guild_id", m.guild_id);
}
}  // namespace ekizu
