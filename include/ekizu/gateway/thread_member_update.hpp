#ifndef EKIZU_GATEWAY_THREAD_MEMBER_UPDATE_HPP
#define EKIZU_GATEWAY_THREAD_MEMBER_UPDATE_HPP

#include <ekizu/channel.hpp>

namespace ekizu {
struct ThreadMemberUpdate {
	ThreadMember member;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ThreadMemberUpdate &m);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ThreadMemberUpdate &m);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_THREAD_MEMBER_UPDATE_HPP
