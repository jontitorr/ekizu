#ifndef EKIZU_COMMAND_DATA_HPP
#define EKIZU_COMMAND_DATA_HPP

#include <ekizu/application_command_data_resolved.hpp>

namespace ekizu {
struct CommandDataOption {
	/// Name of the option.
	std::string name;
	/// Value of the option.
	std::string value;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const CommandDataOption &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j, CommandDataOption &c);

enum class ApplicationCommandType : uint8_t {
	ChatInput,
	User,
	Message,
	Unknown
};

struct ApplicationCommandData {
	/// ID of the command.
	Snowflake id;
	/// Name of the command.
	std::string name;
	/// Type of the command.
	ApplicationCommandType type;
	/// ID of the guild the command is registered to.
	/// Resolved data from the interaction's options.
	std::optional<ApplicationCommandInteractionDataResolved> resolved;
	/// List of options specified by the user.
	std::vector<CommandDataOption> options;
	std::optional<Snowflake> guild_id;
	/// If this is a user or message command, the ID of the targeted
	/// user/message.
	std::optional<Snowflake> target_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ApplicationCommandData &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j, ApplicationCommandData &c);
}  // namespace ekizu

#endif	// EKIZU_COMMAND_DATA_HPP
