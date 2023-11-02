#include <ekizu/interaction.hpp>

namespace ekizu
{
struct InteractionCreate {
	Interaction interaction;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const InteractionCreate &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j, InteractionCreate &c);
} // namespace ekizu