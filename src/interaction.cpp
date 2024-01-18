#include <ekizu/interaction.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const InteractionData &i) {
	std::visit([&j](auto &v) { to_json(j, v); }, i);
}

void to_json(nlohmann::json &j, const Interaction &i) {
	serialize(j, "id", i.id);
	serialize(j, "application_id", i.application_id);
	serialize(j, "type", i.type);
	serialize(j, "data", i.data);
	serialize(j, "guild_id", i.guild_id);
	serialize(j, "channel", i.channel);
	serialize(j, "channel_id", i.channel_id);
	serialize(j, "member", i.member);
	serialize(j, "user", i.user);
	serialize(j, "token", i.token);
	serialize(j, "version", i.version);
	serialize(j, "message", i.message);
	serialize(j, "app_permissions", i.app_permissions);
	serialize(j, "locale", i.locale);
	serialize(j, "guild_locale", i.guild_locale);
}

void from_json(const nlohmann::json &j, Interaction &i) {
	deserialize(j, "id", i.id);
	deserialize(j, "application_id", i.application_id);
	deserialize(j, "type", i.type);

	if (json_util::not_null_all(j, "type", "data")) {
		switch (j["type"].get<InteractionType>()) {
			case InteractionType::ApplicationCommand:
			case InteractionType::ApplicationCommandAutocomplete:
				i.data = j["data"].get<ApplicationCommandData>();
				break;
			case InteractionType::MessageComponent:
				i.data = j["data"].get<MessageComponentData>();
				break;
		}
	}

	deserialize(j, "guild_id", i.guild_id);
	deserialize(j, "channel", i.channel);
	deserialize(j, "channel_id", i.channel_id);
	deserialize(j, "member", i.member);
	deserialize(j, "user", i.user);
	deserialize(j, "token", i.token);
	deserialize(j, "version", i.version);
	deserialize(j, "message", i.message);
	deserialize(j, "app_permissions", i.app_permissions);
	deserialize(j, "locale", i.locale);
	deserialize(j, "guild_locale", i.guild_locale);
}
}  // namespace ekizu
