#ifndef EKIZU_ROLE_HPP
#define EKIZU_ROLE_HPP

#include <ekizu/snowflake.hpp>
#include <optional>

namespace ekizu {
/**
 * @brief Represents a Discord role's tags.
 * @see
 * https://discord.com/developers/docs/topics/permissions#role-object-role-tags-structure
 */
struct RoleTags {
	/// The ID of the bot this role belongs to.
	Snowflake bot_id;
	/// The ID of the integration this role belongs to.
	Snowflake integration_id;
	/// Whether this is the guild's premium subscriber role.
	bool premium_subscriber{};
	/// The ID of the role's subscription listing.
	Snowflake subscription_listing_id;
	/// Whether this role is available for purchase.
	bool available_for_purchase{};
	/// Whether this is a guild's linked role.
	bool guild_connections{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const RoleTags &t);
EKIZU_EXPORT void from_json(const nlohmann::json &j, RoleTags &t);

/**
 * @brief Represents a Discord role.
 * @see https://discord.com/developers/docs/topics/permissions#role-object
 */
struct Role {
	/// The ID of the role.
	Snowflake id;
	/// The name of the role.
	std::string name;
	/// The color of the role, represented in a hexadecimal color code.
	uint32_t color{};
	/// Whether this role is displayed separately from other members.
	bool hoist{};
	/// The hash of the icon.
	std::optional<std::string> icon;
	/// Its unicode emoji.
	std::optional<std::string> unicode_emoji;
	/// The position of the role.
	uint32_t position{};
	/// The permissions of the role, as a bit set.
	uint64_t permissions{};
	/// Whether this role is managed.
	bool managed{};
	/// Whether this role is mentionable.
	bool mentionable{};
	/// The tags of the role.
	RoleTags tags;
	/// Role flags combined as a bitfield.
	uint64_t flags{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const Role &r);
EKIZU_EXPORT void from_json(const nlohmann::json &j, Role &r);
}  // namespace ekizu

#endif	// EKIZU_ROLE_HPP
