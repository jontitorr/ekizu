#include <ekizu/request/bulk_delete_messages.hpp>
#include <nlohmann/json.hpp>

namespace ekizu {
BulkDeleteMessages::BulkDeleteMessages(
	const std::function<Result<net::HttpResponse>(net::HttpRequest)>
		&make_request,
	Snowflake channel_id, const std::vector<Snowflake> &message_ids)
	: m_channel_id{channel_id},
	  m_message_ids{message_ids},
	  m_make_request{make_request} {}

BulkDeleteMessages::operator net::HttpRequest() const {
	return {
		net::HttpMethod::Post,
		fmt::format("/channels/{}/messages/bulk-delete", m_channel_id),
		nlohmann::json{{"messages", m_message_ids}}.dump(),
		{},
	};
}

Result<net::HttpResponse> BulkDeleteMessages::send() const {
	if (!m_make_request) {
		return tl::make_unexpected(
			std::make_error_code(std::errc::operation_not_permitted));
	}

	return m_make_request(*this);
}
}  // namespace ekizu
