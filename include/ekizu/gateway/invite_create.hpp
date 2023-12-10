#ifndef EKIZU_MODELS_INVITE_CREATE_HPP
#define EKIZU_MODELS_INVITE_CREATE_HPP

#include <ekizu/user.hpp>

namespace ekizu {
struct PartialUser {
	/// ID of the user.
	Snowflake id;
	/// Username of the user.
	std::string username;
	/// Discriminator of the user.
	std::string discriminator;
	/// Avatar hash of the user.
	std::string avatar;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const PartialUser &u);
EKIZU_EXPORT void from_json(const nlohmann::json &j, PartialUser &u);

enum class TargetType : uint8_t { Stream = 1, EmbeddedApplication = 2 };

struct InviteCreate {
	/// ID of the channel invited users will first see.
	Snowflake channel_id;
	/// Unique code.
	std::string code;
	/// When the invite was created.
	std::string created_at;
	/// ID of the guild being invited to.
	Snowflake guild_id;
	/// Information about the user who created the invite.
	User inviter;
	/// Maximum age before the invite expires (in seconds).
	uint64_t max_age;
	/// Maximum number of uses before the invite expires.
	uint64_t max_uses;
	/// Target of the invite.
	TargetType target_user_type;
	/// User whose stream to display for this voice channel stream invite.
	PartialUser target_user;
	/// Whether the invite is temporary.
	bool temporary;
	/// Number of times the invite has been used.
	uint8_t uses;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const InviteCreate &invite);
EKIZU_EXPORT void from_json(const nlohmann::json &j, InviteCreate &invite);
}  // namespace ekizu

#endif	// EKIZU_MODELS_INVITE_CREATE_HPP
