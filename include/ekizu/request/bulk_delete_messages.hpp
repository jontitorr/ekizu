#ifndef EKIZU_REQUEST_BULK_DELETE_MESSAGES_HPP
#define EKIZU_REQUEST_BULK_DELETE_MESSAGES_HPP

#include <ekizu/snowflake.hpp>
#include <net/http.hpp>

namespace ekizu
{
struct BulkDeleteMessages {
	BulkDeleteMessages(
		const std::function<Result<net::HttpResponse>(net::HttpRequest)>
			&make_request,
		Snowflake channel_id,
		const std::vector<Snowflake> &message_ids);

	operator net::HttpRequest() const;

	[[nodiscard]] Result<net::HttpResponse> send() const;

    private:
	Snowflake m_channel_id;
	std::vector<Snowflake> m_message_ids;
	std::function<Result<net::HttpResponse>(net::HttpRequest)>
		m_make_request;
};
} // namespace ekizu

#endif // EKIZU_REQUEST_BULK_DELETE_MESSAGES_HPP
