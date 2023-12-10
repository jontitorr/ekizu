#ifndef EKIZU_STAGE_INSTANCE_HPP
#define EKIZU_STAGE_INSTANCE_HPP

#include <ekizu/snowflake.hpp>
#include <optional>
#include <string>

namespace ekizu {
struct StageInstance {
	/// The id of this Stage instance.
	Snowflake id;
	/// The guild id of the associated Stage channel.
	Snowflake guild_id;
	/// The id of the associated Stage channel.
	Snowflake channel_id;
	/// The topic of the Stage instance (1-120 characters).
	std::string topic;
	/// The privacy level of the Stage instance.
	int privacy_level;
	/// Whether or not Stage Discovery is disabled (deprecated).
	bool discoverable_disabled;
	/// The id of the scheduled event for this Stage instance.
	std::optional<Snowflake> guild_scheduled_event_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const StageInstance &s);
EKIZU_EXPORT void from_json(const nlohmann::json &j, StageInstance &s);
}  // namespace ekizu

#endif	// EKIZU_STAGE_INSTANCE_HPP
