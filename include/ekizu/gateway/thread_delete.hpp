#ifndef EKIZU_GATEWAY_THREAD_DELETE_HPP
#define EKIZU_GATEWAY_THREAD_DELETE_HPP

#include <ekizu/channel.hpp>

namespace ekizu {
struct ThreadDelete {
	/// The id of the deleted thread.
	Snowflake id;
	/// The id of the guild the thread belonged to.
	Snowflake guild_id;
	/// The id of the parent channel the thread belonged to.
	Snowflake parent_id;
	/// The type of the thread.
	ChannelType type{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ThreadDelete &t);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ThreadDelete &t);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_THREAD_DELETE_HPP
