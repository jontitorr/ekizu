#ifndef EKIZU_HTTP_CLIENT_HPP
#define EKIZU_HTTP_CLIENT_HPP

#include <ekizu/request/create_message.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu
{
struct HttpClient {
	explicit HttpClient(std::string_view token);

	[[nodiscard]] CreateMessage create_message(Snowflake channel_id);

    private:
	[[nodiscard]] Result<net::HttpResponse> send(net::HttpRequest req);

	std::optional<net::HttpConnection> m_http;
	std::optional<std::string> m_token;
};
} // namespace ekizu

#endif // EKIZU_HTTP_CLIENT_HPP
