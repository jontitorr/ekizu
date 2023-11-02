#ifndef EKIZU_GUILD_HPP
#define EKIZU_GUILD_HPP

#include <ekizu/emoji.hpp>
#include <ekizu/guild_feature.hpp>
#include <ekizu/role.hpp>
#include <ekizu/sticker.hpp>

namespace ekizu
{
enum class DefaultMessageNotificationLevel : uint8_t {
	/// Members will receive notifications for all messages by default
	AllMessages = 0,
	/// Members will receive notifications only for messages that @mention them
	/// by default
	OnlyMentions = 1,
};

enum class ExplicitContentFilter : uint8_t {
	/// Media content will not be scanned
	Disabled = 0,
	/// Media content sent by members without roles will be scanned
	MembersWithoutRoles = 1,
	/// Media content sent by all members will be scanned
	AllMembers = 2,
};

enum class NSFWLevel : uint8_t {
	Default,
	Explicit,
	Safe,
	AgeRestricted,
};

enum class MfaLevel : uint8_t {
	/// Guild has no MFA/2FA requirement for moderation actions.
	None = 0,
	/// Guild has a 2FA requirement for moderation actions
	Elevated = 1,
};

enum class PremiumTier : uint8_t {
	/// Guild has not unlocked any Server Boost perks.
	None,
	/// Guild has unlocked Server Boost level 1 perks.
	Tier1,
	/// Guild has unlocked Server Boost level 2 perks.
	Tier2,
	/// Guild has unlocked Server Boost level 3 perks.
	Tier3,
};

enum class SystemChannelFlags : uint8_t {
	/// Suppress member join notifications.
	SuppressJoinNotifications = 1 << 0,
	/// Suppress server boost notifications.
	SuppressPremiumSubscriptions = 1 << 1,
	/// Suppress server setup tips.
	SuppressGuildReminderNotifications = 1 << 2,
	/// Hide member join sticker reply buttons.
	SuppressJoinNotificationReplies = 1 << 3,
	/// Suppress role subscription purchase and renewal notifications.
	SuppressRoleSubscriptionPurchaseNotifications = 1 << 4,
	/// Hide role subscription sticker reply buttons.
	SuppressRoleSubscriptionPurchaseNotificationReplies = 1 << 5,
};

enum class VerificationLevel : uint8_t {
	/// Unrestricted.
	None = 0,
	/// Must have verified email on account.
	Low = 1,
	/// Must be registered on Discord for longer than 5 minutes
	Medium = 2,
	/// Must be a member of the server for longer than 10 minutes.
	High = 3,
	/// Must have a verified phone number.
	VeryHigh = 4,
};

struct UnavailableGuild {
	/// The ID of the unavailable guild.
	Snowflake id;
	/// Whether the guild is unavailable.
	bool unavailable{};
};

void to_json(nlohmann::json &j, const UnavailableGuild &u);
void from_json(const nlohmann::json &j, UnavailableGuild &u);

/**
 * @brief Represents a Discord server.
 *
 * @see https://discord.com/developers/docs/resources/guild#guild-object
 */
struct Guild {
	/// The ID of the guild.
	Snowflake id;
	/// The name of the guild.
	std::string name;
	/// The hash of the icon.
	std::optional<std::string> icon;
	/// The hash of the icon, when in the template object.
	std::optional<std::string> icon_hash;
	/// The splash hash of the guild.
	std::optional<std::string> splash;
	/// The discovery_splash hash of the guild, only available if the guild
	/// has the "DISCOVERABLE" feature.
	std::optional<std::string> discovery_splash;
	/// Whether the client user is the owner of the guild.
	bool owner{};
	/// The ID of the owner of the guild.
	Snowflake owner_id;
	/// Total permissions for the client user in the guild.
	std::string permissions;
	/// Voice region ID for the guild (DEPRECATED).
	std::optional<std::string> region;
	/// The AFK channel ID for the guild.
	std::optional<Snowflake> afk_channel_id;
	/// The AFK timeout in seconds for the guild.
	uint32_t afk_timeout{};
	/// Whether server widget is enabled for the guild.
	bool widget_enabled{};
	/// The channel ID for the server widget.
	std::optional<Snowflake> widget_channel_id;
	/// Verification level for the guild.
	VerificationLevel verification_level{};
	/// The default message notification level for the guild.
	DefaultMessageNotificationLevel default_message_notifications{};
	/// The explicit content filter level for the guild.
	ExplicitContentFilter explicit_content_filter{};
	/// The roles in the guild.
	std::vector<Role> roles;
	/// The custom emojis in the guild.
	std::vector<Emoji> emojis;
	/// The features enabled on the guild.
	// TODO: Make this a bitset
	std::vector<GuildFeature> features;
	/// The required mfa level for the guild.
	MfaLevel mfa_level{};
	/// The application ID for the guild creator (if it was created by a
	/// bot).
	std::optional<Snowflake> application_id;
	/// The ID of the channel where guild notices such as welcome messages
	/// and boost events are sent.
	std::optional<Snowflake> system_channel_id;
	/// Flags indicating the suppressions enabled for the system channel.
	SystemChannelFlags system_channel_flags{};
	/// The ID of the channel where Community guilds can display rules and
	/// guidelines.
	std::optional<Snowflake> rules_channel_id;
	/// The maximum number of presences for the guild (null is always
	/// returned, apart from the largest of guilds).
	std::optional<uint64_t> max_presences;
	/// The vanity URL code for the guild.
	std::string vanity_url_code;
	/// The description for the guild.
	std::string description;
	/// The banner hash for the guild.
	std::string banner;
	/// The Server Boost level for the guild.
	PremiumTier premium_tier{};
	/// The number of boosts this guild currently has.
	uint64_t premium_subscription_count{};
	/// The preferred locale of a Community guild, used for server discovery
	/// and noticed from Discord, and send in interaction (default to
	/// en-US).
	std::string preferred_locale;
	/// The ID of the channel where admins and moderators of Community
	/// guilds receive notices from Discord.
	std::optional<Snowflake> public_updates_channel_id;
	/// The maximum amount of users in a video channel.
	uint32_t max_video_channel_users{};
	/// Approximate number of members in the guild, returned from the GET
	/// /guilds/<id> endpoint when with_counts is true.
	uint32_t approximate_member_count{};
	/// Approximate number of non-offline members in this guild, returned
	/// from the GET /guilds/<id> endpoint when with_counts is true.
	uint32_t approximate_presence_count{};
	/// The guild NSFW level.
	NSFWLevel nsfw_level{};
	/// Custom guild stickers.
	std::vector<Sticker> stickers;
	/// Whether the guild has the boost progress bar enabled.
	bool premium_progress_bar_enabled{};
	/// The id of the channel where admins and moderators of Community guilds
	/// receive safety alerts from Discord.
	std::optional<Snowflake> safety_alerts_channel_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const Guild &g);
EKIZU_EXPORT void from_json(const nlohmann::json &j, Guild &g);
} // namespace ekizu

#endif // EKIZU_GUILD_HPP
