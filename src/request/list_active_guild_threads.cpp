#include <ekizu/json_util.hpp>
#include <ekizu/request/list_active_guild_threads.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j,
			 const ListActiveGuildThreadsResponse &response) {
	serialize(j, "threads", response.threads);
	serialize(j, "members", response.members);
}

void from_json(const nlohmann::json &j,
			   ListActiveGuildThreadsResponse &response) {
	deserialize(j, "threads", response.threads);
	deserialize(j, "members", response.members);
}

ListActiveGuildThreads::ListActiveGuildThreads(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id)
	: m_guild_id{guild_id}, m_make_request{make_request} {}

ListActiveGuildThreads::operator net::HttpRequest() const {
	return net::HttpRequest{
		net::HttpMethod::get,
		fmt::format("/guilds/{}/threads/active", m_guild_id), 11};
}

Result<ListActiveGuildThreadsResponse> ListActiveGuildThreads::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return deserialize<ListActiveGuildThreadsResponse>(res.body());
}
}  // namespace ekizu
