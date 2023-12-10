#ifndef EKIZU_REQUEST_GET_USER_HPP
#define EKIZU_REQUEST_GET_USER_HPP

#include <ekizu/user.hpp>
#include <net/http.hpp>

namespace ekizu {
/**
 * @brief Represents the  REST API endpoint.
 */
struct GetUser {
	GetUser(const std::function<Result<net::HttpResponse>(net::HttpRequest)>
				&make_request,
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
	[[nodiscard]] Result<User> send() const;

   private:
	Snowflake m_user_id;
	std::function<Result<net::HttpResponse>(net::HttpRequest)> m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_USER_HPP
