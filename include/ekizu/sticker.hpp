#ifndef EKIZU_STICKER_HPP
#define EKIZU_STICKER_HPP

#include <ekizu/user.hpp>

namespace ekizu {
enum class StickerFormatType : uint8_t {
	/// Sticker format is a PNG.
	Png,
	/// Sticker format is an APNG.
	Apng,
	/// Sticker format is a LOTTIE.
	Lottie,
	/// Sticker format is a GIF.
	Gif,
};

enum class StickerType : uint8_t {
	/// Official sticker in a pack.
	///
	/// Part of nitro or in a removed purchasable pack.
	Standard,
	/// Sticker uploaded to a boosted guild for the guild's members.
	Guild,
};

struct Sticker {
	/// Id of the sticker.
	Snowflake id;
	/// For standard stickers, id of the pack the sticker is from.
	Snowflake pack_id;
	/// Name of the sticker.
	std::string name;
	/// Description of the sticker.
	std::optional<std::string> description;
	/// Autocomplete/suggestion tags for the sticker (max 200 characters).
	std::string tags;
	/// Type of sticker.
	StickerType type{};
	/// Type of sticker format.
	StickerFormatType format_type{};
	/// Whether this guild sticker can be used, may be false due to loss of
	/// Server Boosts.
	bool available{};
	/// Id of the guild that owns this sticker.
	Snowflake guild_id;
	/// The user that uploaded the guild sticker.
	User user;
	/// The standard sticker's sort order within its pack.
	int sort_value{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const Sticker &s);
EKIZU_EXPORT void from_json(const nlohmann::json &j, Sticker &s);
}  // namespace ekizu

#endif	// EKIZU_STICKER_HPP
