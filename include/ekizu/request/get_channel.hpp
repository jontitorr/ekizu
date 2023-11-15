#ifndef EKIZU_REQUEST_GET_CHANNEL_HPP
#define EKIZU_REQUEST_GET_CHANNEL_HPP

#include <ekizu/channel.hpp>
#include <ekizu/snowflake.hpp>
#include <net/http.hpp>

namespace ekizu
{
/**
 * @brief Represents the Get Channel REST API endpoint.
 */
struct GetChannel {
	GetChannel(
		const std::function<Result<net::HttpResponse>(net::HttpRequest)>
			&make_request,
		Snowflake channel_id);

	/**
	 * @brief Converts the GetChannel to an HTTP request.
	 *
	 * @return The HTTP request.
	 */
	operator net::HttpRequest() const;

	/**
	 * @brief Sends the GetChannel request.
	 *
	 * @return The result of the request as an HTTP response.
	 */
	[[nodiscard]] Result<Channel> send() const;

    private:
	Snowflake m_channel_id;
	std::function<Result<net::HttpResponse>(net::HttpRequest)>
		m_make_request;
};
} // namespace ekizu

#endif // EKIZU_REQUEST_GET_CHANNEL_HPP
