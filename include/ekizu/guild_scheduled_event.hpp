#ifndef EKIZU_GUILD_SCHEDULED_EVENT_HPP
#define EKIZU_GUILD_SCHEDULED_EVENT_HPP

#include <ekizu/snowflake.hpp>
#include <ekizu/user.hpp>

namespace ekizu
{
struct EntityMetadata {
	/// The location of the event (1-100 characters).
	std::optional<std::string> location;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const EntityMetadata &e);
EKIZU_EXPORT void from_json(const nlohmann::json &j, EntityMetadata &e);

enum class EventStatus : uint8_t {
	Scheduled = 1,
	Active,
	Completed,
	Canceled,
};

enum class PrivacyLevel : uint8_t {
	/// The scheduled event is only accessible to guild members.
	GuildOnly = 2,
};

enum class ScheduledEntityType : uint8_t {
	StageInstance = 1,
	Voice,
	External,
};

struct GuildScheduledEvent {
	/// The ID of the scheduled event.
	Snowflake id;
	/// The guild ID to which the scheduled event belongs.
	Snowflake guild_id;
	/// The channel ID in which the scheduled event will be hosted (null if scheduled entity type is EXTERNAL).
	std::optional<Snowflake> channel_id;
	/// The ID of the user that created the scheduled event.
	std::optional<Snowflake> creator_id;
	/// The name of the scheduled event (1-100 characters).
	std::string name;
	/// The description of the scheduled event (1-1000 characters).
	std::optional<std::string> description;
	/// The time the scheduled event will start.
	std::string scheduled_start_time;
	/// The time the scheduled event will end (required if entity_type is EXTERNAL).
	std::optional<std::string> scheduled_end_time;
	/// The privacy level of the scheduled event.
	PrivacyLevel privacy_level;
	/// The status of the scheduled event.
	EventStatus status;
	/// The type of the scheduled event.
	ScheduledEntityType entity_type;
	/// The ID of an entity associated with a guild scheduled event.
	std::optional<Snowflake> entity_id;
	/// Additional metadata for the guild scheduled event.
	std::optional<EntityMetadata> entity_metadata;
	/// The user that created the scheduled event.
	User creator;
	/// The number of users subscribed to the scheduled event.
	std::optional<int32_t> user_count;
	/// The cover image hash of the scheduled event.
	std::optional<std::string> image;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildScheduledEvent &e);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildScheduledEvent &e);
} // namespace ekizu

#endif // EKIZU_GUILD_SCHEDULED_EVENT_HPP
