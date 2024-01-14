#include <ekizu/json_util.hpp>
#include <ekizu/request/delete_channel.hpp>

namespace ekizu {
DeleteChannel::DeleteChannel(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake channel_id)
	: m_channel_id{channel_id}, m_make_request{make_request} {}

DeleteChannel::operator net::HttpRequest() const {
	return {net::HttpMethod::delete_, fmt::format("/channels/{}", m_channel_id),
			11};
}

Result<Channel> DeleteChannel::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<Channel>(res.body());
}

}  // namespace ekizu
