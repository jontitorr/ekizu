#ifndef EKIZU_GATEWAY_VOICE_STATE_UPDATE_HPP
#define EKIZU_GATEWAY_VOICE_STATE_UPDATE_HPP

#include <ekizu/voice_state.hpp>

namespace ekizu
{
struct VoiceStateUpdate {
	VoiceState voice_state;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const VoiceStateUpdate &v);
EKIZU_EXPORT void from_json(const nlohmann::json &j, VoiceStateUpdate &v);
} // namespace ekizu

#endif // EKIZU_GATEWAY_VOICE_STATE_UPDATE_HPP
