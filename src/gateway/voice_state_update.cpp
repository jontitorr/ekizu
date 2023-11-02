#include <ekizu/gateway/voice_state_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
void to_json(nlohmann::json &j, const VoiceStateUpdate &v)
{
	j = v.voice_state;
}

void from_json(const nlohmann::json &j, VoiceStateUpdate &v)
{
	json_util::detail::deserialize_impl(j, v.voice_state);
}
} // namespace ekizu
