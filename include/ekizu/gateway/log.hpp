#ifndef EKIZU_GATEWAY_LOG_HPP
#define EKIZU_GATEWAY_LOG_HPP

#include <ekizu/log_level.hpp>
#include <string>

namespace ekizu
{
struct Log {
	LogLevel level;
	std::string message;
};
} // namespace ekizu

#endif // EKIZU_GATEWAY_LOG_HPP
