#include <ekizu/request/interaction_client.hpp>

namespace ekizu {
InteractionClient::InteractionClient(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake application_id)
	: m_application_id{application_id}, m_make_request{make_request} {}

CreateResponse InteractionClient::create_response(
	Snowflake interaction_id, std::string_view interaction_token,
	InteractionResponse response) const {
	return {
		m_make_request, interaction_id, interaction_token, std::move(response)};
}

GetOriginalResponse InteractionClient::get_original_response(
	Snowflake interaction_id, std::string_view interaction_token) const {
	return {m_make_request, m_application_id, interaction_token};
}
}  // namespace ekizu