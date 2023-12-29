#ifndef EKIZU_REQUEST_GET_CHANNEL_INVITES_HPP
#define EKIZU_REQUEST_GET_CHANNEL_INVITES_HPP

#include <ekizu/http.hpp>
#include <ekizu/invite.hpp>

namespace ekizu {
struct GetChannelInvites {
	GetChannelInvites(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id);

	operator net::HttpRequest() const;

	[[nodiscard]] Result<std::vector<Invite>> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_CHANNEL_INVITES_HPP
