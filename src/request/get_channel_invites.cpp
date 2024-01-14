#include <ekizu/json_util.hpp>
#include <ekizu/request/get_channel_invites.hpp>

namespace ekizu {
GetChannelInvites::GetChannelInvites(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake channel_id)
	: m_channel_id{channel_id}, m_make_request{make_request} {}

GetChannelInvites::operator net::HttpRequest() const {
	return {net::HttpMethod::get,
			fmt::format("/channels/{}/invites", m_channel_id), 11};
}

Result<std::vector<Invite>> GetChannelInvites::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));
	return json_util::deserialize<std::vector<Invite>>(res.body());
}
}  // namespace ekizu
