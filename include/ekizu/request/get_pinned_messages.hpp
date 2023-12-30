#ifndef EKIZU_REQUEST_GET_PINNED_MESSAGES_HPP
#define EKIZU_REQUEST_GET_PINNED_MESSAGES_HPP

#include <ekizu/http.hpp>
#include <ekizu/message.hpp>

namespace ekizu {
struct GetPinnedMessages {
	GetPinnedMessages(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id);

	operator net::HttpRequest() const;

	[[nodiscard]] EKIZU_EXPORT Result<std::vector<Message>> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_PINNED_MESSAGES_HPP
