#ifndef EKIZU_REQUEST_DELETE_MESSAGE_HPP
#define EKIZU_REQUEST_DELETE_MESSAGE_HPP

#include <ekizu/snowflake.hpp>
#include <net/http.hpp>

namespace ekizu
{
struct DeleteMessage {
	DeleteMessage(
		const std::function<Result<net::HttpResponse>(net::HttpRequest)>
			&make_request,
		Snowflake channel_id, Snowflake message_id);

	operator net::HttpRequest() const;

	[[nodiscard]] Result<net::HttpResponse> send();

    private:
	Snowflake m_channel_id;
	Snowflake m_message_id;
	std::function<Result<net::HttpResponse>(net::HttpRequest)>
		m_make_request;
};
} // namespace ekizu

#endif // EKIZU_REQUEST_DELETE_MESSAGE_HPP
