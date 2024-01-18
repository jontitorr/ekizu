#include <ekizu/application_command_data.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const CommandDataOption &c) {
	serialize(j, "name", c.name);
	serialize(j, "value", c.value);
}

void from_json(const nlohmann::json &j, CommandDataOption &c) {
	deserialize(j, "name", c.name);
	deserialize(j, "value", c.value);
}

void to_json(nlohmann::json &j, const ApplicationCommandData &c) {
	serialize(j, "id", c.id);
	serialize(j, "name", c.name);
	serialize(j, "type", c.type);
	serialize(j, "resolved", c.resolved);
	serialize(j, "options", c.options);
	serialize(j, "guild_id", c.guild_id);
	serialize(j, "target_id", c.target_id);
}

void from_json(const nlohmann::json &j, ApplicationCommandData &c) {
	deserialize(j, "id", c.id);
	deserialize(j, "name", c.name);
	deserialize(j, "type", c.type);
	deserialize(j, "resolved", c.resolved);
	deserialize(j, "options", c.options);
	deserialize(j, "guild_id", c.guild_id);
	deserialize(j, "target_id", c.target_id);
}
}  // namespace ekizu
