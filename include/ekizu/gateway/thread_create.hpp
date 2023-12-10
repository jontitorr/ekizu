#ifndef EKIZU_GATEWAY_THREAD_CREATE_HPP
#define EKIZU_GATEWAY_THREAD_CREATE_HPP

#include <ekizu/channel.hpp>

namespace ekizu {
struct ThreadCreate {
	Channel thread;
};

EKIZU_EXPORT void from_json(const nlohmann::json &j, ThreadCreate &t);
EKIZU_EXPORT void to_json(nlohmann::json &j, const ThreadCreate &t);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_THREAD_CREATE_HPP
