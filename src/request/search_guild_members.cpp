#include <ekizu/json_util.hpp>
#include <ekizu/request/search_guild_members.hpp>

namespace ekizu {
using json_util::deserialize;

SearchGuildMembers::SearchGuildMembers(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id)
	: m_guild_id{guild_id}, m_make_request{make_request} {}

SearchGuildMembers::operator net::HttpRequest() const {
	auto url = fmt::format("/guilds/{}/members/search", m_guild_id);

	if (m_fields.query) { url += fmt::format("?query={}", *m_fields.query); }

	if (m_fields.limit) {
		url += fmt::format(
			"{}limit={}", m_fields.query ? '&' : '?', *m_fields.limit);
	}

	return {net::HttpMethod::get, url, 11};
}

Result<std::vector<GuildMember>> SearchGuildMembers::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<std::vector<GuildMember>>(res.body());
}
}  // namespace ekizu
