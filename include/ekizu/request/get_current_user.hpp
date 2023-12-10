#ifndef EKIZU_REQUEST_GET_CURRENT_USER_HPP
#define EKIZU_REQUEST_GET_CURRENT_USER_HPP

#include <ekizu/user.hpp>
#include <net/http.hpp>

namespace ekizu {
/**
 * @brief Represents the  REST API endpoint.
 */
struct GetCurrentUser {
	GetCurrentUser(
		const std::function<Result<net::HttpResponse>(net::HttpRequest)>
			&make_request);

	/**
	 * @brief Converts the GetCurrentUser to an HTTP request.
	 *
	 * @return The HTTP request.
	 */
	operator net::HttpRequest() const;

	/**
	 * @brief Sends the GetCurrentUser request.
	 *
	 * @return The result of the request as an HTTP response.
	 */
	[[nodiscard]] Result<User> send() const;

   private:
	std::function<Result<net::HttpResponse>(net::HttpRequest)> m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_CURRENT_USER_HPP
