#ifndef EKIZU_GATEWAY_INTEGRATION_CREATE_HPP
#define EKIZU_GATEWAY_INTEGRATION_CREATE_HPP

#include <ekizu/guild_interaction.hpp>

namespace ekizu
{
struct IntegrationCreate {
	GuildIntegration integration;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const IntegrationCreate &i);
EKIZU_EXPORT void from_json(const nlohmann::json &j, IntegrationCreate &i);
} // namespace ekizu

#endif // EKIZU_GATEWAY_INTEGRATION_CREATE_HPP
