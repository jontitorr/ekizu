#ifndef EKIZU_UTIL_HPP
#define EKIZU_UTIL_HPP

#include <system_error>
#include <tl/expected.hpp>

namespace ekizu
{
template <typename T, typename E = std::error_code>
using Result = tl::expected<T, E>;
} // namespace ekizu

#endif // EKIZU_UTIL_HPP
