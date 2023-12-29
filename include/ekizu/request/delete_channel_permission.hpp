#include <ekizu/http.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu {
struct DeleteChannelPermission {
	DeleteChannelPermission(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id, Snowflake overwrite_id);

	operator net::HttpRequest() const;

	Result<void> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	Snowflake m_overwrite_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu