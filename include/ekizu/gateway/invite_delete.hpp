#ifndef EKIZU_MODELS_INVITE_DELETE_HPP
#define EKIZU_MODELS_INVITE_DELETE_HPP

#include <ekizu/snowflake.hpp>

namespace ekizu {
struct InviteDelete {
	/// ID of the channel the invite was deleted from.
	Snowflake channel_id;
	/// Unique code of the deleted invite.
	std::string code;
	/// ID of the guild where the invite was deleted.
	Snowflake guild_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const InviteDelete &invite);
EKIZU_EXPORT void from_json(const nlohmann::json &j, InviteDelete &invite);
}  // namespace ekizu

#endif	// EKIZU_MODELS_INVITE_DELETE_HPP
