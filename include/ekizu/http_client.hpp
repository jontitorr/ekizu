#ifndef EKIZU_HTTP_CLIENT_HPP
#define EKIZU_HTTP_CLIENT_HPP

#include <ekizu/http.hpp>
#include <ekizu/rate_limiter.hpp>
#include <ekizu/request/bulk_delete_messages.hpp>
#include <ekizu/request/create_dm.hpp>
#include <ekizu/request/create_message.hpp>
#include <ekizu/request/create_reaction.hpp>
#include <ekizu/request/crosspost_message.hpp>
#include <ekizu/request/delete_channel.hpp>
#include <ekizu/request/delete_message.hpp>
#include <ekizu/request/delete_own_reaction.hpp>
#include <ekizu/request/delete_user_reaction.hpp>
#include <ekizu/request/edit_message.hpp>
#include <ekizu/request/get_channel.hpp>
#include <ekizu/request/get_channel_message.hpp>
#include <ekizu/request/get_channel_messages.hpp>
#include <ekizu/request/get_current_user.hpp>
#include <ekizu/request/get_reactions.hpp>
#include <ekizu/request/get_user.hpp>
#include <ekizu/request/modify_channel.hpp>
#include <ekizu/request/modify_current_user.hpp>
#include <ekizu/request/pin_message.hpp>
#include <ekizu/request/unpin_message.hpp>

namespace ekizu {
struct HttpClient {
	explicit HttpClient(std::string_view token);

	[[nodiscard]] GetChannel get_channel(Snowflake channel_id) const;
	[[nodiscard]] ModifyChannel modify_channel(Snowflake channel_id) const;
	[[nodiscard]] DeleteChannel delete_channel(Snowflake channel_id) const;
	[[nodiscard]] GetChannelMessages get_channel_messages(
		Snowflake channel_id) const;
	[[nodiscard]] GetChannelMessage get_channel_message(
		Snowflake channel_id, Snowflake message_id) const;
	[[nodiscard]] CreateMessage create_message(Snowflake channel_id) const;
	[[nodiscard]] CreateReaction create_reaction(Snowflake channel_id,
												 Snowflake message_id,
												 RequestReaction emoji) const;
	[[nodiscard]] DeleteOwnReaction delete_own_reaction(
		Snowflake channel_id, Snowflake message_id,
		RequestReaction emoji) const;
	[[nodiscard]] DeleteUserReaction delete_user_reaction(
		Snowflake channel_id, Snowflake message_id, RequestReaction emoji,
		Snowflake user_id) const;
	[[nodiscard]] GetReactions get_reactions(Snowflake channel_id,
											 Snowflake message_id,
											 RequestReaction emoji) const;
	[[nodiscard]] CrosspostMessage crosspost_message(
		Snowflake channel_id, Snowflake message_id) const;
	[[nodiscard]] EditMessage edit_message(Snowflake channel_id,
										   Snowflake message_id) const;
	[[nodiscard]] DeleteMessage delete_message(Snowflake channel_id,
											   Snowflake message_id) const;
	[[nodiscard]] BulkDeleteMessages bulk_delete_messages(
		Snowflake channel_id, const std::vector<Snowflake> &message_ids) const;
	[[nodiscard]] PinMessage pin_message(Snowflake channel_id,
										 Snowflake message_id) const;
	[[nodiscard]] UnpinMessage unpin_message(Snowflake channel_id,
											 Snowflake message_id) const;
	[[nodiscard]] GetCurrentUser get_current_user() const;
	[[nodiscard]] GetUser get_user(Snowflake user_id) const;
	[[nodiscard]] ModifyCurrentUser modify_current_user() const;
	[[nodiscard]] CreateDM create_dm(Snowflake user_id) const;

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
