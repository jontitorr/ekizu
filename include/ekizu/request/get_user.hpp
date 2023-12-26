#ifndef EKIZU_REQUEST_GET_USER_HPP
#define EKIZU_REQUEST_GET_USER_HPP

#include <ekizu/http.hpp>
#include <ekizu/user.hpp>

namespace ekizu {
/**
 * @brief Represents the  REST API endpoint.
 */
struct GetUser {
	GetUser(const std::function<Result<net::HttpResponse>(
				net::HttpRequest, const asio::yield_context &)> &make_request,
			Snowflake user_id);

	/**
	 * @brief Converts the GetUser to an HTTP request.
	 *
	 * @return The HTTP request.
	 */
	operator net::HttpRequest() const;

	/**
	 * @brief Sends the GetUser request.
	 *
	 * @return The result of the request as an HTTP response.
	 */
	Result<User> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_user_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_USER_HPP
