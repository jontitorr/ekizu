#include <ekizu/json_util.hpp>
#include <ekizu/request/get_guild_member.hpp>

namespace ekizu {
GetGuildMember::GetGuildMember(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id, Snowflake user_id)
	: m_guild_id{guild_id}, m_user_id{user_id}, m_make_request{make_request} {}

GetGuildMember::operator net::HttpRequest() const {
	return net::HttpRequest{
		net::HttpMethod::get,
		fmt::format("/guilds/{}/members/{}", m_guild_id, m_user_id), 11};
}

Result<GuildMember> GetGuildMember::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<GuildMember>(res.body());
}
}  // namespace ekizu
