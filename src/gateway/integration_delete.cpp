#include <ekizu/gateway/integration_delete.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
void to_json(nlohmann::json &j, const IntegrationDelete &i)
{
	j = i.integration;
}

void from_json(const nlohmann::json &j, IntegrationDelete &i)
{
	json_util::detail::deserialize_impl(j, i.integration);
}
} // namespace ekizu
