#include <ekizu/json_util.hpp>
#include <ekizu/request/get_channel_message.hpp>

namespace ekizu {
GetChannelMessage::GetChannelMessage(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake channel_id, Snowflake message_id)
	: m_channel_id{channel_id},
	  m_message_id{message_id},
	  m_make_request{make_request} {}

GetChannelMessage::operator net::HttpRequest() const {
	return {net::HttpMethod::get,
			fmt::format("/channels/{}/messages/{}", m_channel_id, m_message_id),
			11};
}

Result<Message> GetChannelMessage::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<Message>(res.body());
}
}  // namespace ekizu
