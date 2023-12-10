#include <ekizu/command_data.hpp>
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

void to_json(nlohmann::json &j, const CommandData &c) {
	serialize(j, "guild_id", c.guild_id);
	serialize(j, "id", c.id);
	serialize(j, "name", c.name);
	serialize(j, "type", c.type);
	serialize(j, "options", c.options);
	serialize(j, "resolved", c.resolved);
	serialize(j, "target_id", c.target_id);
}

void from_json(const nlohmann::json &j, CommandData &c) {
	deserialize(j, "guild_id", c.guild_id);
	deserialize(j, "id", c.id);
	deserialize(j, "name", c.name);
	deserialize(j, "type", c.type);
	deserialize(j, "options", c.options);
	deserialize(j, "resolved", c.resolved);
	deserialize(j, "target_id", c.target_id);
}
}  // namespace ekizu
