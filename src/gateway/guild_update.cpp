#include <ekizu/gateway/guild_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
void to_json(nlohmann::json &j, const GuildUpdate &g)
{
	j = g.guild;
}

void from_json(const nlohmann::json &j, GuildUpdate &g)
{
	json_util::detail::deserialize_impl(j, g.guild);
}
} // namespace ekizu
