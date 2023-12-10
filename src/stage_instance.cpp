#include <ekizu/json_util.hpp>
#include <ekizu/stage_instance.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const StageInstance &s) {
	serialize(j, "id", s.id);
	serialize(j, "guild_id", s.guild_id);
	serialize(j, "channel_id", s.channel_id);
	serialize(j, "topic", s.topic);
	serialize(j, "privacy_level", s.privacy_level);
	serialize(j, "discoverable_disabled", s.discoverable_disabled);
	serialize(j, "guild_scheduled_event_id", s.guild_scheduled_event_id);
}

void from_json(const nlohmann::json &j, StageInstance &s) {
	deserialize(j, "id", s.id);
	deserialize(j, "guild_id", s.guild_id);
	deserialize(j, "channel_id", s.channel_id);
	deserialize(j, "topic", s.topic);
	deserialize(j, "privacy_level", s.privacy_level);
	deserialize(j, "discoverable_disabled", s.discoverable_disabled);
	deserialize(j, "guild_scheduled_event_id", s.guild_scheduled_event_id);
}
}  // namespace ekizu
