#ifndef EKIZU_GATEWAY_INTEGRATION_DELETE_HPP
#define EKIZU_GATEWAY_INTEGRATION_DELETE_HPP

#include <ekizu/guild_interaction.hpp>

namespace ekizu
{
struct IntegrationDelete {
	GuildIntegration integration;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const IntegrationDelete &i);
EKIZU_EXPORT void from_json(const nlohmann::json &j, IntegrationDelete &i);
} // namespace ekizu

#endif // EKIZU_GATEWAY_INTEGRATION_DELETE_HPP
