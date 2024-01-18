#ifndef EKIZU_INTERACTION_HPP
#define EKIZU_INTERACTION_HPP

#include <ekizu/application_command_data.hpp>
#include <ekizu/message.hpp>
#include <ekizu/message_component_data.hpp>

namespace ekizu {
using InteractionData =
	std::variant<ApplicationCommandData, MessageComponentData>;

EKIZU_EXPORT void to_json(nlohmann::json &j, const InteractionData &i);

struct Interaction {
	/// ID of the interaction.
	Snowflake id;
	/// ID of the application this interaction is for.
	Snowflake application_id;
	/// Type of interaction.
	InteractionType type;
	/// Interaction data payload.
	std::optional<InteractionData> data;
	/// Guild that the interaction was sent from.
	std::optional<Snowflake> guild_id;
	/// Channel that the interaction was sent from.
	std::optional<Channel> channel;
	/// Channel that the interaction was sent from (deprecated).
	std::optional<Snowflake> channel_id;
	/// Guild member data for the invoking user, including permissions.
	std::optional<GuildMember> member;
	/// User object for the invoking user, if invoked in a DM.
	std::optional<User> user;
	/// Continuation token for responding to the interaction.
	std::string token;
	/// Read-only property, always 1.
	int version;
	/// For components, the message they were attached to.
	std::optional<Message> message;
	/// Bitwise set of permissions the app or bot has within the channel the
	/// interaction was sent from.
	std::optional<Permissions> app_permissions;
	/// Selected language of the invoking user.
	std::optional<std::string> locale;
	/// Guild's preferred locale, if invoked in a guild.
	std::optional<std::string> guild_locale;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const Interaction &i);
EKIZU_EXPORT void from_json(const nlohmann::json &j, Interaction &i);
}  // namespace ekizu

#endif	// EKIZU_INTERACTION_HPP
