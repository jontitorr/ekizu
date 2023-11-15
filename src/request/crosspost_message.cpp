#include <ekizu/request/crosspost_message.hpp>

namespace ekizu
{
CrosspostMessage::CrosspostMessage(
	const std::function<Result<net::HttpResponse>(net::HttpRequest)>
		&make_request,
	Snowflake channel_id, Snowflake message_id)
	: m_channel_id{ channel_id }
	, m_message_id{ message_id }
	, m_make_request{ make_request }
{
}

CrosspostMessage::operator net::HttpRequest() const
{
	return {
		net::HttpMethod::Post,
		fmt::format("/channels/{}/messages/{}/crosspost", m_channel_id,
			    m_message_id),
		{},
		{},
	};
}

Result<net::HttpResponse> CrosspostMessage::send() const
{
	if (!m_make_request) {
		return tl::make_unexpected(std::make_error_code(
			std::errc::operation_not_permitted));
	}

	return m_make_request(*this);
}
} // namespace ekizu
