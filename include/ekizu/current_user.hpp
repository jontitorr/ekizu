#ifndef EKIZU_CURRENT_USER_HPP
#define EKIZU_CURRENT_USER_HPP

#include <ekizu/user.hpp>

namespace ekizu
{
/**
 * @brief Represents the current user of the client. This is very similar to the
 * `User` struct.
 */
struct CurrentUser {
	/// The user's id.
	Snowflake id;
	/// The user's username, not unique across the platform.
	std::string username;
	/// The user's Discord-tag.
	std::string discriminator;
	/// The user's avatar hash.
	std::optional<std::string> avatar;
	/// Whether the user belongs to an OAuth2 application.
	bool bot{};
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
	/// The flags on a user's account.
	UserFlags flags{};
	/// The type of Nitro subscription on a user's account.
	PremiumType premium_type{};
	/// The flags of the user's public profile.
	UserFlags public_flags{};
};

void to_json(nlohmann::json &j, const CurrentUser &u);
void from_json(const nlohmann::json &j, CurrentUser &u);
} // namespace ekizu

#endif // EKIZU_CURRENT_USER_HPP
