#ifndef EKIZU_GATEWAY_PRESENCE_UPDATE_HPP
#define EKIZU_GATEWAY_PRESENCE_UPDATE_HPP

#include <ekizu/presence.hpp>

namespace ekizu {
struct PresenceUpdate {
	Presence presence;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const PresenceUpdate &payload);
EKIZU_EXPORT void from_json(const nlohmann::json &j, PresenceUpdate &payload);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_PRESENCE_UPDATE_HPP
