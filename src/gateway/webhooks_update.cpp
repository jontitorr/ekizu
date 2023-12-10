#include <ekizu/gateway/webhooks_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const WebhooksUpdate &w) {
	serialize(j, "guild_id", w.guild_id);
	serialize(j, "channel_id", w.channel_id);
}

void from_json(const nlohmann::json &j, WebhooksUpdate &w) {
	deserialize(j, "guild_id", w.guild_id);
	deserialize(j, "channel_id", w.channel_id);
}
}  // namespace ekizu
