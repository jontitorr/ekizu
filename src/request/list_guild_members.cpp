#include <ekizu/json_util.hpp>
#include <ekizu/request/list_guild_members.hpp>

namespace ekizu {
using json_util::deserialize;

ListGuildMembers::ListGuildMembers(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id)
	: m_guild_id{guild_id}, m_make_request{make_request} {}

ListGuildMembers::operator net::HttpRequest() const {
	auto url = fmt::format("/guilds/{}/members", m_guild_id);

	if (m_fields.limit) { url += fmt::format("?limit={}", *m_fields.limit); }

	if (m_fields.after) {
		url += fmt::format(
			"{}after={}", m_fields.limit ? '&' : '?', *m_fields.after);
	}

	return {net::HttpMethod::get, url, 11};
}

Result<std::vector<GuildMember>> ListGuildMembers::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<std::vector<GuildMember>>(res.body());
}
}  // namespace ekizu
