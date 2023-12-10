#ifndef EKIZU_GATEWAY_GUILD_ROLE_UPDATE_HPP
#define EKIZU_GATEWAY_GUILD_ROLE_UPDATE_HPP

#include <ekizu/role.hpp>

namespace ekizu {
struct GuildRoleUpdate {
	/// ID of the guild.
	Snowflake guild_id;
	/// Role that was updated.
	Role role;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildRoleUpdate &e);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildRoleUpdate &e);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_GUILD_ROLE_UPDATE_HPP
