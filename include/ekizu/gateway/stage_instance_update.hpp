#ifndef EKIZU_GATEWAY_STAGE_INSTANCE_UPDATE_HPP
#define EKIZU_GATEWAY_STAGE_INSTANCE_UPDATE_HPP

#include <ekizu/stage_instance.hpp>

namespace ekizu
{
struct StageInstanceUpdate {
	StageInstance stage_instance;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const StageInstanceUpdate &s);
EKIZU_EXPORT void from_json(const nlohmann::json &j, StageInstanceUpdate &s);
} // namespace ekizu

#endif // EKIZU_GATEWAY_STAGE_INSTANCE_UPDATE_HPP
