#include <ekizu/json_util.hpp>
#include <ekizu/sticker.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const Sticker &s)
{
	serialize(j, "id", s.id);
	serialize(j, "pack_id", s.pack_id);
	serialize(j, "name", s.name);
	serialize(j, "description", s.description);
	serialize(j, "tags", s.tags);
	serialize(j, "type", s.type);
	serialize(j, "format_type", s.format_type);
	serialize(j, "available", s.available);
	serialize(j, "guild_id", s.guild_id);
	serialize(j, "sort_value", s.sort_value);
	serialize(j, "user", s.user);
}

void from_json(const nlohmann::json &j, Sticker &s)
{
	deserialize(j, "id", s.id);
	deserialize(j, "pack_id", s.pack_id);
	deserialize(j, "name", s.name);
	deserialize(j, "description", s.description);
	deserialize(j, "tags", s.tags);
	deserialize(j, "type", s.type);
	deserialize(j, "format_type", s.format_type);
	deserialize(j, "available", s.available);
	deserialize(j, "guild_id", s.guild_id);
	deserialize(j, "sort_value", s.sort_value);
	deserialize(j, "user", s.user);
}
} // namespace ekizu
