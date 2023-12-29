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

GetChannel HttpClient::get_channel(Snowflake channel_id) const {
	return GetChannel{m_rate_limiter_make_request, channel_id};
}

ModifyChannel HttpClient::modify_channel(Snowflake channel_id) const {
	return ModifyChannel{m_rate_limiter_make_request, channel_id};
}

DeleteChannel HttpClient::delete_channel(Snowflake channel_id) const {
	return DeleteChannel{m_rate_limiter_make_request, channel_id};
}

GetChannelMessages HttpClient::get_channel_messages(
	Snowflake channel_id) const {
	return GetChannelMessages{m_rate_limiter_make_request, channel_id};
}

GetChannelMessage HttpClient::get_channel_message(Snowflake channel_id,
												  Snowflake message_id) const {
	return GetChannelMessage{
		m_rate_limiter_make_request, channel_id, message_id};
}

CreateMessage HttpClient::create_message(Snowflake channel_id) const {
	return CreateMessage{m_rate_limiter_make_request, channel_id};
}

CrosspostMessage HttpClient::crosspost_message(Snowflake channel_id,
											   Snowflake message_id) const {
	return CrosspostMessage{
		m_rate_limiter_make_request, channel_id, message_id};
}

CreateReaction HttpClient::create_reaction(
	Snowflake channel_id, Snowflake message_id, RequestReaction emoji) const {
	return CreateReaction{
		m_rate_limiter_make_request, channel_id, message_id, std::move(emoji)};
}

DeleteOwnReaction HttpClient::delete_own_reaction(
	Snowflake channel_id, Snowflake message_id, RequestReaction emoji) const {
	return DeleteOwnReaction{
		m_rate_limiter_make_request, channel_id, message_id, std::move(emoji)};
}

DeleteUserReaction HttpClient::delete_user_reaction(
	Snowflake channel_id, Snowflake message_id, RequestReaction emoji,
	Snowflake user_id) const {
	return DeleteUserReaction{m_rate_limiter_make_request, channel_id,
							  message_id, std::move(emoji), user_id};
}

GetReactions HttpClient::get_reactions(
	Snowflake channel_id, Snowflake message_id, RequestReaction emoji) const {
	return GetReactions{
		m_rate_limiter_make_request, channel_id, message_id, std::move(emoji)};
}

DeleteAllReactions HttpClient::delete_all_reactions(
	Snowflake channel_id, Snowflake message_id) const {
	return DeleteAllReactions{
		m_rate_limiter_make_request, channel_id, message_id};
}

DeleteAllReactionsForEmoji HttpClient::delete_all_reactions_for_emoji(
	Snowflake channel_id, Snowflake message_id, RequestReaction emoji) const {
	return DeleteAllReactionsForEmoji{
		m_rate_limiter_make_request, channel_id, message_id, std::move(emoji)};
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

EditChannelPermissions HttpClient::edit_channel_permissions(
	Snowflake channel_id, const PermissionOverwrite &overwrite) const {
	return EditChannelPermissions{
		m_rate_limiter_make_request, channel_id, overwrite};
}

GetChannelInvites HttpClient::get_channel_invites(Snowflake channel_id) const {
	return GetChannelInvites{m_rate_limiter_make_request, channel_id};
}

CreateInvite HttpClient::create_invite(Snowflake channel_id) const {
	return CreateInvite{m_rate_limiter_make_request, channel_id};
}

DeleteChannelPermission HttpClient::delete_channel_permission(
	Snowflake channel_id, Snowflake overwrite_id) {
	return DeleteChannelPermission{
		m_rate_limiter_make_request, channel_id, overwrite_id};
}

FollowAnnouncementChannel HttpClient::follow_announcement_channel(
	Snowflake channel_id, Snowflake webhook_channel_id) const {
	return FollowAnnouncementChannel{
		m_rate_limiter_make_request, channel_id, webhook_channel_id};
}

TriggerTypingIndicator HttpClient::trigger_typing_indicator(
	Snowflake channel_id) const {
	return TriggerTypingIndicator{m_rate_limiter_make_request, channel_id};
}

GetPinnedMessages HttpClient::get_pinned_messages(Snowflake channel_id) const {
	return GetPinnedMessages{m_rate_limiter_make_request, channel_id};
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

ModifyCurrentUser HttpClient::modify_current_user() const {
	return ModifyCurrentUser{m_rate_limiter_make_request};
}

CreateDM HttpClient::create_dm(Snowflake user_id) const {
	return CreateDM{m_rate_limiter_make_request, user_id};
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

	if (auto res = m_http->request(req, yield); res) { return res; }

	BOOST_OUTCOME_TRY(
		auto http, net::HttpConnection::connect("https://discord.com", yield));
	m_http = std::move(http);

	return m_http->request(req, yield);
}
}  // namespace ekizu