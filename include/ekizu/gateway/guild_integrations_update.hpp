#ifndef EKIZU_GATEWAY_GUILD_INTEGRATIONS_UPDATE_HPP
#define EKIZU_GATEWAY_GUILD_INTEGRATIONS_UPDATE_HPP

#include <ekizu/snowflake.hpp>

namespace ekizu
{
struct GuildIntegrationsUpdate {
	/// The ID of the guild.
	Snowflake guild_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildIntegrationsUpdate &i);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
			    GuildIntegrationsUpdate &i);
} // namespace ekizu

#endif // EKIZU_GATEWAY_GUILD_INTEGRATIONS_UPDATE_HPP
