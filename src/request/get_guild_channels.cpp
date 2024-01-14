#include <ekizu/json_util.hpp>
#include <ekizu/request/get_guild_channels.hpp>

namespace ekizu {
GetGuildChannels::GetGuildChannels(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id)
	: m_guild_id{guild_id}, m_make_request{make_request} {}

GetGuildChannels::operator net::HttpRequest() const {
	return net::HttpRequest{net::HttpMethod::get,
							fmt::format("/guilds/{}/channels", m_guild_id), 11};
}

Result<std::vector<Channel>> GetGuildChannels::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<std::vector<Channel>>(res.body());
}
}  // namespace ekizu
