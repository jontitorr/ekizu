#ifndef EKIZU_GATEWAY_GUILD_ROLE_DELETE_HPP
#define EKIZU_GATEWAY_GUILD_ROLE_DELETE_HPP

#include <ekizu/role.hpp>

namespace ekizu {
struct GuildRoleDelete {
	/// ID of the guild.
	Snowflake guild_id;
	/// ID of the role that was deleted.
	Snowflake role_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildRoleDelete &e);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildRoleDelete &e);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_GUILD_ROLE_DELETE_HPP
