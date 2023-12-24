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
struct HttpConnection::Impl : std::enable_shared_from_this<Impl> {
	Impl(asio::io_context &ex, ssl::context &ctx)
		: m_ctx{ex}, m_resolver{ex}, m_stream{ex, ctx} {}

	void run(std::string_view host, std::string_view port,
			 std::function<void()> on_handshake = {}) {
		boost::asio::spawn(
			m_ctx,
			[this, host, port, on_handshake = std::move(on_handshake)](
				const boost::asio::yield_context &yield) mutable {
				if (!SSL_set_tlsext_host_name(
						m_stream.native_handle(), host.data())) {
					beast::error_code ec{static_cast<int>(::ERR_get_error()),
										 asio::error::get_ssl_category()};
					std::cerr << ec.message() << "\n";
					return;
				}

				boost::system::error_code ec;
				m_on_handshake = std::move(on_handshake);
				const auto results =
					m_resolver.async_resolve(host, port, yield[ec]);

				if (ec) { return fail(ec, "resolve"); }

				beast::get_lowest_layer(m_stream).expires_after(
					std::chrono::seconds(30));
				beast::get_lowest_layer(m_stream).async_connect(
					results, yield[ec]);

				if (ec) { return fail(ec, "connect"); }

				m_stream.async_handshake(ssl::stream_base::client, yield[ec]);

				if (ec) { return fail(ec, "handshake"); }
				if (m_on_handshake) { m_on_handshake(); }
			},
			boost::asio::detached);
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

	Result<void> request(const HttpRequest &req,
						 std::function<void(HttpResponse)> cb) {
		boost::asio::spawn(
			m_ctx,
			[this, req, cb = std::move(cb)](
				const boost::asio::yield_context &yield) mutable {
				bool write_in_progress = !m_queue.empty();
				m_queue.emplace_back(req, std::move(cb));
				if (!write_in_progress) { do_request(yield); }
			},
			boost::asio::detached);
		return outcome::success();
	}

   private:
	asio::io_context &m_ctx;
	asio::ip::tcp::resolver m_resolver;
	beast::ssl_stream<beast::tcp_stream> m_stream;
	beast::flat_buffer m_buffer;
	HttpResponse m_res;
	std::function<void()> m_on_handshake;
	std::deque<std::pair<HttpRequest, std::function<void(HttpResponse)>>>
		m_queue;
};

Result<void> HttpConnection::request(const HttpRequest &req,
									 std::function<void(HttpResponse)> cb) {
	return m_impl->request(req, std::move(cb));
}

HttpConnection::HttpConnection(HttpConnection &&) noexcept = default;
HttpConnection &HttpConnection::operator=(HttpConnection &&) noexcept = default;
HttpConnection::~HttpConnection() = default;

Result<HttpConnection> HttpConnection::connect(
	asio::io_context &ioc, std::string_view url, std::function<void()> cb) {
	BOOST_OUTCOME_TRY(auto uri, boost::urls::parse_uri(url));

	if (uri.scheme() != "http" && uri.scheme() != "https") {
		return boost::system::errc::not_supported;
	}

	ssl::context ctx{ssl::context::tlsv12_client};
	boost::system::error_code ec;

	ctx.set_verify_mode(
		ssl::context::verify_peer | ssl::context::verify_fail_if_no_peer_cert,
		ec);

	if (ec) { return ec; }

	ctx.set_default_verify_paths(ec);

	if (ec) { return ec; }

	boost::certify::enable_native_https_server_verification(ctx);

	auto impl = std::make_shared<Impl>(ioc, ctx);
	impl->run(uri.host(), uri.scheme() == "http" ? "80" : "443", std::move(cb));

	return HttpConnection{std::move(impl)};
}

Result<void> HttpConnection::get(asio::io_context &ioc, std::string_view url,
								 std::function<void(HttpResponse)> cb) {
	BOOST_OUTCOME_TRY(auto uri, boost::urls::parse_uri(url));
	BOOST_OUTCOME_TRY(auto conn, connect(ioc, url));

	HttpRequest req{http::verb::get, uri.path().empty() ? "/" : uri.path(), 11};

	return conn.request(req, std::move(cb));
}

HttpConnection::HttpConnection(std::shared_ptr<Impl> impl)
	: m_impl{std::move(impl)} {}
}  // namespace ekizu::net