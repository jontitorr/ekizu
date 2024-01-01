#include <ekizu/json_util.hpp>
#include <ekizu/request/get_guild.hpp>

namespace ekizu {
GetGuild::GetGuild(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id)
	: m_guild_id{guild_id}, m_make_request{make_request} {}

GetGuild::operator net::HttpRequest() const {
	return net::HttpRequest{
		net::HttpMethod::get,
		fmt::format("/guilds/{}{}", m_guild_id,
					m_fields.with_counts
						? fmt::format("?with_counts={}", *m_fields.with_counts)
						: ""),
		11};
}

Result<Guild> GetGuild::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<Guild>(res.body());
}
}  // namespace ekizu
