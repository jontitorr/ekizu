#ifndef EKIZU_HTTP_HPP
#define EKIZU_HTTP_HPP

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4242)
#endif

#include <boost/beast/http.hpp>
#include <ekizu/util.hpp>

#ifdef _WIN32
#pragma warning(pop)
#endif

namespace ekizu::net {
namespace asio = boost::asio;
namespace http = boost::beast::http;
using HttpRequest = http::request<http::string_body>;
using HttpResponse = http::response<http::string_body>;
using HttpMethod = http::verb;
using HttpStatus = http::status;

struct HttpConnection {
	HttpConnection(const HttpConnection &) = delete;
	HttpConnection &operator=(const HttpConnection &) = delete;
	HttpConnection(HttpConnection &&) noexcept;
	HttpConnection &operator=(HttpConnection &&) noexcept;
	~HttpConnection();

	[[nodiscard]] static Result<HttpConnection> connect(
		asio::io_context &ioc, std::string_view url,
		std::function<void()> cb = {});

	Result<void> request(const HttpRequest &req,
						 std::function<void(HttpResponse)> cb);

	static Result<void> get(asio::io_context &ioc, std::string_view url,
							std::function<void(HttpResponse)> cb);

   private:
	struct Impl;

	explicit HttpConnection(std::shared_ptr<Impl> impl);

	std::shared_ptr<Impl> m_impl;
};
}  // namespace ekizu::net

#endif	// EKIZU_HTTP_HPP
