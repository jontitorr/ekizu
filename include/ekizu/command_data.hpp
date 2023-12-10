#ifndef EKIZU_COMMAND_DATA_HPP
#define EKIZU_COMMAND_DATA_HPP

#include <ekizu/command_data_resolved.hpp>

namespace ekizu {
struct CommandDataOption {
	/// Name of the option.
	std::string name;
	/// Value of the option.
	std::string value;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const CommandDataOption &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j, CommandDataOption &c);

enum class CommandType : uint8_t { ChatInput, User, Message, Unknown };

struct CommandData {
	/// ID of the guild the command is registered to.
	std::optional<Snowflake> guild_id;
	/// ID of the command.
	Snowflake id;
	/// Name of the command.
	std::string name;
	/// Type of the command.
	CommandType type;
	/// List of options specified by the user.
	std::vector<CommandDataOption> options;
	/// Resolved data from the interaction's options.
	std::optional<CommandInteractionDataResolved> resolved;
	/// If this is a user or message command, the ID of the targeted
	/// user/message.
	std::optional<Snowflake> target_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const CommandData &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j, CommandData &c);
}  // namespace ekizu

#endif	// EKIZU_COMMAND_DATA_HPP
