#ifndef EKIZU_GATEWAY_GUILD_MEMBER_UPDATE_HPP
#define EKIZU_GATEWAY_GUILD_MEMBER_UPDATE_HPP

#include <ekizu/guild_member.hpp>

namespace ekizu
{
struct GuildMemberUpdate {
	Snowflake guild_id;
	std::vector<Snowflake> roles;
	User user;
	std::optional<std::string> nick;
	std::optional<std::string> avatar;
	std::optional<std::string> joined_at;
	std::optional<std::string> premium_since;
	std::optional<bool> deaf;
	std::optional<bool> mute;
	bool pending{};
	std::optional<std::string> communication_disabled_until;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildMemberUpdate &m);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildMemberUpdate &m);
} // namespace ekizu

#endif // EKIZU_GATEWAY_GUILD_MEMBER_UPDATE_HPP
