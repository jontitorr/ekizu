#ifndef EKIZU_MESSAGE_COMPONENT_DATA_HPP
#define EKIZU_MESSAGE_COMPONENT_DATA_HPP

#include <ekizu/message_component.hpp>

namespace ekizu {
struct MessageComponentData {
	std::string custom_id;
	ComponentType type;
	std::vector<std::string> values;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const MessageComponentData &d);
EKIZU_EXPORT void from_json(const nlohmann::json &j, MessageComponentData &d);
}  // namespace ekizu

#endif	// EKIZU_MESSAGE_COMPONENT_DATA_HPP
