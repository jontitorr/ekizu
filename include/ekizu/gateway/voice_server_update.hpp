#ifndef EKIZU_GATEWAY_VOICE_SERVER_UPDATE_HPP
#define EKIZU_GATEWAY_VOICE_SERVER_UPDATE_HPP

#include <ekizu/snowflake.hpp>
#include <optional>
#include <string>

namespace ekizu
{
struct VoiceServerUpdate {
	/// Voice connection token.
	std::string token;
	/// Guild this voice server update is for.
	Snowflake guild_id;
	/// Voice server host (optional). A null endpoint indicates that the voice server allocated has gone away.
	std::optional<std::string> endpoint;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const VoiceServerUpdate &v);
EKIZU_EXPORT void from_json(const nlohmann::json &j, VoiceServerUpdate &v);
} // namespace ekizu

#endif // EKIZU_GATEWAY_VOICE_SERVER_UPDATE_HPP
