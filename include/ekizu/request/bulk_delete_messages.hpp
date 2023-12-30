#ifndef EKIZU_REQUEST_BULK_DELETE_MESSAGES_HPP
#define EKIZU_REQUEST_BULK_DELETE_MESSAGES_HPP

#include <ekizu/http.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu {
struct BulkDeleteMessages {
	BulkDeleteMessages(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id, const std::vector<Snowflake> &message_ids);

	operator net::HttpRequest() const;

	[[nodiscard]] EKIZU_EXPORT Result<> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	std::vector<Snowflake> m_message_ids;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_BULK_DELETE_MESSAGES_HPP
