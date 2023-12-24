#include <boost/outcome/try.hpp>
#include <ekizu/http_client.hpp>

namespace ekizu {
HttpClient::HttpClient(boost::asio::io_context &ctx, std::string_view token)
	: m_ctx{ctx},
	  m_rate_limiter{[this](net::HttpRequest req,
							std::function<void(net::HttpResponse)> cb) {
		  return send(std::move(req), std::move(cb));
	  }},
	  m_rate_limiter_make_request{
		  [this](
			  net::HttpRequest req, std::function<void(net::HttpResponse)> cb) {
			  return m_rate_limiter.send({std::move(req)}, std::move(cb));
		  }},
	  m_token{token} {}

CreateMessage HttpClient::create_message(Snowflake channel_id) {
	return CreateMessage{m_rate_limiter_make_request, channel_id};
}

CrosspostMessage HttpClient::crosspost_message(Snowflake channel_id,
											   Snowflake message_id) {
	return CrosspostMessage{
		m_rate_limiter_make_request, channel_id, message_id};
}

EditMessage HttpClient::edit_message(Snowflake channel_id,
									 Snowflake message_id) {
	return EditMessage{m_rate_limiter_make_request, channel_id, message_id};
}

DeleteMessage HttpClient::delete_message(Snowflake channel_id,
										 Snowflake message_id) {
	return DeleteMessage{m_rate_limiter_make_request, channel_id, message_id};
}

BulkDeleteMessages HttpClient::bulk_delete_messages(
	Snowflake channel_id, const std::vector<Snowflake> &message_ids) {
	return BulkDeleteMessages{
		m_rate_limiter_make_request, channel_id, message_ids};
}

PinMessage HttpClient::pin_message(Snowflake channel_id, Snowflake message_id) {
	return PinMessage{m_rate_limiter_make_request, channel_id, message_id};
}

UnpinMessage HttpClient::unpin_message(Snowflake channel_id,
									   Snowflake message_id) {
	return UnpinMessage{m_rate_limiter_make_request, channel_id, message_id};
}

GetCurrentUser HttpClient::get_current_user() {
	return GetCurrentUser{m_rate_limiter_make_request};
}

GetUser HttpClient::get_user(Snowflake user_id) {
	return GetUser{m_rate_limiter_make_request, user_id};
}

Result<void> HttpClient::send(net::HttpRequest req,
							  std::function<void(net::HttpResponse)> cb) {
	if (!m_token) { return boost::system::errc::operation_not_permitted; }

	req.set(net::http::field::authorization, fmt::format("Bot {}", *m_token));
	req.set(net::http::field::host, "discord.com");
	req.target(fmt::format("/api/v10{}", boost::to_string(req.target())));

	if (m_http) { return m_http->request(req, std::move(cb)); }

	BOOST_OUTCOME_TRY(
		auto http,
		net::HttpConnection::connect(
			m_ctx, "https://discord.com",
			[this, req = std::move(req), cb = std::move(cb)]() mutable {
				boost::ignore_unused(m_http->request(req, std::move(cb)));
			}));

	m_http = std::move(http);
	return outcome::success();
}
}  // namespace ekizu