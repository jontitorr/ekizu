#ifndef EKIZU_REQUEST_GET_CURRENT_USER_HPP
#define EKIZU_REQUEST_GET_CURRENT_USER_HPP

#include <ekizu/http.hpp>
#include <ekizu/user.hpp>

namespace ekizu {
/**
 * @brief Represents the  REST API endpoint.
 */
struct GetCurrentUser {
	explicit GetCurrentUser(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request);

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
	Result<User> send(const asio::yield_context &yield) const;

   private:
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_CURRENT_USER_HPP
