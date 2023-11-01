#include <ekizu/request/create_message.hpp>
#include <nlohmann/json.hpp>

namespace ekizu
{
CreateMessage::CreateMessage(
	std::function<Result<net::HttpResponse>(net::HttpRequest)> make_request,
	Snowflake channel_id)
	: m_channel_id{ channel_id }
	, m_make_request{ std::move(make_request) }
{
}

CreateMessage::operator net::HttpRequest() const
{
	net::HttpRequest req;
	req.method = net::HttpMethod::Post;
	req.path = fmt::format("/channels/{}/messages", m_channel_id);
	req.headers.emplace("Content-Type", "application/json");
	// TODO: There are more fields that just "content" but this will work for now.
	req.body =
		nlohmann::json{
			{ "content", m_content ? *m_content : "" },
		}
			.dump();
	return req;
}

Result<net::HttpResponse> CreateMessage::send()
{
	if (!m_make_request) {
		return tl::make_unexpected(std::make_error_code(
			std::errc::operation_not_permitted));
	}

	return m_make_request(*this);
}
} // namespace ekizu