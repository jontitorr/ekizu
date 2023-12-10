#ifndef EKIZU_GUILD_MEMBER_HPP
#define EKIZU_GUILD_MEMBER_HPP

#include <ekizu/user.hpp>

namespace ekizu {
/**
 * @brief Represents a member of a Discord server.
 *
 * @see https://discord.com/developers/docs/resources/guild#guild-member-object
 */
struct GuildMember {
	/// The user this guild member represents.
	User user;
	/// The member's Guild nickname.
	std::optional<std::string> nick;
	/// The member's Guild avatar hash.
	std::optional<std::string> avatar;
	/// The member's Guild roles.
	std::vector<Snowflake> roles;
	/// ISO8601 timestamp showing when the member joined the guild.
	std::string joined_at;
	/// ISO8601 timestamp showing when the member started boosting the guild.
	std::optional<std::string> premium_since;
	/// Whether the member is deafened in voice channels.
	bool deaf{};
	/// Whether the member is muted in voice channels.
	bool mute{};
	/// Whether the user has not yet passed the guild's Membership Screening
	/// requirements.
	bool pending{};
	/// Total permissions of the member in the guild, including overwrites.
	std::string permissions;
	/// ISO8601 timestamp showing when the user's timeout will expire and they
	/// can communicate in the guild again, this will be null or a time in the
	/// past if the user is not currently timed out.
	std::string communication_disabled_until;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildMember &m);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildMember &m);
}  // namespace ekizu

#endif	// EKIZU_GUILD_MEMBER_HPP
