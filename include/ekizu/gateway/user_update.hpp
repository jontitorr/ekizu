#ifndef EKIZU_GATEWAY_USER_UPDATE_HPP
#define EKIZU_GATEWAY_USER_UPDATE_HPP

#include <ekizu/current_user.hpp>

namespace ekizu {
struct UserUpdate {
	CurrentUser user;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const UserUpdate &u);
EKIZU_EXPORT void from_json(const nlohmann::json &j, UserUpdate &u);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_USER_UPDATE_HPP
