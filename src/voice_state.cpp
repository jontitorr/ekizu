#include <ekizu/json_util.hpp>
#include <ekizu/voice_state.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const VoiceState &v) {
	serialize(j, "guild_id", v.guild_id);
	serialize(j, "channel_id", v.channel_id);
	serialize(j, "user_id", v.user_id);
	serialize(j, "member", v.member);
	serialize(j, "session_id", v.session_id);
	serialize(j, "deaf", v.deaf);
	serialize(j, "mute", v.mute);
	serialize(j, "self_deaf", v.self_deaf);
	serialize(j, "self_mute", v.self_mute);
	serialize(j, "self_stream", v.self_stream);
	serialize(j, "self_video", v.self_video);
	serialize(j, "suppress", v.suppress);
	serialize(j, "request_to_speak_timestamp", v.request_to_speak_timestamp);
}

void from_json(const nlohmann::json &j, VoiceState &v) {
	deserialize(j, "guild_id", v.guild_id);
	deserialize(j, "channel_id", v.channel_id);
	deserialize(j, "user_id", v.user_id);
	deserialize(j, "member", v.member);
	deserialize(j, "session_id", v.session_id);
	deserialize(j, "deaf", v.deaf);
	deserialize(j, "mute", v.mute);
	deserialize(j, "self_deaf", v.self_deaf);
	deserialize(j, "self_mute", v.self_mute);
	deserialize(j, "self_stream", v.self_stream);
	deserialize(j, "self_video", v.self_video);
	deserialize(j, "suppress", v.suppress);
	deserialize(j, "request_to_speak_timestamp", v.request_to_speak_timestamp);
}
}  // namespace ekizu
