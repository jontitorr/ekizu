#ifndef EKIZU_REQUEST_LIST_ACTIVE_GUILD_THREADS_HPP
#define EKIZU_REQUEST_LIST_ACTIVE_GUILD_THREADS_HPP

#include <ekizu/channel.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct ListActiveGuildThreadsResponse {
	std::vector<Channel> threads;
	std::vector<ThreadMember> members;
};

EKIZU_EXPORT void to_json(nlohmann::json &j,
						  const ListActiveGuildThreadsResponse &response);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
							ListActiveGuildThreadsResponse &response);

struct ListActiveGuildThreads {
	ListActiveGuildThreads(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake guild_id);

	operator net::HttpRequest() const;

	EKIZU_EXPORT Result<ListActiveGuildThreadsResponse> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_LIST_ACTIVE_GUILD_THREADS_HPP
