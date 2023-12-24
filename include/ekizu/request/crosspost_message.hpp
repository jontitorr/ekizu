#ifndef EKIZU_REQUEST_CROSSPOST_MESSAGE_HPP
#define EKIZU_REQUEST_CROSSPOST_MESSAGE_HPP

#include <ekizu/http.hpp>
#include <ekizu/message.hpp>

namespace ekizu {
/**
 * @brief Represents the Crosspost Message REST API endpoint.
 */
struct CrosspostMessage {
	CrosspostMessage(
		const std::function<void(net::HttpRequest,
								 std::function<void(net::HttpResponse)>)>
			&make_request,
		Snowflake channel_id, Snowflake message_id);

	/**
	 * @brief Converts the CrosspostMessage to an HTTP request.
	 *
	 * @return The HTTP request.
	 */
	operator net::HttpRequest() const;

	/**
	 * @brief Sends the CrosspostMessage request.
	 *
	 * @return The result of the request as an HTTP response.
	 */
	void send(std::function<void(Message)> cb) const;

   private:
	Snowflake m_channel_id;
	Snowflake m_message_id;
	std::function<void(
		net::HttpRequest, std::function<void(net::HttpResponse)>)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_CROSSPOST_MESSAGE_HPP
