#ifndef EKIZU_GATEWAY_THREAD_UPDATE_HPP
#define EKIZU_GATEWAY_THREAD_UPDATE_HPP

#include <ekizu/channel.hpp>

namespace ekizu {
struct ThreadUpdate {
	Channel thread;
};

EKIZU_EXPORT void from_json(const nlohmann::json &j, ThreadUpdate &t);
EKIZU_EXPORT void to_json(nlohmann::json &j, const ThreadUpdate &t);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_THREAD_UPDATE_HPP
