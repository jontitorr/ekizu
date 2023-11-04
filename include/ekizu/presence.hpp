#ifndef EKIZU_PRESENCE_HPP
#define EKIZU_PRESENCE_HPP

#include <ekizu/activity_flags.hpp>
#include <ekizu/snowflake.hpp>
#include <optional>

namespace ekizu
{
/**
* @brief Represents the images for the presence and their hover texts.
*
* @see https://discord.com/developers/docs/topics/gateway-events#activity-object-activity-assets.
*/
struct ActivityAssets {
	/// The ID for the large image asset of the activity.
	std::optional<std::string> large_image;
	/// The text that appears when hovering over the large image asset.
	std::optional<std::string> large_text;
	/// The ID for the small image asset of the activity.
	std::optional<std::string> small_image;
	/// The text that appears when hovering over the small image asset.
	std::optional<std::string> small_text;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ActivityAssets &a);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ActivityAssets &a);

/**
* @brief Represents a button shown in the Rich Presence.
*
* This struct represents a button that can be shown in the Rich Presence of an activity.
* It contains the label and the URL associated with the button.
*
* @see https://discord.com/developers/docs/topics/gateway-events#activity-object-activity-buttons
*/
struct ActivityButton {
	/// Text shown on the button (1-32 characters)
	std::string label;
	/// URL opened when clicking the button (1-512 characters)
	std::string url;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ActivityButton &b);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ActivityButton &b);

/**
* @brief Represents an emoji used for a custom status in an activity.
*
* This struct represents an emoji that can be used as a custom status in the activity object.
* It contains the name, ID, and whether the emoji is animated or not.
*
* @see https://discord.com/developers/docs/topics/gateway-events#activity-object-activity-emoji
*/
struct ActivityEmoji {
	/// Name of the emoji
	std::string name;
	/// ID of the emoji
	Snowflake id;
	/// Whether this emoji is animated
	std::optional<bool> animated;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ActivityEmoji &e);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ActivityEmoji &e);

struct ActivityParty {
	/// Party id
	std::optional<std::string> id;
	/// Party size
	std::optional<std::array<uint64_t, 2> > size;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ActivityParty &p);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ActivityParty &p);

struct ActivitySecrets {
	/// Secrets for the game
	std::optional<std::string> join;
	/// Secrets for the game
	std::optional<std::string> spectate;
	/// Secrets for the game
	std::optional<std::string> match;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ActivitySecrets &s);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ActivitySecrets &s);

struct ActivityTimestamps {
	/// Unix timestamp (in milliseconds) of when the activity was added to the user's session
	std::optional<uint64_t> start;
	/// Unix timestamp (in milliseconds) of when the activity was added to the user's session
	std::optional<uint64_t> end;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ActivityTimestamps &t);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ActivityTimestamps &t);

enum class ActivityType : uint8_t {
	Playing,
	Streaming,
	Listening,
	Watching,
	Custom,
	Competing,
	Unknown,
};

struct Activity {
	/// Activity's name
	std::string name;
	/// Activity type
	ActivityType type;
	/// Stream URL, is validated when type is 1
	std::optional<std::string> url;
	/// Unix timestamp (in milliseconds) of when the activity was added to the user's session
	std::optional<uint64_t> created_at;
	/// Unix timestamps for start and/or end of the game
	std::optional<ActivityTimestamps> timestamps;
	/// Application ID for the game
	std::optional<Snowflake> application_id;
	/// What the player is currently doing
	std::optional<std::string> details;
	/// User's current party status, or text used for a custom status
	std::optional<std::string> state;
	/// Emoji used for a custom status
	std::optional<ActivityEmoji> emoji;
	/// Information for the current party of the player
	std::optional<ActivityParty> party;
	/// Images for the presence and their hover texts
	std::optional<ActivityAssets> assets;
	/// Secrets for Rich Presence joining and spectating
	std::optional<ActivitySecrets> secrets;
	/// Whether or not the activity is an instanced game session
	std::optional<bool> instance;
	/// Activity flags ORed together, describes what the payload includes
	std::optional<ActivityFlags> flags;
	/// Custom buttons shown in the Rich Presence (max 2)
	std::vector<ActivityButton> buttons;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const Activity &a);
EKIZU_EXPORT void from_json(const nlohmann::json &j, Activity &a);

enum class Status : uint8_t {
	DoNotDisturb,
	Idle,
	Invisible,
	Offline,
	Online,
	Unknown,
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const Status &s);
EKIZU_EXPORT void from_json(const nlohmann::json &j, Status &s);

struct ClientStatus {
	std::optional<Status> desktop;
	std::optional<Status> mobile;
	std::optional<Status> web;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ClientStatus &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ClientStatus &c);

/**
 * @brief Represents a user's presence on a guild.
 *
 * A user's presence is their current state on a guild. This event is sent when a user's presence or info, such as name or avatar, is updated.
 *
 * For more information, refer to the [Discord API documentation](https://discord.com/developers/docs/topics/gateway-events#presence).
 */
struct Presence {
	/// The user's activities.
	std::vector<Activity> activities;
	/// The client status of the user.
	ClientStatus client_status;
	/// The ID of the guild.
	Snowflake guild_id;
	/// The status of the user.
	Status status;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const Presence &p);
EKIZU_EXPORT void from_json(const nlohmann::json &j, Presence &p);
} // namespace ekizu

#endif // EKIZU_PRESENCE_HPP
