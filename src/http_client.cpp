#include <ekizu/http_client.hpp>
#include <utility>

namespace
{
constexpr uint8_t API_VERSION{ 10 };
} // namespace

namespace ekizu
{
HttpClient::HttpClient(std::string_view token)
	: m_rate_limiter{ [this](net::HttpRequest req) {
		return send(std::move(req));
	} }
	, m_rate_limiter_make_request{ [this](net::HttpRequest req) {
		return m_rate_limiter.send({ std::move(req) });
	} }
	, m_token{ token }
{
}

CreateMessage HttpClient::create_message(Snowflake channel_id)
{
	return CreateMessage{ m_rate_limiter_make_request, channel_id };
}

CrosspostMessage HttpClient::crosspost_message(Snowflake channel_id,
					       Snowflake message_id)
{
	return CrosspostMessage{ m_rate_limiter_make_request, channel_id,
				 message_id };
}

EditMessage HttpClient::edit_message(Snowflake channel_id, Snowflake message_id)
{
	return EditMessage{ m_rate_limiter_make_request, channel_id,
			    message_id };
}

DeleteMessage HttpClient::delete_message(Snowflake channel_id,
					 Snowflake message_id)
{
	return DeleteMessage{ m_rate_limiter_make_request, channel_id,
			      message_id };
}

BulkDeleteMessages
HttpClient::bulk_delete_messages(Snowflake channel_id,
				 const std::vector<Snowflake> &message_ids)
{
	return BulkDeleteMessages{ m_rate_limiter_make_request, channel_id,
				   message_ids };
}

PinMessage HttpClient::pin_message(Snowflake channel_id, Snowflake message_id)
{
	return PinMessage{ m_rate_limiter_make_request, channel_id,
			   message_id };
}

UnpinMessage HttpClient::unpin_message(Snowflake channel_id,
				       Snowflake message_id)
{
	return UnpinMessage{ m_rate_limiter_make_request, channel_id,
			     message_id };
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

	auto res = m_http->request(req);

	if (!res && res.error() == net::SslError::ZeroReturn) {
		auto conn = net::HttpConnection::connect("https://discord.com");

		if (!conn) {
			return tl::make_unexpected(conn.error());
		}

		m_http = std::move(*conn);
		res = m_http->request(req);
	}

	return res;
}
} // namespace ekizu