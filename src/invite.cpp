#include <ekizu/invite.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const InviteChannel &i) {
	serialize(j, "id", i.id);
	serialize(j, "name", i.name);
	serialize(j, "type", i.type);
}

void from_json(const nlohmann::json &j, InviteChannel &i) {
	deserialize(j, "id", i.id);
	deserialize(j, "name", i.name);
	deserialize(j, "type", i.type);
}

void to_json(nlohmann::json &j, const InviteGuild &i) {
	serialize(j, "id", i.id);
	serialize(j, "name", i.name);
	serialize(j, "splash", i.splash);
	serialize(j, "banner", i.banner);
	serialize(j, "description", i.description);
	serialize(j, "icon", i.icon);
	serialize(j, "features", i.features);
	serialize(j, "verification_level", i.verification_level);
	serialize(j, "vanity_url_code", i.vanity_url_code);
	serialize(j, "nsfw_level", i.nsfw_level);
	serialize(j, "premium_subscription_count", i.premium_subscription_count);
}

void from_json(const nlohmann::json &j, InviteGuild &i) {
	deserialize(j, "id", i.id);
	deserialize(j, "name", i.name);
	deserialize(j, "splash", i.splash);
	deserialize(j, "banner", i.banner);
	deserialize(j, "description", i.description);
	deserialize(j, "icon", i.icon);
	deserialize(j, "features", i.features);
	deserialize(j, "verification_level", i.verification_level);
	deserialize(j, "vanity_url_code", i.vanity_url_code);
	deserialize(j, "nsfw_level", i.nsfw_level);
	deserialize(j, "premium_subscription_count", i.premium_subscription_count);
}

void to_json(nlohmann::json &j, const Invite &i) {
	serialize(j, "code", i.code);
	serialize(j, "guild", i.guild);
	serialize(j, "channel", i.channel);
	serialize(j, "inviter", i.inviter);
	serialize(j, "target_user", i.target_user);
	serialize(j, "approximate_presence_count", i.approximate_presence_count);
	serialize(j, "approximate_member_count", i.approximate_member_count);
	serialize(j, "expires_at", i.expires_at);
}

void from_json(const nlohmann::json &j, Invite &i) {
	deserialize(j, "code", i.code);
	deserialize(j, "guild", i.guild);
	deserialize(j, "channel", i.channel);
	deserialize(j, "inviter", i.inviter);
	deserialize(j, "target_user", i.target_user);
	deserialize(j, "approximate_presence_count", i.approximate_presence_count);
	deserialize(j, "approximate_member_count", i.approximate_member_count);
	deserialize(j, "expires_at", i.expires_at);
}

std::string Invite::url() const {
	return fmt::format("https://discord.com/invite/{}", code);
}
}  // namespace ekizu
