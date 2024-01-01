#ifndef EKIZU_REQUEST_GET_GUILD_CHANNELS_HPP
#define EKIZU_REQUEST_GET_GUILD_CHANNELS_HPP

#include <ekizu/channel.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
/**
 * @brief Represents the Get Guild Channels API request.
 */
struct GetGuildChannels {
	/**
	 * @brief Constructor for GetGuildChannels.
	 * @param make_request The function to make the HTTP request.
	 * @param guild_id The ID of the guild.
	 */
	GetGuildChannels(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake guild_id);

	/**
	 * @brief Converts the request to an HTTP request.
	 * @return The HTTP request.
	 */
	operator net::HttpRequest() const;

	/**
	 * @brief Sends the API request to get guild channels.
	 * @param yield The coroutine yield context.
	 * @return Result containing the list of guild channels on success.
	 */
	EKIZU_EXPORT Result<std::vector<Channel>> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_GUILD_CHANNELS_HPP
