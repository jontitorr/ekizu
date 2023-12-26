#include <boost/asio/detached.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>
#include <boost/certify/https_verification.hpp>
#include <boost/outcome/try.hpp>
#include <boost/url/parse.hpp>
#include <deque>
#include <ekizu/http.hpp>
#include <iostream>

namespace {
using boost::asio::ip::tcp;
namespace beast = boost::beast;
namespace ssl = boost::asio::ssl;

// Report a failure
void fail(beast::error_code ec, char const *what) {
	std::cerr << what << ": " << ec.message() << "\n";
}
}  // namespace

namespace ekizu::net {
struct HttpConnection::Impl {
	Impl(asio::any_io_executor ex, ssl::context &ctx)
		: m_resolver{ex}, m_stream{ex, ctx} {}

	void run(std::string_view host, std::string_view port,
			 const asio::yield_context &yield) {
		if (!SSL_set_tlsext_host_name(m_stream.native_handle(), host.data())) {
			beast::error_code ec{static_cast<int>(::ERR_get_error()),
								 asio::error::get_ssl_category()};
			std::cerr << ec.message() << "\n";
			return;
		}

		boost::system::error_code ec;
		const auto results = m_resolver.async_resolve(host, port, yield[ec]);

		if (ec) { return fail(ec, "resolve"); }

		beast::get_lowest_layer(m_stream).expires_after(
			std::chrono::seconds(30));
		beast::get_lowest_layer(m_stream).async_connect(results, yield[ec]);

		if (ec) { return fail(ec, "connect"); }

		m_stream.async_handshake(ssl::stream_base::client, yield[ec]);

		if (ec) { return fail(ec, "handshake"); }
	}

	void do_request(boost::asio::yield_context yield) {
		boost::system::error_code ec;
		auto cb = std::move(m_queue.front().second);
		http::async_write(m_stream, m_queue.front().first, yield[ec]);

		if (ec) { return fail(ec, "write"); }

		http::async_read(m_stream, m_buffer, m_res, yield);
		m_queue.pop_front();

		if (ec) { return fail(ec, "read"); }
		if (cb) { cb(m_res); }
		if (!m_queue.empty()) { do_request(yield); }
	}

	Result<HttpResponse> request(const HttpRequest &req,
								 const asio::yield_context &yield) {
		boost::system::error_code ec;
		http::async_write(m_stream, req, yield[ec]);

		if (ec) { return ec; }

		m_res = {};
		http::async_read(m_stream, m_buffer, m_res, yield[ec]);

		if (ec) { return ec; }

		return m_res;
	}

   private:
	asio::ip::tcp::resolver m_resolver;
	beast::ssl_stream<beast::tcp_stream> m_stream;
	beast::flat_buffer m_buffer;
	HttpResponse m_res;
	std::deque<std::pair<HttpRequest, std::function<void(HttpResponse)>>>
		m_queue;
};

Result<HttpResponse> HttpConnection::request(const HttpRequest &req,
											 const asio::yield_context &yield) {
	return m_impl->request(req, yield);
}

HttpConnection::HttpConnection(HttpConnection &&) noexcept = default;
HttpConnection &HttpConnection::operator=(HttpConnection &&) noexcept = default;
HttpConnection::~HttpConnection() = default;

Result<HttpConnection> HttpConnection::connect(
	std::string_view url, const asio::yield_context &yield) {
	BOOST_OUTCOME_TRY(auto uri, boost::urls::parse_uri(url));

	if (uri.scheme() != "http" && uri.scheme() != "https") {
		return boost::system::errc::not_supported;
	}

	ssl::context ctx{ssl::context::tlsv12_client};
	boost::system::error_code ec;

	if (ctx.set_verify_mode(ssl::context::verify_peer |
								ssl::context::verify_fail_if_no_peer_cert,
							ec) ||
		ctx.set_default_verify_paths(ec)) {
		return ec;
	}

	boost::certify::enable_native_https_server_verification(ctx);

	auto impl = std::make_shared<Impl>(yield.get_executor(), ctx);
	impl->run(uri.host(), uri.scheme() == "http" ? "80" : "443", yield);

	return HttpConnection{std::move(impl)};
}

Result<HttpResponse> HttpConnection::get(std::string_view url,
										 const asio::yield_context &yield) {
	BOOST_OUTCOME_TRY(auto uri, boost::urls::parse_uri(url));
	BOOST_OUTCOME_TRY(auto conn, connect(url, yield));

	HttpRequest req{http::verb::get, uri.path().empty() ? "/" : uri.path(), 11};

	return conn.request(req, yield);
}

HttpConnection::HttpConnection(std::shared_ptr<Impl> impl)
	: m_impl{std::move(impl)} {}
}  // namespace ekizu::net