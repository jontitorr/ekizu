#include <ekizu/gateway/stage_instance_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
void to_json(nlohmann::json &j, const StageInstanceUpdate &s)
{
	j = s.stage_instance;
}

void from_json(const nlohmann::json &j, StageInstanceUpdate &s)
{
	json_util::detail::deserialize_impl(j, s.stage_instance);
}
} // namespace ekizu
