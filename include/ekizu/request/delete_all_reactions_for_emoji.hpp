#ifndef EKIZU_REQUEST_DELETE_ALL_REACTIONS_FOR_EMOJI_HPP
#define EKIZU_REQUEST_DELETE_ALL_REACTIONS_FOR_EMOJI_HPP

#include <ekizu/request/create_reaction.hpp>

namespace ekizu {
struct DeleteAllReactionsForEmoji {
	DeleteAllReactionsForEmoji(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id, Snowflake message_id, RequestReaction emoji);

	operator net::HttpRequest() const;

	EKIZU_EXPORT Result<> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	RequestReaction m_emoji;
	Snowflake m_message_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_DELETE_ALL_REACTIONS_FOR_EMOJI_HPP
