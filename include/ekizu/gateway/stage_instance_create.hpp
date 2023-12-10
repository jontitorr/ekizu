#ifndef EKIZU_GATEWAY_STAGE_INSTANCE_CREATE_HPP
#define EKIZU_GATEWAY_STAGE_INSTANCE_CREATE_HPP

#include <ekizu/stage_instance.hpp>

namespace ekizu {
struct StageInstanceCreate {
	StageInstance stage_instance;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const StageInstanceCreate &s);
EKIZU_EXPORT void from_json(const nlohmann::json &j, StageInstanceCreate &s);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_STAGE_INSTANCE_CREATE_HPP
