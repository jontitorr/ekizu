#include <ekizu/request/remove_guild_member_role.hpp>

namespace ekizu {
RemoveGuildMemberRole::RemoveGuildMemberRole(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id, Snowflake user_id, Snowflake role_id)
	: m_guild_id{guild_id},
	  m_user_id{user_id},
	  m_role_id{role_id},
	  m_make_request{make_request} {}

RemoveGuildMemberRole::operator net::HttpRequest() const {
	return {net::HttpMethod::delete_,
			fmt::format("/guilds/{}/members/{}/roles/{}", m_guild_id, m_user_id,
						m_role_id),
			11};
}

Result<> RemoveGuildMemberRole::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));

	if (res.result() == net::HttpStatus::no_content) {
		return outcome::success();
	}

	return boost::system::errc::operation_not_permitted;
}
}  // namespace ekizu
