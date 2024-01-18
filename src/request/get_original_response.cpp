#include <ekizu/json_util.hpp>
#include <ekizu/request/interaction/get_original_response.hpp>

namespace ekizu {
GetOriginalResponse::GetOriginalResponse(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake application_id, std::string_view interaction_token)
	: m_application_id{application_id},
	  m_interaction_token{interaction_token},
	  m_make_request{make_request} {}

GetOriginalResponse::operator net::HttpRequest() const {
	return {net::HttpMethod::get,
			fmt::format("/webhooks/{}/{}/messages/@original", m_application_id,
						m_interaction_token),
			11};
}

Result<Message> GetOriginalResponse::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<Message>(res.body());
}
}  // namespace ekizu
