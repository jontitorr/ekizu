#ifndef EKIZU_REQUEST_CREATE_REACTION_HPP
#define EKIZU_REQUEST_CREATE_REACTION_HPP

#include <ekizu/emoji.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct CustomEmoji {
	Snowflake id;
	std::string name;
};

using RequestReaction = std::variant<CustomEmoji, std::string>;

struct CreateReaction {
	CreateReaction(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id, Snowflake message_id, RequestReaction emoji);

	operator net::HttpRequest() const;

	Result<> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	RequestReaction m_emoji;
	Snowflake m_message_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu
#endif	// EKIZU_REQUEST_CREATE_REACTION_HPP
