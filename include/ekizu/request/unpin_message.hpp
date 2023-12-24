#ifndef EKIZU_REQUEST_UNPIN_MESSAGE_HPP
#define EKIZU_REQUEST_UNPIN_MESSAGE_HPP

#include <ekizu/http.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu {
/**
 * @brief Represents the Unpin Message REST API endpoint.
 */
struct UnpinMessage {
	UnpinMessage(const std::function<void(
					 net::HttpRequest, std::function<void(net::HttpResponse)>)>
					 &make_request,
				 Snowflake channel_id, Snowflake message_id);

	/**
	 * @brief Converts the UnpinMessage to an HTTP request.
	 *
	 * @return The HTTP request.
	 */
	operator net::HttpRequest() const;

	/**
	 * @brief Sends the UnpinMessage request.
	 *
	 * @return The result of the request as an HTTP response.
	 */
	[[nodiscard]] Result<void> send(
		std::function<void(net::HttpResponse)> cb) const;

   private:
	Snowflake m_channel_id;
	Snowflake m_message_id;
	std::function<void(
		net::HttpRequest, std::function<void(net::HttpResponse)>)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_UNPIN_MESSAGE_HPP
