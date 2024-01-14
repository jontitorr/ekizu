#ifndef EKIZU_REQUEST_CREATE_DM_HPP
#define EKIZU_REQUEST_CREATE_DM_HPP

#include <ekizu/channel.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct CreateDM {
	CreateDM(const std::function<Result<net::HttpResponse>(
				 net::HttpRequest, const asio::yield_context &)> &make_request,
			 Snowflake user_id);

	operator net::HttpRequest() const;

	EKIZU_EXPORT Result<Channel> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_user_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_CREATE_DM_HPP
