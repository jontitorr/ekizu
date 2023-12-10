#ifndef EKIZU_GATEWAY_UNAVAILABLE_GUILD_HPP
#define EKIZU_GATEWAY_UNAVAILABLE_GUILD_HPP

#include <ekizu/snowflake.hpp>

namespace ekizu {
struct UnavailableGuild {
	Snowflake id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const UnavailableGuild &u);
EKIZU_EXPORT void from_json(const nlohmann::json &j, UnavailableGuild &u);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_UNAVAILABLE_GUILD_HPP
