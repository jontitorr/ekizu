#ifndef EKIZU_GATEWAY_GUILD_MEMBERS_CHUNK_HPP
#define EKIZU_GATEWAY_GUILD_MEMBERS_CHUNK_HPP

#include <ekizu/guild_member.hpp>
#include <ekizu/presence.hpp>

namespace ekizu {
struct GuildMembersChunk {
	/// ID of the guild.
	Snowflake guild_id;
	/// Set of guild members.
	std::vector<GuildMember> members;
	/// Chunk index in the expected chunks for this response (0 <= chunk_index <
	/// chunk_count).
	int chunk_index;
	/// Total number of expected chunks for this response.
	int chunk_count;
	/// When passing an invalid ID to REQUEST_GUILD_MEMBERS, it will be returned
	/// here.
	std::optional<std::vector<Snowflake> > not_found;
	/// When passing true to REQUEST_GUILD_MEMBERS, presences of the returned
	/// members will be here.
	std::optional<std::vector<Presence> > presences;
	/// Nonce used in the Guild Members Request.
	std::optional<std::string> nonce;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildMembersChunk &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j, GuildMembersChunk &c);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_GUILD_MEMBERS_CHUNK_HPP
