#include <ekizu/request/unpin_message.hpp>

namespace ekizu
{
UnpinMessage::UnpinMessage(
	const std::function<Result<net::HttpResponse>(net::HttpRequest)>
		&make_request,
	Snowflake channel_id, Snowflake message_id)
	: m_channel_id{ channel_id }
	, m_message_id{ message_id }
	, m_make_request{ make_request }
{
}

UnpinMessage::operator net::HttpRequest() const
{
	return {
		net::HttpMethod::Delete,
		fmt::format("/channels/{}/pins/{}", m_channel_id, m_message_id),
		{},
		{},
	};
}

Result<net::HttpResponse> UnpinMessage::send() const
{
	if (!m_make_request) {
		return tl::make_unexpected(std::make_error_code(
			std::errc::operation_not_permitted));
	}

	return m_make_request(*this);
}
} // namespace ekizu
