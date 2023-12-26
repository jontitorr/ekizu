#include <ekizu/http_client.hpp>
#include <ekizu/json_util.hpp>
#include <ekizu/request/get_channel.hpp>

namespace ekizu {
GetChannel::GetChannel(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake channel_id)
	: m_channel_id{channel_id}, m_make_request{make_request} {}

GetChannel::operator net::HttpRequest() const {
	return net::HttpRequest{
		net::HttpMethod::get, fmt::format("/channels/{}", m_channel_id), 11};
}

Result<Channel> GetChannel::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<Channel>(res.body());
}
}  // namespace ekizu
