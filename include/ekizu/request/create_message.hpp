#ifndef EKIZU_REQUEST_CREATE_MESSAGE_HPP
#define EKIZU_REQUEST_CREATE_MESSAGE_HPP

#include <ekizu/message.hpp>
#include <net/http.hpp>

namespace ekizu
{
struct CreateMessage {
	CreateMessage(std::function<Result<net::HttpResponse>(net::HttpRequest)>,
		      Snowflake channel_id);

	operator net::HttpRequest() const;

	CreateMessage &with_content(std::string_view content)
	{
		m_content = std::string{ content };
		return *this;
	}

	[[nodiscard]] Result<net::HttpResponse> send();

    private:
	Snowflake m_channel_id;
	std::optional<std::string> m_content;
	std::function<Result<net::HttpResponse>(net::HttpRequest)>
		m_make_request;
};
} // namespace ekizu

#endif // EKIZU_REQUEST_CREATE_MESSAGE_HPP
