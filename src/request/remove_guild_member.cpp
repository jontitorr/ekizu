#include <ekizu/request/remove_guild_member.hpp>

namespace ekizu {
RemoveGuildMember::RemoveGuildMember(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id, Snowflake user_id)
	: m_guild_id{guild_id}, m_user_id{user_id}, m_make_request{make_request} {}

RemoveGuildMember::operator net::HttpRequest() const {
	return {net::HttpMethod::delete_,
			fmt::format("/guilds/{}/members/{}", m_guild_id, m_user_id), 11};
}

Result<> RemoveGuildMember::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	if (res.result() == net::HttpStatus::no_content) {
		return outcome::success();
	}

	return boost::system::errc::operation_not_permitted;
}
}  // namespace ekizu
