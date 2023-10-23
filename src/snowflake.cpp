#include <ekizu/snowflake.hpp>
#include <nlohmann/json.hpp>

namespace ekizu
{
void to_json(nlohmann::json &j, const Snowflake &s)
{
	j = s.id;
}

void from_json(const nlohmann::json &j, Snowflake &s)
{
	// If using an inconsistent API version such as v6, you usually get various
	// formats of Snowflakes, such as some being uint64_t and others being
	// string. In v10, this is no longer the case, but the code will remain
	// compatible.
	if (j.is_string()) {
		if (const std::string str = j;
		    !str.empty() &&
		    std::all_of(str.begin(), str.end(), [](unsigned char c) {
			    return std::isdigit(c);
		    })) {
			s.id = std::stoull(str);
		}
	} else if (j.is_number_unsigned()) {
		s.id = j;
	}
}
} // namespace ekizu
