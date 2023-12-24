#ifndef EKIZU_HTTP_CLIENT_HPP
#define EKIZU_HTTP_CLIENT_HPP

#include <ekizu/http.hpp>
#include <ekizu/rate_limiter.hpp>
#include <ekizu/request/bulk_delete_messages.hpp>
#include <ekizu/request/create_message.hpp>
#include <ekizu/request/crosspost_message.hpp>
#include <ekizu/request/delete_message.hpp>
#include <ekizu/request/edit_message.hpp>
#include <ekizu/request/get_current_user.hpp>
#include <ekizu/request/get_user.hpp>
#include <ekizu/request/pin_message.hpp>
#include <ekizu/request/unpin_message.hpp>

namespace ekizu {
struct HttpClient {
	explicit HttpClient(boost::asio::io_context &ctx, std::string_view token);

	[[nodiscard]] CreateMessage create_message(Snowflake channel_id);
	[[nodiscard]] CrosspostMessage crosspost_message(Snowflake channel_id,
													 Snowflake message_id);
	[[nodiscard]] EditMessage edit_message(Snowflake channel_id,
										   Snowflake message_id);
	[[nodiscard]] DeleteMessage delete_message(Snowflake channel_id,
											   Snowflake message_id);
	[[nodiscard]] BulkDeleteMessages bulk_delete_messages(
		Snowflake channel_id, const std::vector<Snowflake> &message_ids);
	[[nodiscard]] PinMessage pin_message(Snowflake channel_id,
										 Snowflake message_id);
	[[nodiscard]] UnpinMessage unpin_message(Snowflake channel_id,
											 Snowflake message_id);
	[[nodiscard]] GetCurrentUser get_current_user();
	[[nodiscard]] GetUser get_user(Snowflake user_id);

   private:
	/// Function which sends an HTTP request. This is wrapped around a
	/// ratelimiter and passed around to other structs which need the
	/// functionality.
	[[nodiscard]] Result<void> send(net::HttpRequest req,
									std::function<void(net::HttpResponse)> cb);

	boost::asio::io_context &m_ctx;
	std::optional<net::HttpConnection> m_http;
	RateLimiter m_rate_limiter;
	std::function<void(
		net::HttpRequest, std::function<void(net::HttpResponse)>)>
		m_rate_limiter_make_request;
	std::optional<std::string> m_token;
};
}  // namespace ekizu

#endif	// EKIZU_HTTP_CLIENT_HPP
