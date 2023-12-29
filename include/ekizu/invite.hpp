#ifndef EKIZU_INVITE_HPP
#define EKIZU_INVITE_HPP

#include <ekizu/channel.hpp>
#include <ekizu/guild.hpp>

namespace ekizu {
struct InviteChannel {
	Snowflake id;
	std::string name;
	ChannelType type;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const InviteChannel &i);
EKIZU_EXPORT void from_json(const nlohmann::json &j, InviteChannel &i);

struct InviteGuild {
	Snowflake id;
	std::string name;
	std::optional<std::string> splash;
	std::optional<std::string> banner;
	std::optional<std::string> description;
	std::optional<std::string> icon;
	std::vector<GuildFeature> features;
	VerificationLevel verification_level;
	std::optional<std::string> vanity_url_code;
	NSFWLevel nsfw_level;
	uint64_t premium_subscription_count;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const InviteGuild &i);
EKIZU_EXPORT void from_json(const nlohmann::json &j, InviteGuild &i);

enum class TargetType : uint8_t { Stream = 1, EmbeddedApplication = 2 };

struct Invite {
	std::string code;
	std::optional<InviteGuild> guild;
	InviteChannel channel;
	std::optional<User> inviter;
	TargetType target_type;
	std::optional<User> target_user;
	std::optional<uint64_t> approximate_presence_count;
	std::optional<uint64_t> approximate_member_count;
	std::optional<std::string> expires_at;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const Invite &i);
EKIZU_EXPORT void from_json(const nlohmann::json &j, Invite &i);
}  // namespace ekizu

#endif	// EKIZU_INVITE_HPP
