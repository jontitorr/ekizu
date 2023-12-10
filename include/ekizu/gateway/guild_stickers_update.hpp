#ifndef EKIZU_GATEWAY_GUILD_STICKERS_UPDATE_HPP
#define EKIZU_GATEWAY_GUILD_STICKERS_UPDATE_HPP

#include <ekizu/sticker.hpp>

namespace ekizu {
struct GuildStickersUpdate {
	/// The ID of the guild where stickers were updated.
	Snowflake guild_id;
	/// Array of stickers that were updated.
	std::vector<Sticker> stickers;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildStickersUpdate &u);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildStickersUpdate &u);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_GUILD_STICKERS_UPDATE_HPP
