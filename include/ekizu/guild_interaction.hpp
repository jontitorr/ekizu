#ifndef EKIZU_GATEWAY_GUILD_INTEGRATION_HPP
#define EKIZU_GATEWAY_GUILD_INTEGRATION_HPP

#include <cstdint>
#include <ekizu/user.hpp>

namespace ekizu
{
enum class GuildIntegrationType : uint8_t {
	Twitch,
	Youtube,
	Discord,
	GuildSubscription,
	Unknown,
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildIntegrationType &i);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildIntegrationType &i);

struct IntegrationAccount {
	/// Account ID.
	Snowflake id;
	/// Account name.
	std::string name;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const IntegrationAccount &i);
EKIZU_EXPORT void from_json(const nlohmann::json &j, IntegrationAccount &i);

struct IntegrationApplication {
	/// ID of the app.
	Snowflake id;
	/// Name of the app.
	std::string name;
	/// Icon hash of the app.
	std::optional<std::string> icon;
	/// Description of the app.
	std::optional<std::string> description;
	/// The bot associated with this application.
	std::optional<User> bot;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const IntegrationApplication &i);
EKIZU_EXPORT void from_json(const nlohmann::json &j, IntegrationApplication &i);

enum class IntegrationExpireBehavior : uint8_t {
	RemoveRole,
	RemoveMember,
};

struct GuildIntegration {
	/// Integration ID.
	Snowflake id;
	/// Integration name.
	std::string name;
	/// Integration type (twitch, youtube, discord, or guild_subscription).
	GuildIntegrationType type;
	/// Is this integration enabled.
	std::optional<bool> enabled;
	/// Is this integration syncing.
	std::optional<bool> syncing;
	/// ID that this integration uses for "subscribers."
	std::optional<Snowflake> role_id;
	/// Whether emoticons should be synced for this integration (twitch only currently).
	std::optional<bool> enable_emoticons;
	/// The behavior of expiring subscribers.
	std::optional<IntegrationExpireBehavior> expire_behavior;
	/// The grace period (in days) before expiring subscribers.
	std::optional<uint64_t> expire_grace_period;
	/// User for this integration.
	std::optional<User> user;
	/// Integration account information.
	IntegrationAccount account;
	/// When this integration was last synced.
	std::optional<std::string> synced_at;
	/// How many subscribers this integration has.
	std::optional<uint64_t> subscriber_count;
	/// Has this integration been revoked.
	std::optional<bool> revoked;
	/// The bot/OAuth2 application for Discord integrations.
	std::optional<IntegrationApplication> application;
	/// The scopes the application has been authorized for.
	std::optional<std::vector<std::string> > scopes;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildIntegration &i);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildIntegration &i);
} // namespace ekizu

#endif // EKIZU_GATEWAY_GUILD_INTEGRATION_HPP
