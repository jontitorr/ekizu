#ifndef EKIZU_REQUEST_GET_CHANNEL_HPP
#define EKIZU_REQUEST_GET_CHANNEL_HPP

#include <ekizu/channel.hpp>
#include <ekizu/http.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu {
/**
 * @brief Represents the Get Channel REST API endpoint.
 */
struct GetChannel {
	GetChannel(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id);

	/**
	 * @brief Converts the GetChannel to an HTTP request.
	 *
	 * @return The HTTP request.
	 */
	operator net::HttpRequest() const;

	[[nodiscard]] EKIZU_EXPORT Result<Channel> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_CHANNEL_HPP
