#ifndef EKIZU_GATEWAY_INTEGRATION_UPDATE_HPP
#define EKIZU_GATEWAY_INTEGRATION_UPDATE_HPP

#include <ekizu/guild_interaction.hpp>

namespace ekizu {
struct IntegrationUpdate {
	GuildIntegration integration;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const IntegrationUpdate &i);
EKIZU_EXPORT void from_json(const nlohmann::json &j, IntegrationUpdate &i);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_INTEGRATION_UPDATE_HPP
