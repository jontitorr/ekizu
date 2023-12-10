#ifndef EKIZU_PARTIAL_GUILD_HPP
#define EKIZU_PARTIAL_GUILD_HPP

#include <ekizu/guild.hpp>
#include <ekizu/permissions.hpp>

namespace ekizu {
struct PartialGuild {
	/// Guild ID.
	Snowflake id;
	/// Guild name (2-100 characters, excluding trailing and leading
	/// whitespace).
	std::string name;
	/// Icon hash.
	std::optional<std::string> icon;
	/// Icon hash (returned when in the template object).
	std::optional<std::string> icon_hash;
	/// Splash hash.
	std::optional<std::string> splash;
	/// Discovery splash hash (only present for guilds with the "DISCOVERABLE"
	/// feature).
	std::optional<std::string> discovery_splash;
	/// True if the user is the owner of the guild.
	std::optional<bool> owner;
	/// ID of owner.
	Snowflake owner_id;
	/// Total permissions for the user in the guild (excludes overwrites and
	/// implicit permissions).
	std::optional<Permissions> permissions;
	/// Voice region ID for the guild (deprecated).
	std::optional<std::string> region;
	/// ID of AFK channel.
	std::optional<Snowflake> afk_channel_id;
	/// AFK timeout in seconds.
	uint16_t afk_timeout;
	/// True if the server widget is enabled.
	std::optional<bool> widget_enabled;
	/// Channel ID that the widget will generate an invite to, or null if set to
	/// no invite.
	std::optional<Snowflake> widget_channel_id;
	/// Verification level required for the guild.
	VerificationLevel verification_level;
	/// Default message notifications level.
	DefaultMessageNotificationLevel default_message_notifications;
	/// Explicit content filter level.
	ExplicitContentFilter explicit_content_filter;
	/// Roles in the guild.
	std::vector<Role> roles;
	/// Custom guild emojis.
	std::vector<Emoji> emojis;
	/// Enabled guild features.
	std::vector<GuildFeature> features;
	/// Required MFA level for the guild.
	MfaLevel mfa_level;
	/// Application ID of the guild creator if it is bot-created.
	std::optional<Snowflake> application_id;
	/// The ID of the channel where guild notices such as welcome messages and
	/// boost events are posted.
	std::optional<Snowflake> system_channel_id;
	/// System channel flags.
	SystemChannelFlags system_channel_flags;
	/// The ID of the channel where Community guilds can display rules and/or
	/// guidelines.
	std::optional<Snowflake> rules_channel_id;
	/// The maximum number of presences for the guild (null is always returned,
	/// apart from the largest of guilds).
	std::optional<uint64_t> max_presences;
	/// The maximum number of members for the guild.
	std::optional<uint64_t> max_members;
	/// The vanity URL code for the guild.
	std::optional<std::string> vanity_url_code;
	/// The description of a guild.
	std::optional<std::string> description;
	/// Banner hash.
	std::optional<std::string> banner;
	/// Premium tier (Server Boost level).
	PremiumTier premium_tier;
	/// The number of boosts this guild currently has.
	std::optional<uint64_t> premium_subscription_count;
	/// The preferred locale of a Community guild; used in server discovery and
	/// notices from Discord, and sent in interactions; defaults to "en-US".
	std::string preferred_locale;
	/// The ID of the channel where admins and moderators of Community guilds
	/// receive notices from Discord.
	std::optional<Snowflake> public_updates_channel_id;
	/// Guild NSFW level.
	NSFWLevel nsfw_level;
	/// Custom guild stickers.
	std::vector<Sticker> stickers;
	/// Whether the guild has the boost progress bar enabled.
	bool premium_progress_bar_enabled;
	/// Maximum amount of users in a video channel.
	std::optional<uint64_t> max_video_channel_users;
	/// Maximum amount of users in a stage video channel.
	std::optional<uint64_t> max_stage_video_channel_users;
	/// Approximate number of members in this guild, returned from the GET
	/// /guilds/<id> and /users/@me/guilds endpoints when with_counts is true.
	std::optional<uint64_t> approximate_member_count;
	/// Approximate number of non-offline members in this guild, returned from
	/// the GET /guilds/<id> and /users/@me/guilds endpoints when with_counts is
	/// true.
	std::optional<uint64_t> approximate_presence_count;
	/// The welcome screen of a Community guild, shown to new members, returned
	/// in an Invite's guild object.
	// Add welcome screen object here.
	// Guild WelcomeScreen welcome_screen;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const PartialGuild &g);
EKIZU_EXPORT void from_json(const nlohmann::json &j, PartialGuild &g);
}  // namespace ekizu

#endif	// EKIZU_PARTIAL_GUILD_HPP
