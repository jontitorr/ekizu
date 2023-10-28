#ifndef EKIZU_GATEWAY_GUILD_MEMBER_ADD_HPP
#define EKIZU_GATEWAY_GUILD_MEMBER_ADD_HPP

#include <ekizu/guild_member.hpp>

namespace ekizu
{
struct GuildMemberAdd {
	Snowflake guild_id;
	GuildMember member;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildMemberAdd &m);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildMemberAdd &m);
} // namespace ekizu

#endif // EKIZU_GATEWAY_GUILD_MEMBER_ADD_HPP
