#ifndef EKIZU_REQUEST_GET_USER_HPP
#define EKIZU_REQUEST_GET_USER_HPP

#include <ekizu/http.hpp>
#include <ekizu/user.hpp>

namespace ekizu {
/**
 * @brief Represents the  REST API endpoint.
 */
struct GetUser {
	GetUser(const std::function<void(net::HttpRequest,
									 std::function<void(net::HttpResponse)>)>
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
	void send(std::function<void(User)> cb) const;

   private:
	Snowflake m_user_id;
	std::function<void(
		net::HttpRequest, std::function<void(net::HttpResponse)>)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_USER_HPP
