#ifndef EKIZU_GATEWAY_STAGE_INSTANCE_DELETE_HPP
#define EKIZU_GATEWAY_STAGE_INSTANCE_DELETE_HPP

#include <ekizu/stage_instance.hpp>

namespace ekizu
{
struct StageInstanceDelete {
	StageInstance stage_instance;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const StageInstanceDelete &s);
EKIZU_EXPORT void from_json(const nlohmann::json &j, StageInstanceDelete &s);
} // namespace ekizu

#endif // EKIZU_GATEWAY_STAGE_INSTANCE_DELETE_HPP
