#include <ekizu/http_client.hpp>
#include <utility>

namespace
{
constexpr uint8_t API_VERSION{ 10 };
} // namespace

namespace ekizu
{
HttpClient::HttpClient(std::string_view token)
	: m_token{ token }
{
}

CreateMessage HttpClient::create_message(Snowflake channel_id)
{
	return CreateMessage{ [this](net::HttpRequest req) {
				     return send(std::move(req));
			     },
			      channel_id };
}

Result<net::HttpResponse> HttpClient::send(net::HttpRequest req)
{
	if (!m_token) {
		return tl::make_unexpected(std::make_error_code(
			std::errc::operation_not_permitted));
	}

	if (!m_http) {
		auto conn = net::HttpConnection::connect("https://discord.com");

		if (!conn) {
			return tl::make_unexpected(conn.error());
		}

		m_http = std::move(*conn);
	}

	req.headers.emplace("Authorization", fmt::format("Bot {}", *m_token));

	req.path = fmt::format("/api/v{}/{}", API_VERSION, req.path);

	return m_http->request(req);
}
} // namespace ekizu