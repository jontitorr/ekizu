#include <ekizu/request/unpin_message.hpp>

namespace ekizu {
UnpinMessage::UnpinMessage(
	const std::function<void(net::HttpRequest,
							 std::function<void(net::HttpResponse)>)>&
		make_request,
	Snowflake channel_id, Snowflake message_id)
	: m_channel_id{channel_id},
	  m_message_id{message_id},
	  m_make_request{make_request} {}

UnpinMessage::operator net::HttpRequest() const {
	return {net::HttpMethod::delete_,
			fmt::format("/channels/{}/pins/{}", m_channel_id, m_message_id),
			11};
}

Result<void> UnpinMessage::send(
	std::function<void(net::HttpResponse)> cb) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	m_make_request(*this, [cb = std::move(cb)](const net::HttpResponse& res) {
		if (cb) { cb(res); }
	});

	return outcome::success();
}
}  // namespace ekizu
