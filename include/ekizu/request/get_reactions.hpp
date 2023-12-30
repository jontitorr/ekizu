#ifndef EKIZU_REQUEST_GET_REACTIONS_HPP
#define EKIZU_REQUEST_GET_REACTIONS_HPP

#include <ekizu/request/create_reaction.hpp>

namespace ekizu {
struct GetReactions {
	GetReactions(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context&)>& make_request,
		Snowflake channel_id, Snowflake message_id, RequestReaction emoji);

	operator net::HttpRequest() const;

	GetReactions& after(Snowflake after) {
		m_after = after;
		return *this;
	}

	GetReactions& limit(uint8_t limit) {
		m_limit = limit;
		return *this;
	}

	[[nodiscard]] EKIZU_EXPORT Result<std::vector<User>> send(
		const asio::yield_context& yield) const;

   private:
	std::optional<Snowflake> m_after;
	Snowflake m_channel_id;
	RequestReaction m_emoji;
	std::optional<uint8_t> m_limit;
	Snowflake m_message_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context&)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_REACTIONS_HPP
