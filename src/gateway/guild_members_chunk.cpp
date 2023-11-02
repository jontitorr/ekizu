#include <ekizu/gateway/guild_members_chunk.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const GuildMembersChunk &c)
{
	serialize(j, "guild_id", c.guild_id);
	serialize(j, "members", c.members);
	serialize(j, "chunk_index", c.chunk_index);
	serialize(j, "chunk_count", c.chunk_count);
	serialize(j, "not_found", c.not_found);
	serialize(j, "presences", c.presences);
	serialize(j, "nonce", c.nonce);
}

void from_json(const nlohmann::json &j, GuildMembersChunk &c)
{
	deserialize(j, "guild_id", c.guild_id);
	deserialize(j, "members", c.members);
	deserialize(j, "chunk_index", c.chunk_index);
	deserialize(j, "chunk_count", c.chunk_count);
	deserialize(j, "not_found", c.not_found);
	deserialize(j, "presences", c.presences);
	deserialize(j, "nonce", c.nonce);
}
} // namespace ekizu
