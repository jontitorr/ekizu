#ifndef EKIZU_REQUEST_INTERACTION_CLIENT_HPP
#define EKIZU_REQUEST_INTERACTION_CLIENT_HPP

#include <ekizu/request/interaction/create_response.hpp>

namespace ekizu {
struct InteractionClient {
	InteractionClient(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake application_id);

	[[nodiscard]] CreateResponse create_response(
		Snowflake interaction_id, std::string_view interaction_token,
		InteractionResponse response) const;

   private:
	Snowflake m_application_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_INTERACTION_CLIENT_HPP
