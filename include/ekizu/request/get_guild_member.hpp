#ifndef EKIZU_REQUEST_GET_GUILD_MEMBER_HPP
#define EKIZU_REQUEST_GET_GUILD_MEMBER_HPP

#include <ekizu/guild_member.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct GetGuildMember {
	GetGuildMember(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake guild_id, Snowflake user_id);

	operator net::HttpRequest() const;

	EKIZU_EXPORT Result<GuildMember> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	Snowflake m_user_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_GUILD_MEMBER_HPP
