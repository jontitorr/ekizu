#ifndef EKIZU_GATEWAY_BAN_REMOVE_HPP
#define EKIZU_GATEWAY_BAN_REMOVE_HPP

#include <ekizu/user.hpp>

namespace ekizu
{
struct BanRemove {
	/// The ID of the guild.
	Snowflake guild_id;
	/// The unbanned user.
	User user;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const BanRemove &b);
EKIZU_EXPORT void from_json(const nlohmann::json &j, BanRemove &b);
} // namespace ekizu

#endif // EKIZU_GATEWAY_BAN_REMOVE_HPP
