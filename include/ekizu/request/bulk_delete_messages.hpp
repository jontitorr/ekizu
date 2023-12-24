#ifndef EKIZU_REQUEST_BULK_DELETE_MESSAGES_HPP
#define EKIZU_REQUEST_BULK_DELETE_MESSAGES_HPP

#include <ekizu/http.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu {
struct BulkDeleteMessages {
	BulkDeleteMessages(
		const std::function<void(net::HttpRequest,
								 std::function<void(net::HttpResponse)>)>
			&make_request,
		Snowflake channel_id, const std::vector<Snowflake> &message_ids);

	operator net::HttpRequest() const;

	[[nodiscard]] Result<void> send(
		std::function<void(net::HttpResponse)> cb) const;

   private:
	Snowflake m_channel_id;
	std::vector<Snowflake> m_message_ids;
	std::function<void(
		net::HttpRequest, std::function<void(net::HttpResponse)>)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_BULK_DELETE_MESSAGES_HPP
