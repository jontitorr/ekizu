#ifndef EKIZU_GATEWAY_MENTION_HPP
#define EKIZU_GATEWAY_MENTION_HPP

#include <ekizu/user.hpp>

namespace ekizu {
struct Mention {
	/// ID of the user.
	Snowflake id;
	/// Username of the user.
	std::string username;
	/// The user's Discord-tag (discriminator).
	std::string discriminator;
	/// The user's display name, if it is set. For bots, this is the application
	/// name.
	std::optional<std::string> global_name;
	/// The user's avatar hash.
	std::optional<std::string> avatar;
	/// Whether the user belongs to an OAuth2 application.
	bool bot;
	/// Whether the user is an Official Discord System user (part of the urgent
	/// message system).
	std::optional<bool> system;
	/// Whether the user has two-factor authentication enabled on their account.
	std::optional<bool> mfa_enabled;
	/// The user's banner hash.
	std::optional<std::string> banner;
	/// The user's banner color encoded as an integer representation of a
	/// hexadecimal color code.
	std::optional<int> accent_color;
	/// The user's chosen language option.
	std::optional<std::string> locale;
	/// Whether the email on this account has been verified.
	std::optional<bool> verified;
	/// The user's email.
	std::optional<std::string> email;
	/// The flags on a user's account.
	std::optional<int> flags;
	/// The type of Nitro subscription on a user's account.
	std::optional<int> premium_type;
	/// The public flags on a user's account.
	std::optional<int> public_flags;
	/// The user's avatar decoration hash.
	std::optional<std::string> avatar_decoration;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const Mention &m);
EKIZU_EXPORT void from_json(const nlohmann::json &j, Mention &m);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_MENTION_HPP
