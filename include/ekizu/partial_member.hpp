#ifndef EKIZU_PARTIAL_MEMBER_HPP
#define EKIZU_PARTIAL_MEMBER_HPP

#include <ekizu/user.hpp>

namespace ekizu
{
struct PartialMember {
	/// The user this guild member represents.
	std::optional<User> user;
	/// The user's guild nickname.
	std::optional<std::string> nick;
	/// The user's guild avatar hash.
	std::optional<std::string> avatar;
	/// Array of the user's role object ids.
	std::optional<std::vector<Snowflake> > roles;
	/// ISO8601 timestamp showing when the user joined the guild.
	std::string joined_at;
	/// ISO8601 timestamp showing when the user started boosting the guild.
	std::optional<std::string> premium_since;
	/// Whether the user is deafened in voice channels.
	bool deaf{};
	/// Whether the user is muted in voice channels.
	bool mute{};
	/// Whether the user has not yet passed the guild's Membership Screening
	/// requirements.
	std::optional<bool> pending;
	/// Total permissions of the member in the guild, including overwrites.
	std::optional<std::string> permissions;
	/// ISO8601 timestamp showing when the user's timeout will expire and they
	/// can communicate in the guild again, this will be null or a time in the
	/// past if the user is not currently timed out.
	std::optional<std::string> communication_disabled_until;
};

void to_json(nlohmann::json &j, const PartialMember &m);
void from_json(const nlohmann::json &j, PartialMember &m);
} // namespace ekizu

#endif // EKIZU_PARTIAL_MEMBER_HPP
