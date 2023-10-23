#ifndef EKIZU_GATEWAY_MESSAGE_CREATE_HPP
#define EKIZU_GATEWAY_MESSAGE_CREATE_HPP

#include <ekizu/message.hpp>

namespace ekizu
{
struct MessageCreate {
	Message message;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const MessageCreate &m);
EKIZU_EXPORT void from_json(const nlohmann::json &j, MessageCreate &m);
} // namespace ekizu

#endif // EKIZU_GATEWAY_MESSAGE_CREATE_HPP
