#ifndef EKIZU_GATEWAY_GUILD_EMOJIS_UPDATE_HPP
#define EKIZU_GATEWAY_GUILD_EMOJIS_UPDATE_HPP

#include <ekizu/snowflake.hpp>
#include <ekizu/emoji.hpp>

namespace ekizu
{
struct GuildEmojisUpdate {
	/// The ID of the guild.
	Snowflake guild_id;
	/// Array of emojis.
	std::vector<Emoji> emojis;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildEmojisUpdate &e);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildEmojisUpdate &e);
} // namespace ekizu

#endif // EKIZU_GATEWAY_GUILD_EMOJIS_UPDATE_HPP
