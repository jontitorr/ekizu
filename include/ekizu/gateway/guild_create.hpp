#ifndef EKIZU_GATEWAY_GUILD_CREATE_HPP
#define EKIZU_GATEWAY_GUILD_CREATE_HPP

#include <ekizu/guild.hpp>

namespace ekizu {
struct GuildCreate {
	Guild guild;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildCreate &g);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildCreate &g);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_GUILD_CREATE_HPP
