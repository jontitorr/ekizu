#include <ekizu/json_util.hpp>
#include <ekizu/request/get_guild_preview.hpp>

namespace ekizu {
GetGuildPreview::GetGuildPreview(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id)
	: m_guild_id{guild_id}, m_make_request{make_request} {}

GetGuildPreview::operator net::HttpRequest() const {
	return net::HttpRequest{net::HttpMethod::get,
							fmt::format("/guilds/{}/preview", m_guild_id), 11};
}

Result<GuildPreview> GetGuildPreview::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<GuildPreview>(res.body());
}
}  // namespace ekizu
