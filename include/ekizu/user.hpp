#ifndef EKIZU_USER_HPP
#define EKIZU_USER_HPP

#include <ekizu/snowflake.hpp>
#include <optional>

namespace ekizu {
/**
 * @brief The type of Nitro subscription on a user's account.
 * @see
 * https://discord.com/developers/docs/resources/user#user-object-premium-types
 */
enum class PremiumType : uint8_t { None, NitroClassic, Nitro, NitroBasic };

enum class UserFlags : uint32_t {
	/// Discord Employee
	Staff = 1 << 0,
	/// Partnered Server Owner
	Partner = 1 << 1,
	/// HypeSquad Events Member
	HypeSquad = 1 << 2,
	/// Bug Hunter Level 1
	BugHunterLevel1 = 1 << 3,
	/// House Bravery Member
	HypeSquadOnlineHouse1 = 1 << 6,
	/// House Brilliance Member
	HypeSquadOnlineHouse2 = 1 << 7,
	/// House Balance Member
	HypeSquadOnlineHouse3 = 1 << 8,
	/// Early Nitro Supporter
	PremiumEarlySupporter = 1 << 9,
	/// User is a team
	TeamPseudoUser = 1 << 10,
	/// Bug Hunter Level 2
	BugHunterLevel2 = 1 << 14,
	/// Verified Bot
	VerifiedBot = 1 << 16,
	/// Early Verified Bot Developer
	VerifiedDeveloper = 1 << 17,
	/// Moderator Programs Alumni
	CertifiedModerator = 1 << 18,
	/// Bot uses only HTTP interactions and is shown in the online member list
	BotHttpInteractions = 1 << 19,
	/// User is an Active Developer
	ActiveDeveloper = 1 << 22
};

/**
 * @brief Represents a Discord user.
 * @see https://discord.com/developers/docs/resources/user#user-object
 */
struct User {
	/// The user's id.
	Snowflake id;
	/// The user's username, not unique across the platform.
	std::string username;
	/// The user's Discord-tag.
	std::string discriminator;
	/// The user's display name, if it is set. For bots, this is the application
	/// name.
	std::optional<std::string> global_name;
	/// The user's avatar hash.
	std::optional<std::string> avatar;
	/// Whether the user belongs to an OAuth2 application.
	bool bot{};
	/// Whether the user is an Official Discord System user (part of the urgent
	/// message system).
	bool system{};
	/// Whether the user has two factor enabled on their account.
	bool mfa_enabled{};
	/// The user's banner hash.
	std::optional<std::string> banner;
	/// The user's banner color encoded as an integer representation of
	/// hexadecimal color code
	std::optional<uint16_t> accent_color;
	/// The user's chosen language option.
	std::string locale;
	/// Whether the email on this account has been verified.
	bool verified{};
	/// The user's email.
	std::optional<std::string> email;
	/// The flags on a user's account.
	UserFlags flags{};
	/// The type of Nitro subscription on a user's account.
	PremiumType premium_type{};
	/// The flags of the user's public profile.
	UserFlags public_flags{};
	/// The user's avatar decoration hash.
	std::optional<std::string> avatar_decoration;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const User &u);
EKIZU_EXPORT void from_json(const nlohmann::json &j, User &u);
}  // namespace ekizu

#endif	// EKIZU_USER_HPP
