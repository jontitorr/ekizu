#ifndef EKIZU_GATEWAY_READY_HPP
#define EKIZU_GATEWAY_READY_HPP

#include <array>
#include <ekizu/current_user.hpp>
#include <ekizu/guild.hpp>
#include <ekizu/partial_application.hpp>

namespace ekizu {
struct Ready {
	/// The API version.
	uint8_t v{};
	/// The client's user.
	CurrentUser user;
	/// The guilds the user is in.
	std::vector<UnavailableGuild> guilds;
	/// Used for resuming connections.
	std::string session_id;
	/// Gateway URL for resuming connections.
	std::string resume_gateway_url;
	/// Shard information associated with this session, if sent when
	/// identifying (is an array of two integers (shard_id, num_shards)).
	std::optional<std::array<uint64_t, 2> > shard_info;
	/// PartialApplication object containing the application's ID and public
	/// flags.
	PartialApplication application;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const Ready &payload);
EKIZU_EXPORT void from_json(const nlohmann::json &j, Ready &payload);
}  // namespace ekizu

#endif	// EKIZU_GATEWAY_READY_HPP
