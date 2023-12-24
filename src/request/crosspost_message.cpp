#include <ekizu/json_util.hpp>
#include <ekizu/request/crosspost_message.hpp>

namespace ekizu {
CrosspostMessage::CrosspostMessage(
	const std::function<void(net::HttpRequest,
							 std::function<void(net::HttpResponse)>)>&
		make_request,
	Snowflake channel_id, Snowflake message_id)
	: m_channel_id{channel_id},
	  m_message_id{message_id},
	  m_make_request{make_request} {}

CrosspostMessage::operator net::HttpRequest() const {
	return {net::HttpMethod::post,
			fmt::format("/channels/{}/messages/{}/crosspost", m_channel_id,
						m_message_id),
			11};
}

void CrosspostMessage::send(std::function<void(Message)> cb) const {
	if (!m_make_request) { return; }

	m_make_request(*this, [c = std::move(cb)](net::HttpResponse res) {
		auto msg = json_util::deserialize<Message>(res.body());
		if (c && msg) { c(msg.value()); }
	});
}
}  // namespace ekizu
