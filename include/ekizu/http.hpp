#ifndef EKIZU_HTTP_HPP
#define EKIZU_HTTP_HPP

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4242)
#endif

#include <ekizu/export.h>

#include <boost/asio/spawn.hpp>
#include <boost/beast/http.hpp>
#include <boost/outcome/try.hpp>
#include <ekizu/util.hpp>

#ifdef _WIN32
#pragma warning(pop)
#endif

namespace ekizu {
namespace asio = boost::asio;
}  // namespace ekizu

namespace ekizu::net {
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
	EKIZU_EXPORT ~HttpConnection();

	EKIZU_EXPORT [[nodiscard]] static Result<HttpConnection> connect(
		std::string_view url, const asio::yield_context &yield);

	EKIZU_EXPORT Result<HttpResponse> request(const HttpRequest &req,
											  const asio::yield_context &yield);

	EKIZU_EXPORT static Result<HttpResponse> get(
		std::string_view url, const asio::yield_context &yield);

   private:
	struct Impl;

	explicit HttpConnection(std::shared_ptr<Impl> impl);

	std::shared_ptr<Impl> m_impl;
};
}  // namespace ekizu::net

#endif	// EKIZU_HTTP_HPP
