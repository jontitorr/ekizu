#ifndef EKIZU_REQUEST_FOLLOW_ANNOUNCEMENT_CHANNEL_HPP
#define EKIZU_REQUEST_FOLLOW_ANNOUNCEMENT_CHANNEL_HPP

#include <ekizu/http.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu {
struct FollowedChannel {
	Snowflake channel_id;
	Snowflake webhook_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const FollowedChannel &f);
EKIZU_EXPORT void from_json(const nlohmann::json &j, FollowedChannel &f);

struct FollowAnnouncementChannel {
	FollowAnnouncementChannel(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id, Snowflake webhook_channel_id);

	operator net::HttpRequest() const;

	EKIZU_EXPORT Result<> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	Snowflake m_webhook_channel_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_FOLLOW_ANNOUNCEMENT_CHANNEL_HPP
