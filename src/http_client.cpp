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

CreateGuild HttpClient::create_guild(std::string_view name) const {
	return CreateGuild{m_rate_limiter_make_request, name};
}

GetGuild HttpClient::get_guild(Snowflake guild_id) const {
	return GetGuild{m_rate_limiter_make_request, guild_id};
}

GetGuildPreview HttpClient::get_guild_preview(Snowflake guild_id) const {
	return GetGuildPreview{m_rate_limiter_make_request, guild_id};
}

ModifyGuild HttpClient::modify_guild(Snowflake guild_id) const {
	return ModifyGuild{m_rate_limiter_make_request, guild_id};
}

DeleteGuild HttpClient::delete_guild(Snowflake guild_id) const {
	return DeleteGuild{m_rate_limiter_make_request, guild_id};
}

GetGuildChannels HttpClient::get_guild_channels(Snowflake guild_id) const {
	return GetGuildChannels{m_rate_limiter_make_request, guild_id};
}

CreateGuildChannel HttpClient::create_guild_channel(
	Snowflake guild_id, std::string_view name) const {
	return CreateGuildChannel{m_rate_limiter_make_request, guild_id, name};
}

ModifyGuildChannelPositions HttpClient::modify_guild_channel_positions(
	Snowflake guild_id,
	std::vector<ModifyGuildChannelPosition> channels) const {
	return ModifyGuildChannelPositions{
		m_rate_limiter_make_request, guild_id, std::move(channels)};
}

ListActiveGuildThreads HttpClient::list_active_guild_threads(
	Snowflake guild_id) const {
	return ListActiveGuildThreads{m_rate_limiter_make_request, guild_id};
}

GetGuildMember HttpClient::get_guild_member(Snowflake guild_id,
											Snowflake user_id) const {
	return GetGuildMember{m_rate_limiter_make_request, guild_id, user_id};
}

ListGuildMembers HttpClient::list_guild_members(Snowflake guild_id) const {
	return ListGuildMembers{m_rate_limiter_make_request, guild_id};
}

SearchGuildMembers HttpClient::search_guild_members(Snowflake guild_id) const {
	return SearchGuildMembers{m_rate_limiter_make_request, guild_id};
}

AddGuildMember HttpClient::add_guild_member(
	Snowflake guild_id, Snowflake user_id,
	std::string_view access_token) const {
	return AddGuildMember{
		m_rate_limiter_make_request, guild_id, user_id, access_token};
}

ModifyGuildMember HttpClient::modify_guild_member(Snowflake guild_id,
												  Snowflake user_id) const {
	return ModifyGuildMember{m_rate_limiter_make_request, guild_id, user_id};
}

ModifyCurrentMember HttpClient::modify_current_member(
	Snowflake guild_id) const {
	return ModifyCurrentMember{m_rate_limiter_make_request, guild_id};
}

AddGuildMemberRole HttpClient::add_guild_member_role(
	Snowflake guild_id, Snowflake user_id, Snowflake role_id) const {
	return AddGuildMemberRole{
		m_rate_limiter_make_request, guild_id, user_id, role_id};
}

RemoveGuildMemberRole HttpClient::remove_guild_member_role(
	Snowflake guild_id, Snowflake user_id, Snowflake role_id) const {
	return RemoveGuildMemberRole{
		m_rate_limiter_make_request, guild_id, user_id, role_id};
}

RemoveGuildMember HttpClient::remove_guild_member(Snowflake guild_id,
												  Snowflake user_id) const {
	return RemoveGuildMember{m_rate_limiter_make_request, guild_id, user_id};
}

GetGuildBans HttpClient::get_guild_bans(Snowflake guild_id) const {
	return GetGuildBans{m_rate_limiter_make_request, guild_id};
}

GetGuildBan HttpClient::get_guild_ban(Snowflake guild_id,
									  Snowflake user_id) const {
	return GetGuildBan{m_rate_limiter_make_request, guild_id, user_id};
}

GetGuildInvites HttpClient::get_guild_invites(Snowflake guild_id) const {
	return GetGuildInvites{m_rate_limiter_make_request, guild_id};
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
		EKIZU_TRY(auto http,
				  net::HttpConnection::connect("https://discord.com", yield));
		m_http = std::move(http);
	}

	if (auto res = m_http->request(req, yield); res) { return res; }

	EKIZU_TRY(
		auto http, net::HttpConnection::connect("https://discord.com", yield));
	m_http = std::move(http);

	return m_http->request(req, yield);
}
}  // namespace ekizu