#ifndef EKIZU_REQUEST_TRIGGER_TYPING_INDICATOR_HPP
#define EKIZU_REQUEST_TRIGGER_TYPING_INDICATOR_HPP

#include <ekizu/http.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu {
struct TriggerTypingIndicator {
	TriggerTypingIndicator(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id);

	operator net::HttpRequest() const;

	EKIZU_EXPORT Result<> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_TRIGGER_TYPING_INDICATOR_HPP
