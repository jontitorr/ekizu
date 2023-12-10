#include <ekizu/guild_interaction.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildIntegrationType &i) {
	switch (i) {
		case GuildIntegrationType::Twitch: j = "twitch"; break;
		case GuildIntegrationType::Youtube: j = "youtube"; break;
		case GuildIntegrationType::Discord: j = "discord"; break;
		case GuildIntegrationType::GuildSubscription:
			j = "guild_subscription";
			break;
		case GuildIntegrationType::Unknown: j = "unknown";
	}
}

void from_json(const nlohmann::json &j, GuildIntegrationType &i) {
	if (!j.is_string()) { return; }

	const std::string str = j;

	if (str == "twitch") {
		i = GuildIntegrationType::Twitch;
	} else if (str == "youtube") {
		i = GuildIntegrationType::Youtube;
	} else if (str == "discord") {
		i = GuildIntegrationType::Discord;
	} else if (str == "guild_subscription") {
		i = GuildIntegrationType::GuildSubscription;
	} else {
		i = GuildIntegrationType::Unknown;
	}
}

void to_json(nlohmann::json &j, const IntegrationAccount &i) {
	serialize(j, "id", i.id);
	serialize(j, "name", i.name);
}

void from_json(const nlohmann::json &j, IntegrationAccount &i) {
	deserialize(j, "id", i.id);
	deserialize(j, "name", i.name);
}

void to_json(nlohmann::json &j, const IntegrationApplication &i) {
	serialize(j, "id", i.id);
	serialize(j, "name", i.name);
	serialize(j, "icon", i.icon);
	serialize(j, "description", i.description);
	serialize(j, "bot", i.bot);
}

void from_json(const nlohmann::json &j, IntegrationApplication &i) {
	deserialize(j, "id", i.id);
	deserialize(j, "name", i.name);
	deserialize(j, "icon", i.icon);
	deserialize(j, "description", i.description);
	deserialize(j, "bot", i.bot);
}

void to_json(nlohmann::json &j, const GuildIntegration &i) {
	serialize(j, "id", i.id);
	serialize(j, "name", i.name);
	serialize(j, "type", i.type);
	serialize(j, "enabled", i.enabled);
	serialize(j, "syncing", i.syncing);
	serialize(j, "role_id", i.role_id);
	serialize(j, "enable_emoticons", i.enable_emoticons);
	serialize(j, "expire_behavior", i.expire_behavior);
	serialize(j, "expire_grace_period", i.expire_grace_period);
	serialize(j, "user", i.user);
	serialize(j, "account", i.account);
	serialize(j, "synced_at", i.synced_at);
	serialize(j, "subscriber_count", i.subscriber_count);
	serialize(j, "revoked", i.revoked);
	serialize(j, "application", i.application);
	serialize(j, "scopes", i.scopes);
}

void from_json(const nlohmann::json &j, GuildIntegration &i) {
	deserialize(j, "id", i.id);
	deserialize(j, "name", i.name);
	deserialize(j, "type", i.type);
	deserialize(j, "enabled", i.enabled);
	deserialize(j, "syncing", i.syncing);
	deserialize(j, "role_id", i.role_id);
	deserialize(j, "enable_emoticons", i.enable_emoticons);
	deserialize(j, "expire_behavior", i.expire_behavior);
	deserialize(j, "expire_grace_period", i.expire_grace_period);
	deserialize(j, "user", i.user);
	deserialize(j, "account", i.account);
	deserialize(j, "synced_at", i.synced_at);
	deserialize(j, "subscriber_count", i.subscriber_count);
	deserialize(j, "revoked", i.revoked);
	deserialize(j, "application", i.application);
	deserialize(j, "scopes", i.scopes);
}
}  // namespace ekizu
