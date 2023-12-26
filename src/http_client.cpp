#include <boost/outcome/try.hpp>
#include <ekizu/http_client.hpp>
#include <utility>

namespace ekizu {
HttpClient::HttpClient(std::string_view token)
	: m_rate_limiter{[this](net::HttpRequest req,
							const asio::yield_context &yield) {
		  return send(std::move(req), yield);
	  }},
	  m_rate_limiter_make_request{
		  [this](net::HttpRequest req, const asio::yield_context &yield) {
			  return m_rate_limiter.send({std::move(req)}, yield);
		  }},
	  m_token{token} {}

CreateMessage HttpClient::create_message(Snowflake channel_id) const {
	return CreateMessage{m_rate_limiter_make_request, channel_id};
}

CreateReaction HttpClient::create_reaction(
	Snowflake channel_id, Snowflake message_id, RequestReaction emoji) const {
	return CreateReaction{
		m_rate_limiter_make_request, channel_id, message_id, std::move(emoji)};
}

CrosspostMessage HttpClient::crosspost_message(Snowflake channel_id,
											   Snowflake message_id) const {
	return CrosspostMessage{
		m_rate_limiter_make_request, channel_id, message_id};
}

EditMessage HttpClient::edit_message(Snowflake channel_id,
									 Snowflake message_id) const {
	return EditMessage{m_rate_limiter_make_request, channel_id, message_id};
}

DeleteMessage HttpClient::delete_message(Snowflake channel_id,
										 Snowflake message_id) const {
	return DeleteMessage{m_rate_limiter_make_request, channel_id, message_id};
}

BulkDeleteMessages HttpClient::bulk_delete_messages(
	Snowflake channel_id, const std::vector<Snowflake> &message_ids) const {
	return BulkDeleteMessages{
		m_rate_limiter_make_request, channel_id, message_ids};
}

PinMessage HttpClient::pin_message(Snowflake channel_id,
								   Snowflake message_id) const {
	return PinMessage{m_rate_limiter_make_request, channel_id, message_id};
}

UnpinMessage HttpClient::unpin_message(Snowflake channel_id,
									   Snowflake message_id) const {
	return UnpinMessage{m_rate_limiter_make_request, channel_id, message_id};
}

GetCurrentUser HttpClient::get_current_user() const {
	return GetCurrentUser{m_rate_limiter_make_request};
}

GetUser HttpClient::get_user(Snowflake user_id) const {
	return GetUser{m_rate_limiter_make_request, user_id};
}

Result<net::HttpResponse> HttpClient::send(net::HttpRequest req,
										   const asio::yield_context &yield) {
	if (!m_token) { return boost::system::errc::operation_not_permitted; }

	req.set(net::http::field::authorization, fmt::format("Bot {}", *m_token));
	req.set(net::http::field::host, "discord.com");
	req.target(fmt::format("/api/v10{}", boost::to_string(req.target())));

	if (!m_http) {
		BOOST_OUTCOME_TRY(auto http, net::HttpConnection::connect(
										 "https://discord.com", yield));
		m_http = std::move(http);
	}

	return m_http->request(req, yield);
}
}  // namespace ekizu