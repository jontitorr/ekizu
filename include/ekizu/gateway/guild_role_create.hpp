#ifndef EKIZU_GATEWAY_GUILD_ROLE_CREATE_HPP
#define EKIZU_GATEWAY_GUILD_ROLE_CREATE_HPP

#include <ekizu/role.hpp>

namespace ekizu {
struct GuildRoleCreate {
	/// ID of the guild
	Snowflake guild_id;
	/// Role that was created
	Role role;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildRoleCreate &r);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildRoleCreate &r);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_GUILD_ROLE_CREATE_HPP
