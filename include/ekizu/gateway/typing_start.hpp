#ifndef EKIZU_TYPING_START_HPP
#define EKIZU_TYPING_START_HPP

#include <ekizu/guild_member.hpp>

namespace ekizu {
struct TypingStart {
	/// ID of the channel.
	Snowflake channel_id;
	/// ID of the guild (optional).
	std::optional<Snowflake> guild_id;
	/// ID of the user.
	Snowflake user_id;
	/// Unix time (in seconds) of when the user started typing.
	uint64_t timestamp{};
	/// Member who started typing if this happened in a guild.
	std::optional<GuildMember> member;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const TypingStart &t);
EKIZU_EXPORT void from_json(const nlohmann::json &j, TypingStart &t);
}  // namespace ekizu

#endif	// EKIZU_TYPING_START_HPP
