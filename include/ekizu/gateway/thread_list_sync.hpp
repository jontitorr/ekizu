#ifndef EKIZU_GATEWAY_THREAD_LIST_SYNC_HPP
#define EKIZU_GATEWAY_THREAD_LIST_SYNC_HPP

#include <ekizu/channel.hpp>

namespace ekizu
{
struct ThreadListSync {
	/// ID of the guild.
	Snowflake guild_id;
	/// Parent channel IDs whose threads are being synced.
	std::optional<std::vector<Snowflake> > channel_ids;
	/// All active threads in the given channels that the current user can access.
	std::vector<Channel> threads;
	/// All thread member objects from the synced threads for the current user.
	std::vector<ThreadMember> members;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ThreadListSync &t);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ThreadListSync &t);
} // namespace ekizu

#endif // EKIZU_GATEWAY_THREAD_LIST_SYNC_HPP
