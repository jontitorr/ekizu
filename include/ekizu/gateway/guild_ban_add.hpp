#ifndef EKIZU_GATEWAY_GUILD_BAN_ADD_HPP
#define EKIZU_GATEWAY_GUILD_BAN_ADD_HPP

#include <ekizu/user.hpp>

namespace ekizu {
struct GuildBanAdd {
	/// The ID of the guild.
	Snowflake guild_id;
	/// The banned user.
	User user;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildBanAdd &b);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildBanAdd &b);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_GUILD_BAN_ADD_HPP
