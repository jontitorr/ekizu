#include <ekizu/guild.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const UnavailableGuild &u) {
	serialize(j, "id", u.id);
	serialize(j, "unavailable", u.unavailable);
}

void from_json(const nlohmann::json &j, UnavailableGuild &u) {
	deserialize(j, "id", u.id);
	deserialize(j, "unavailable", u.unavailable);
}

void to_json(nlohmann::json &j, const Guild &g) {
	serialize(j, "id", g.id);
	serialize(j, "name", g.name);
	serialize(j, "icon", g.icon);
	serialize(j, "icon_hash", g.icon_hash);
	serialize(j, "splash", g.splash);
	serialize(j, "discovery_splash", g.discovery_splash);
	serialize(j, "owner", g.owner);
	serialize(j, "owner_id", g.owner_id);
	serialize(j, "permissions", g.permissions);
	serialize(j, "region", g.region);
	serialize(j, "afk_channel_id", g.afk_channel_id);
	serialize(j, "afk_timeout", g.afk_timeout);
	serialize(j, "widget_enabled", g.widget_enabled);
	serialize(j, "widget_channel_id", g.widget_channel_id);
	serialize(j, "verification_level", g.verification_level);
	serialize(
		j, "default_message_notifications", g.default_message_notifications);
	serialize(j, "explicit_content_filter", g.explicit_content_filter);
	serialize(j, "roles", g.roles);
	serialize(j, "emojis", g.emojis);
	serialize(j, "features", g.features);
	serialize(j, "mfa_level", g.mfa_level);
	serialize(j, "application_id", g.application_id);
	serialize(j, "system_channel_id", g.system_channel_id);
	serialize(j, "system_channel_flags", g.system_channel_flags);
	serialize(j, "rules_channel_id", g.rules_channel_id);
	serialize(j, "max_presences", g.max_presences);
	serialize(j, "vanity_url_code", g.vanity_url_code);
	serialize(j, "description", g.description);
	serialize(j, "banner", g.banner);
	serialize(j, "premium_tier", g.premium_tier);
	serialize(j, "premium_subscription_count", g.premium_subscription_count);
	serialize(j, "preferred_locale", g.preferred_locale);
	serialize(j, "public_updates_channel_id", g.public_updates_channel_id);
	serialize(j, "max_video_channel_users", g.max_video_channel_users);
	serialize(j, "approximate_member_count", g.approximate_member_count);
	serialize(j, "approximate_presence_count", g.approximate_presence_count);
	serialize(j, "nsfw_level", g.nsfw_level);
	serialize(j, "stickers", g.stickers);
	serialize(
		j, "premium_progress_bar_enabled", g.premium_progress_bar_enabled);
	serialize(j, "safety_alerts_channel_id", g.safety_alerts_channel_id);
	serialize(j, "voice_states", g.voice_states);
}

void from_json(const nlohmann::json &j, Guild &g) {
	deserialize(j, "id", g.id);
	deserialize(j, "name", g.name);
	deserialize(j, "icon", g.icon);
	deserialize(j, "icon_hash", g.icon_hash);
	deserialize(j, "splash", g.splash);
	deserialize(j, "discovery_splash", g.discovery_splash);
	deserialize(j, "owner", g.owner);
	deserialize(j, "owner_id", g.owner_id);
	deserialize(j, "permissions", g.permissions);
	deserialize(j, "region", g.region);
	deserialize(j, "afk_channel_id", g.afk_channel_id);
	deserialize(j, "afk_timeout", g.afk_timeout);
	deserialize(j, "widget_enabled", g.widget_enabled);
	deserialize(j, "widget_channel_id", g.widget_channel_id);
	deserialize(j, "verification_level", g.verification_level);
	deserialize(
		j, "default_message_notifications", g.default_message_notifications);
	deserialize(j, "explicit_content_filter", g.explicit_content_filter);
	deserialize(j, "roles", g.roles);
	deserialize(j, "emojis", g.emojis);
	deserialize(j, "features", g.features);
	deserialize(j, "mfa_level", g.mfa_level);
	deserialize(j, "application_id", g.application_id);
	deserialize(j, "system_channel_id", g.system_channel_id);
	deserialize(j, "system_channel_flags", g.system_channel_flags);
	deserialize(j, "rules_channel_id", g.rules_channel_id);
	deserialize(j, "max_presences", g.max_presences);
	deserialize(j, "vanity_url_code", g.vanity_url_code);
	deserialize(j, "description", g.description);
	deserialize(j, "banner", g.banner);
	deserialize(j, "premium_tier", g.premium_tier);
	deserialize(j, "premium_subscription_count", g.premium_subscription_count);
	deserialize(j, "preferred_locale", g.preferred_locale);
	deserialize(j, "public_updates_channel_id", g.public_updates_channel_id);
	deserialize(j, "max_video_channel_users", g.max_video_channel_users);
	deserialize(j, "approximate_member_count", g.approximate_member_count);
	deserialize(j, "approximate_presence_count", g.approximate_presence_count);
	deserialize(j, "nsfw_level", g.nsfw_level);
	deserialize(j, "stickers", g.stickers);
	deserialize(
		j, "premium_progress_bar_enabled", g.premium_progress_bar_enabled);
	deserialize(j, "safety_alerts_channel_id", g.safety_alerts_channel_id);
	deserialize(j, "members", g.members);
	deserialize(j, "voice_states", g.voice_states);
}
}  // namespace ekizu
