#ifndef EKIZU_GUILD_PREVIEW_HPP
#define EKIZU_GUILD_PREVIEW_HPP

#include <ekizu/emoji.hpp>
#include <ekizu/guild_feature.hpp>
#include <ekizu/sticker.hpp>

namespace ekizu {
struct GuildPreview {
	Snowflake id;
	std::string name;
	std::optional<std::string> icon;
	std::optional<std::string> splash;
	std::optional<std::string> discovery_splash;
	std::vector<Emoji> emojis;
	std::vector<GuildFeature> features;
	uint64_t approximate_member_count{};
	uint64_t approximate_presence_count{};
	std::optional<std::string> description;
	std::vector<Sticker> stickers;
};

EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildPreview &g);
EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildPreview &g);
}  // namespace ekizu

#endif	// EKIZU_GUILD_PREVIEW_HPP
