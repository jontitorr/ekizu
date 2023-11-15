#include <ekizu/http_client.hpp>
#include <ekizu/json_util.hpp>
#include <ekizu/request/get_channel.hpp>

namespace ekizu
{
GetChannel::GetChannel(
	const std::function<Result<net::HttpResponse>(net::HttpRequest)>
		&make_request,
	Snowflake channel_id)
	: m_channel_id{ channel_id }
	, m_make_request{ make_request }
{
}

GetChannel::operator net::HttpRequest() const
{
	return {
		net::HttpMethod::Get,
		fmt::format("/channels/{}", m_channel_id),
		{},
		{},
	};
}

Result<Channel> GetChannel::send() const
{
	if (!m_make_request) {
		return tl::make_unexpected(std::make_error_code(
			std::errc::operation_not_permitted));
	}

	return m_make_request(*this).and_then([](auto res) -> Result<Channel> {
		return json_util::deserialize<Channel>(res.body);
	});
}
} // namespace ekizu
