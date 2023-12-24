#ifndef EKIZU_REQUEST_PIN_MESSAGE_HPP
#define EKIZU_REQUEST_PIN_MESSAGE_HPP

#include <ekizu/http.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu {
/**
 * @brief Represents the Pin Message REST API endpoint.
 */
struct PinMessage {
	PinMessage(const std::function<void(net::HttpRequest,
										std::function<void(net::HttpResponse)>)>
				   &make_request,
			   Snowflake channel_id, Snowflake message_id);

	/**
	 * @brief Converts the PinMessage to an HTTP request.
	 *
	 * @return The HTTP request.
	 */
	operator net::HttpRequest() const;

	/**
	 * @brief Sends the PinMessage request.
	 *
	 * @return The result of the request as an HTTP response.
	 */
	void send(std::function<void()> cb) const;

   private:
	Snowflake m_channel_id;
	Snowflake m_message_id;
	std::function<void(
		net::HttpRequest, std::function<void(net::HttpResponse)>)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_PIN_MESSAGE_HPP
