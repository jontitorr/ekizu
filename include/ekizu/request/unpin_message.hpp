#ifndef EKIZU_REQUEST_UNPIN_MESSAGE_HPP
#define EKIZU_REQUEST_UNPIN_MESSAGE_HPP

#include <ekizu/http.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu {
/**
 * @brief Represents the Unpin Message REST API endpoint.
 */
struct UnpinMessage {
	UnpinMessage(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
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
	[[nodiscard]] EKIZU_EXPORT Result<> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	Snowflake m_message_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_UNPIN_MESSAGE_HPP
