#ifndef EKIZU_REQUEST_REMOVE_GUILD_MEMBER_ROLE_HPP
#define EKIZU_REQUEST_REMOVE_GUILD_MEMBER_ROLE_HPP

#include <ekizu/http.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu {
struct RemoveGuildMemberRole {
	RemoveGuildMemberRole(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake guild_id, Snowflake user_id, Snowflake role_id);

	operator net::HttpRequest() const;

	EKIZU_EXPORT Result<> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	Snowflake m_user_id;
	Snowflake m_role_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_REMOVE_GUILD_MEMBER_ROLE_HPP
