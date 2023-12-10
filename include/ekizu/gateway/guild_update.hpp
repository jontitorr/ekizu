#ifndef EKIZU_GATEWAY_GUILD_UPDATE_HPP
#define EKIZU_GATEWAY_GUILD_UPDATE_HPP

#include <ekizu/partial_guild.hpp>

namespace ekizu {
struct GuildUpdate {
	PartialGuild guild;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildUpdate &g);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildUpdate &g);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_GUILD_UPDATE_HPP
