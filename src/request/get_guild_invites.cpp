#include <ekizu/json_util.hpp>
#include <ekizu/request/get_guild_invites.hpp>

namespace ekizu {
GetGuildInvites::GetGuildInvites(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id)
	: m_guild_id{guild_id}, m_make_request{make_request} {}

GetGuildInvites::operator net::HttpRequest() const {
	return net::HttpRequest{net::HttpMethod::get,
							fmt::format("/guilds/{}/invites", m_guild_id), 11};
}

Result<std::vector<Invite>> GetGuildInvites::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<std::vector<Invite>>(res.body());
}
}  // namespace ekizu
