#ifndef EKIZU_REQUEST_DELETE_MESSAGE_HPP
#define EKIZU_REQUEST_DELETE_MESSAGE_HPP

#include <ekizu/http.hpp>
#include <ekizu/snowflake.hpp>

namespace ekizu {
struct DeleteMessage {
	DeleteMessage(const std::function<void(
					  net::HttpRequest, std::function<void(net::HttpResponse)>)>
					  &make_request,
				  Snowflake channel_id, Snowflake message_id);

	operator net::HttpRequest() const;

	void send(std::function<void()> cb = {}) const;

   private:
	Snowflake m_channel_id;
	Snowflake m_message_id;
	std::function<void(
		net::HttpRequest, std::function<void(net::HttpResponse)>)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_DELETE_MESSAGE_HPP
