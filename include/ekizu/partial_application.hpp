#ifndef EKIZU_PARTIAL_APPLICATION_HPP
#define EKIZU_PARTIAL_APPLICATION_HPP

#include <ekizu/application_flags.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu
{
struct PartialApplication {
	/// The application id.
	Snowflake id;
	/// The application's public flags.
	ApplicationFlags flags{};
};

void to_json(nlohmann::json &j, const PartialApplication &a);
void from_json(const nlohmann::json &j, PartialApplication &a);
} // namespace ekizu

#endif // EKIZU_PARTIAL_APPLICATION_HPP
