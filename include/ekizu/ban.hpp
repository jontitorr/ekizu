#ifndef EKIZU_BAN_HPP
#define EKIZU_BAN_HPP

#include <ekizu/user.hpp>

namespace ekizu {
/**
 * @brief Represents a ban of a user from a Discord guild.
 */
struct Ban {
	/// The reason for the ban.
	std::optional<std::string> reason;
	/// The banned user.
	User user;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const Ban &b);
EKIZU_EXPORT void from_json(const nlohmann::json &j, Ban &b);
}  // namespace ekizu

#endif	// EKIZU_BAN_HPP
