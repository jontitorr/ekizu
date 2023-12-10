#include <ekizu/guild_scheduled_event.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const EntityMetadata &e) {
	serialize(j, "location", e.location);
}

void from_json(const nlohmann::json &j, EntityMetadata &e) {
	deserialize(j, "location", e.location);
}

void to_json(nlohmann::json &j, const GuildScheduledEvent &e) {
	serialize(j, "id", e.id);
	serialize(j, "guild_id", e.guild_id);
	serialize(j, "channel_id", e.channel_id);
	serialize(j, "creator_id", e.creator_id);
	serialize(j, "name", e.name);
	serialize(j, "description", e.description);
	serialize(j, "scheduled_start_time", e.scheduled_start_time);
	serialize(j, "scheduled_end_time", e.scheduled_end_time);
	serialize(j, "privacy_level", e.privacy_level);
	serialize(j, "status", e.status);
	serialize(j, "entity_type", e.entity_type);
	serialize(j, "entity_id", e.entity_id);
	serialize(j, "entity_metadata", e.entity_metadata);
	serialize(j, "creator", e.creator);
	serialize(j, "user_count", e.user_count);
	serialize(j, "image", e.image);
}

void from_json(const nlohmann::json &j, GuildScheduledEvent &e) {
	deserialize(j, "id", e.id);
	deserialize(j, "guild_id", e.guild_id);
	deserialize(j, "channel_id", e.channel_id);
	deserialize(j, "creator_id", e.creator_id);
	deserialize(j, "name", e.name);
	deserialize(j, "description", e.description);
	deserialize(j, "scheduled_start_time", e.scheduled_start_time);
	deserialize(j, "scheduled_end_time", e.scheduled_end_time);
	deserialize(j, "privacy_level", e.privacy_level);
	deserialize(j, "status", e.status);
	deserialize(j, "entity_type", e.entity_type);
	deserialize(j, "entity_id", e.entity_id);
	deserialize(j, "entity_metadata", e.entity_metadata);
	deserialize(j, "creator", e.creator);
	deserialize(j, "user_count", e.user_count);
	deserialize(j, "image", e.image);
}
}  // namespace ekizu
