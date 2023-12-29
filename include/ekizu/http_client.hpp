#ifndef EKIZU_HTTP_CLIENT_HPP
#define EKIZU_HTTP_CLIENT_HPP

#include <ekizu/http.hpp>
#include <ekizu/rate_limiter.hpp>
#include <ekizu/request/bulk_delete_messages.hpp>
#include <ekizu/request/create_dm.hpp>
#include <ekizu/request/create_invite.hpp>
#include <ekizu/request/create_message.hpp>
#include <ekizu/request/create_reaction.hpp>
#include <ekizu/request/crosspost_message.hpp>
#include <ekizu/request/delete_all_reactions.hpp>
#include <ekizu/request/delete_all_reactions_for_emoji.hpp>
#include <ekizu/request/delete_channel.hpp>
#include <ekizu/request/delete_channel_permission.hpp>
#include <ekizu/request/delete_message.hpp>
#include <ekizu/request/delete_own_reaction.hpp>
#include <ekizu/request/delete_user_reaction.hpp>
#include <ekizu/request/edit_channel_permissions.hpp>
#include <ekizu/request/edit_message.hpp>
#include <ekizu/request/follow_announcement_channel.hpp>
#include <ekizu/request/get_channel.hpp>
#include <ekizu/request/get_channel_invites.hpp>
#include <ekizu/request/get_channel_message.hpp>
#include <ekizu/request/get_channel_messages.hpp>
#include <ekizu/request/get_current_user.hpp>
#include <ekizu/request/get_pinned_messages.hpp>
#include <ekizu/request/get_reactions.hpp>
#include <ekizu/request/get_user.hpp>
#include <ekizu/request/modify_channel.hpp>
#include <ekizu/request/modify_current_user.hpp>
#include <ekizu/request/pin_message.hpp>
#include <ekizu/request/trigger_typing_indicator.hpp>
#include <ekizu/request/unpin_message.hpp>

namespace ekizu {
struct HttpClient {
	EKIZU_EXPORT explicit HttpClient(std::string_view token);

	// https://discord.com/developers/docs/resources/channel

	EKIZU_EXPORT [[nodiscard]] GetChannel get_channel(
		Snowflake channel_id) const;
	EKIZU_EXPORT [[nodiscard]] ModifyChannel modify_channel(
		Snowflake channel_id) const;
	EKIZU_EXPORT [[nodiscard]] DeleteChannel delete_channel(
		Snowflake channel_id) const;
	EKIZU_EXPORT [[nodiscard]] GetChannelMessages get_channel_messages(
		Snowflake channel_id) const;
	EKIZU_EXPORT [[nodiscard]] GetChannelMessage get_channel_message(
		Snowflake channel_id, Snowflake message_id) const;
	EKIZU_EXPORT [[nodiscard]] CreateMessage create_message(
		Snowflake channel_id) const;
	EKIZU_EXPORT [[nodiscard]] CrosspostMessage crosspost_message(
		Snowflake channel_id, Snowflake message_id) const;
	EKIZU_EXPORT [[nodiscard]] CreateReaction create_reaction(
		Snowflake channel_id, Snowflake message_id,
		RequestReaction emoji) const;
	EKIZU_EXPORT [[nodiscard]] DeleteOwnReaction delete_own_reaction(
		Snowflake channel_id, Snowflake message_id,
		RequestReaction emoji) const;
	EKIZU_EXPORT [[nodiscard]] DeleteUserReaction delete_user_reaction(
		Snowflake channel_id, Snowflake message_id, RequestReaction emoji,
		Snowflake user_id) const;
	EKIZU_EXPORT [[nodiscard]] GetReactions get_reactions(
		Snowflake channel_id, Snowflake message_id,
		RequestReaction emoji) const;
	EKIZU_EXPORT [[nodiscard]] DeleteAllReactions delete_all_reactions(
		Snowflake channel_id, Snowflake message_id) const;
	EKIZU_EXPORT [[nodiscard]] DeleteAllReactionsForEmoji
	delete_all_reactions_for_emoji(Snowflake channel_id, Snowflake message_id,
								   RequestReaction emoji) const;
	EKIZU_EXPORT [[nodiscard]] EditMessage edit_message(
		Snowflake channel_id, Snowflake message_id) const;
	EKIZU_EXPORT [[nodiscard]] DeleteMessage delete_message(
		Snowflake channel_id, Snowflake message_id) const;
	EKIZU_EXPORT [[nodiscard]] BulkDeleteMessages bulk_delete_messages(
		Snowflake channel_id, const std::vector<Snowflake> &message_ids) const;
	EKIZU_EXPORT [[nodiscard]] EditChannelPermissions edit_channel_permissions(
		Snowflake channel_id, const PermissionOverwrite &overwrite) const;
	EKIZU_EXPORT [[nodiscard]] GetChannelInvites get_channel_invites(
		Snowflake channel_id) const;
	EKIZU_EXPORT [[nodiscard]] CreateInvite create_invite(
		Snowflake channel_id) const;
	EKIZU_EXPORT [[nodiscard]] DeleteChannelPermission
	delete_channel_permission(Snowflake channel_id, Snowflake overwrite_id);
	EKIZU_EXPORT [[nodiscard]] FollowAnnouncementChannel
	follow_announcement_channel(Snowflake channel_id,
								Snowflake webhook_channel_id) const;
	EKIZU_EXPORT [[nodiscard]] TriggerTypingIndicator trigger_typing_indicator(
		Snowflake channel_id) const;
	EKIZU_EXPORT [[nodiscard]] GetPinnedMessages get_pinned_messages(
		Snowflake channel_id) const;
	EKIZU_EXPORT [[nodiscard]] PinMessage pin_message(
		Snowflake channel_id, Snowflake message_id) const;
	EKIZU_EXPORT [[nodiscard]] UnpinMessage unpin_message(
		Snowflake channel_id, Snowflake message_id) const;

	// https://discord.com/developers/docs/resources/user

	EKIZU_EXPORT [[nodiscard]] GetCurrentUser get_current_user() const;
	EKIZU_EXPORT [[nodiscard]] GetUser get_user(Snowflake user_id) const;
	EKIZU_EXPORT [[nodiscard]] ModifyCurrentUser modify_current_user() const;
	EKIZU_EXPORT [[nodiscard]] CreateDM create_dm(Snowflake user_id) const;

   private:
	/// Function which sends an HTTP request. This is wrapped around a
	/// ratelimiter and passed around to other structs which need the
	/// functionality.
	[[nodiscard]] Result<net::HttpResponse> send(
		net::HttpRequest req, const asio::yield_context &yield);

	std::optional<net::HttpConnection> m_http;
	RateLimiter m_rate_limiter;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_rate_limiter_make_request;
	std::optional<std::string> m_token;
};
}  // namespace ekizu

#endif	// EKIZU_HTTP_CLIENT_HPP
