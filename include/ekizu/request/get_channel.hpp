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
	GetChannel(const std::function<void(net::HttpRequest,
										std::function<void(net::HttpResponse)>)>
				   &make_request,
			   Snowflake channel_id);

	/**
	 * @brief Converts the GetChannel to an HTTP request.
	 *
	 * @return The HTTP request.
	 */
	operator net::HttpRequest() const;

	void send(std::function<void(Channel)> cb) const;

   private:
	Snowflake m_channel_id;
	std::function<void(
		net::HttpRequest, std::function<void(net::HttpResponse)>)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_CHANNEL_HPP
