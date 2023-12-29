#ifndef EKIZU_REQUEST_GET_CHANNEL_MESSAGE_HPP
#define EKIZU_REQUEST_GET_CHANNEL_MESSAGE_HPP

#include <ekizu/http.hpp>
#include <ekizu/message.hpp>

namespace ekizu {
/**
 * @brief Represents the Get Channel REST API endpoint.
 */
struct GetChannelMessage {
	GetChannelMessage(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id, Snowflake message_id);

	operator net::HttpRequest() const;

	EKIZU_EXPORT Result<Message> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	Snowflake m_message_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_CHANNEL_MESSAGE_HPP
