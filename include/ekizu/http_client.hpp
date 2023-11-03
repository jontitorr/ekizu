#ifndef EKIZU_HTTP_CLIENT_HPP
#define EKIZU_HTTP_CLIENT_HPP

#include <ekizu/rate_limiter.hpp>
#include <ekizu/request/create_message.hpp>

namespace ekizu
{
struct HttpClient {
	explicit HttpClient(std::string_view token);

	[[nodiscard]] CreateMessage create_message(Snowflake channel_id);

    private:
	/// Function which sends an HTTP request. This is wrapped around a ratelimiter and passed around to other structs which need the functionality.
	[[nodiscard]] Result<net::HttpResponse> send(net::HttpRequest req);

	std::optional<net::HttpConnection> m_http;
	RateLimiter m_rate_limiter;
	std::optional<std::string> m_token;
};
} // namespace ekizu

#endif // EKIZU_HTTP_CLIENT_HPP
