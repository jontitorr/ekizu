#include <ekizu/http_client.hpp>
#include <ekizu/json_util.hpp>
#include <ekizu/request/get_channel.hpp>

namespace ekizu {
GetChannel::GetChannel(
	const std::function<void(net::HttpRequest,
							 std::function<void(net::HttpResponse)>)>
		&make_request,
	Snowflake channel_id)
	: m_channel_id{channel_id}, m_make_request{make_request} {}

GetChannel::operator net::HttpRequest() const {
	return net::HttpRequest{
		net::HttpMethod::get, fmt::format("/channels/{}", m_channel_id), 11};
}

void GetChannel::send(std::function<void(Channel)> cb) const {
	if (!m_make_request) { return; }

	m_make_request(*this, [cb = std::move(cb)](net::HttpResponse res) {
		auto msg = json_util::deserialize<Channel>(res.body());
		if (cb && msg) { cb(msg.value()); }
	});
}
}  // namespace ekizu
