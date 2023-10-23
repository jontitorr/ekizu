#ifndef EKIZU_GATEWAY_GUILD_DELETE_HPP
#define EKIZU_GATEWAY_GUILD_DELETE_HPP

#include <ekizu/snowflake.hpp>

namespace ekizu
{
struct GuildDelete {
	/// The ID of the guild.
	Snowflake guild_id;
	/// If `unavailable` is `None` the user was removed from the guild.
	bool unavailable{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildDelete &g);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildDelete &g);
} // namespace ekizu

#endif // EKIZU_GATEWAY_GUILD_DELETE_HPP
