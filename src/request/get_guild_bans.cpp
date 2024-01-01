#include <ekizu/json_util.hpp>
#include <ekizu/request/get_guild_bans.hpp>

namespace ekizu {
using json_util::deserialize;

GetGuildBans::GetGuildBans(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id)
	: m_guild_id{guild_id}, m_make_request{make_request} {}

GetGuildBans::operator net::HttpRequest() const {
	auto url = fmt::format("/guilds/{}/bans", m_guild_id);
	std::string params;

	if (m_fields.limit) {
		if (params.empty()) { params += '?'; }
		params += fmt::format("{}limit={}", *m_fields.limit);
	}

	if (m_fields.before) {
		params += fmt::format(
			"{}before={}", params.empty() ? '?' : '&', *m_fields.before);
	}

	if (m_fields.after) {
		params += fmt::format(
			"{}after={}", params.empty() ? '?' : '&', *m_fields.after);
	}

	url = fmt::format("{}{}", std::move(url), std::move(params));

	return {net::HttpMethod::get, url, 11};
}

Result<std::vector<Ban>> GetGuildBans::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<std::vector<Ban>>(res.body());
}
}  // namespace ekizu
