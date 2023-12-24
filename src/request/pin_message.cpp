#include <ekizu/request/pin_message.hpp>

namespace ekizu {
PinMessage::PinMessage(
	const std::function<void(net::HttpRequest,
							 std::function<void(net::HttpResponse)>)>&
		make_request,
	Snowflake channel_id, Snowflake message_id)
	: m_channel_id{channel_id},
	  m_message_id{message_id},
	  m_make_request{make_request} {}

PinMessage::operator net::HttpRequest() const {
	return {net::HttpMethod::put,
			fmt::format("/channels/{}/pins/{}", m_channel_id, m_message_id),
			11};
}

void PinMessage::send(std::function<void()> cb) const {
	if (!m_make_request) { return; }

	m_make_request(*this, [c = std::move(cb)](const net::HttpResponse& res) {
		if (res.result() == net::HttpStatus::no_content && c) { c(); }
	});
}
}  // namespace ekizu
