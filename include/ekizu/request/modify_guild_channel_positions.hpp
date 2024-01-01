#ifndef EKIZU_REQUEST_MODIFY_GUILD_CHANNEL_POSITIONS_HPP
#define EKIZU_REQUEST_MODIFY_GUILD_CHANNEL_POSITIONS_HPP

#include <ekizu/http.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu {
struct ModifyGuildChannelPosition {
	/// Channel ID to be modified.
	Snowflake id;
	/// Sorting position of the channel (optional).
	std::optional<uint64_t> position;
	/// Syncs permission overwrites with the new parent if moving to a new
	/// category (optional).
	std::optional<bool> lock_permissions{};
	/// New parent ID for the channel that is moved (optional).
	std::optional<Snowflake> parent_id{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j,
						  const ModifyGuildChannelPosition &f);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
							ModifyGuildChannelPosition &f);

struct ModifyGuildChannelPositions {
	ModifyGuildChannelPositions(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake guild_id, std::vector<ModifyGuildChannelPosition> channels);

	operator net::HttpRequest() const;

	EKIZU_EXPORT Result<> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	std::vector<ModifyGuildChannelPosition> m_channels;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_MODIFY_GUILD_CHANNEL_POSITIONS_HPP
