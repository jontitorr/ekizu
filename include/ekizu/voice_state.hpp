#ifndef EKIZU_VOICE_STATE_HPP
#define EKIZU_VOICE_STATE_HPP

#include <ekizu/guild_member.hpp>

namespace ekizu {
struct VoiceState {
	/// The guild id this voice state is for (optional).
	std::optional<Snowflake> guild_id;
	/// The channel id this user is connected to (optional).
	std::optional<Snowflake> channel_id;
	/// The user id this voice state is for.
	Snowflake user_id;
	/// The guild member this voice state is for.
	std::optional<GuildMember> member;
	/// The session id for this voice state.
	std::string session_id;
	/// Whether this user is deafened by the server.
	bool deaf;
	/// Whether this user is muted by the server.
	bool mute;
	/// Whether this user is locally deafened.
	bool self_deaf;
	/// Whether this user is locally muted.
	bool self_mute;
	/// Whether this user is streaming using "Go Live" (optional).
	std::optional<bool> self_stream;
	/// Whether this user's camera is enabled.
	bool self_video;
	/// Whether this user's permission to speak is denied.
	bool suppress;
	/// The time at which the user requested to speak (optional).
	std::optional<std::string> request_to_speak_timestamp;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const VoiceState &v);
EKIZU_EXPORT void from_json(const nlohmann::json &j, VoiceState &v);
}  // namespace ekizu

#endif	// EKIZU_VOICE_STATE_HPP
