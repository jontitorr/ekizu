#ifndef EKIZU_LOG_LEVEL_HPP
#define EKIZU_LOG_LEVEL_HPP

#include <cstdint>

namespace ekizu
{
enum class LogLevel : uint8_t { Trace, Debug, Info, Warn, Error, Critical };
} // namespace ekizu

#endif // EKIZU_LOG_LEVEL_HPP
