#ifndef EKIZU_UTIL_HPP
#define EKIZU_UTIL_HPP

// NOTE: No exceptions should be the goal.
// #define BOOST_NO_EXCEPTIONS
#include <boost/outcome/result.hpp>

namespace ekizu {
template <typename T>
using Result = boost::outcome_v2::result<T>;
// using Result = boost::outcome_v2::result<T, boost::system::error_code,
// 										 boost::outcome_v2::policy::terminate>;
namespace outcome = boost::outcome_v2;
}  // namespace ekizu

#endif	// EKIZU_UTIL_HPP
