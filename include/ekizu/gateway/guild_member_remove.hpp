#ifndef EKIZU_GATEWAY_GUILD_MEMBER_REMOVE_HPP
#define EKIZU_GATEWAY_GUILD_MEMBER_REMOVE_HPP

#include <ekizu/guild_member.hpp>

namespace ekizu {
struct GuildMemberRemove {
	Snowflake guild_id;
	GuildMember member;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildMemberRemove &m);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildMemberRemove &m);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_GUILD_MEMBER_REMOVE_HPP
