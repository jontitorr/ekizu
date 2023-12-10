#ifndef EKIZU_THREAD_MEMBERS_UPDATE_HPP
#define EKIZU_THREAD_MEMBERS_UPDATE_HPP

#include <ekizu/channel.hpp>

namespace ekizu {
struct ThreadMembersUpdate {
	/// ID of the thread.
	Snowflake id;
	/// ID of the guild.
	Snowflake guild_id;
	/// Approximate number of members in the thread, capped at 50.
	uint8_t member_count;
	/// Users who were added to the thread.
	std::optional<std::vector<ThreadMember> > added_members;
	/// ID of the users who were removed from the thread.
	std::vector<Snowflake> removed_member_ids;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ThreadMembersUpdate &t);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ThreadMembersUpdate &t);
}  // namespace ekizu

#endif	// EKIZU_THREAD_MEMBERS_UPDATE_HPP
