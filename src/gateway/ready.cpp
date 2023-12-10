#include <ekizu/gateway/ready.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const Ready &payload) {
	serialize(j, "v", payload.v);
	serialize(j, "user", payload.user);
	serialize(j, "guilds", payload.guilds);
	serialize(j, "session_id", payload.session_id);
	serialize(j, "resume_gateway_url", payload.resume_gateway_url);
	serialize(j, "shard_info", payload.shard_info);
	serialize(j, "application", payload.application);
}

void from_json(const nlohmann::json &j, Ready &payload) {
	deserialize(j, "v", payload.v);
	deserialize(j, "user", payload.user);
	deserialize(j, "guilds", payload.guilds);
	deserialize(j, "session_id", payload.session_id);
	deserialize(j, "resume_gateway_url", payload.resume_gateway_url);
	deserialize(j, "shard_info", payload.shard_info);
	deserialize(j, "application", payload.application);
}
}  // namespace ekizu
