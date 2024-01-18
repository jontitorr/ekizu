#ifndef EKIZU_REQUEST_INTERACTION_GET_ORIGINAL_RESPONSE_HPP
#define EKIZU_REQUEST_INTERACTION_GET_ORIGINAL_RESPONSE_HPP

#include <ekizu/http.hpp>
#include <ekizu/message.hpp>

namespace ekizu {
struct GetOriginalResponse {
	GetOriginalResponse(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake application_id, std::string_view interaction_token);

	operator net::HttpRequest() const;

	EKIZU_EXPORT Result<Message> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_application_id;
	std::string m_interaction_token;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_INTERACTION_GET_ORIGINAL_RESPONSE_HPP
