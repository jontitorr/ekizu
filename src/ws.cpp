#include <fmt/core.h>

#include <boost/asio/detached.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/certify/https_verification.hpp>
#include <boost/outcome/try.hpp>
#include <boost/url/parse.hpp>
#include <ekizu/ws.hpp>

namespace {
using namespace ekizu::net;
namespace http = beast::http;
namespace outcome = boost::outcome_v2;
namespace ssl = boost::asio::ssl;

ekizu::Result<std::variant<ws::stream<tcp_stream>, ws::stream<ssl_stream>>>
create_stream(bool ssl, asio::any_io_executor ioc) {
	if (!ssl) { return ws::stream<tcp_stream>(ioc); }

	ssl::context ctx{ssl::context::tlsv12_client};

	if (boost::system::error_code ec;
		ctx.set_verify_mode(ssl::context::verify_peer |
								ssl::context::verify_fail_if_no_peer_cert,
							ec) ||
		ctx.set_default_verify_paths(ec)) {
		return ec;
	}

	boost::certify::enable_native_https_server_verification(ctx);

	return ws::stream<ssl_stream>(ioc, ctx);
}

template <typename Stream>
ekizu::Result<> connect_stream(tcp::resolver &resolver, Stream &stream,
							   std::string_view host, std::string_view path,
							   bool ssl, const asio::yield_context &yield) {
	boost::system::error_code ec;

	// Look up the domain name
	const auto results =
		resolver.async_resolve(host, ssl ? "443" : "80", yield[ec]);

	if (ec) { return ec; }

	// Set the timeout for the operation
	beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));

	// Make the connection on the IP address we get
	// from a lookup
	const auto ep =
		beast::get_lowest_layer(stream).async_connect(results, yield[ec]);

	if (ec) { return ec; }

	// Set SNI Hostname (many hosts need this to
	// handshake successfully)
	if constexpr (std::is_same_v<Stream, ws::stream<ssl_stream>>) {
		if (!SSL_set_tlsext_host_name(
				stream.next_layer().native_handle(), host.data())) {
			ec = beast::error_code(static_cast<int>(::ERR_get_error()),
								   asio::error::get_ssl_category());
			return ec;
		}

		// Set a timeout on the operation
		beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));
		// Perform the SSL handshake
		stream.next_layer().async_handshake(
			ssl::stream_base::client, yield[ec]);

		if (ec) { return ec; }
	}

	// Turn off the timeout on the tcp_stream,
	// because the websocket stream has its own
	// timeout system.
	beast::get_lowest_layer(stream).expires_never();

	// Set suggested timeout settings for the
	// websocket
	stream.set_option(
		ws::stream_base::timeout::suggested(beast::role_type::client));

	// Set a decorator to change the User-Agent of
	// the handshake
	stream.set_option(ws::stream_base::decorator([](ws::request_type &req) {
		req.set(http::field::user_agent,
				std::string(BOOST_BEAST_VERSION_STRING) +
					" websocket-client-async-"
					"ssl");
	}));

	// Perform the websocket handshake
	stream.async_handshake(
		fmt::format("{}:{}", host, ep.port()), path, yield[ec]);

	if (ec) { return ec; }

	return outcome::success();
}
}  // namespace

namespace ekizu::net {

Result<WebSocketClient> WebSocketClient::connect(
	std::string_view url, const asio::yield_context &yield) {
	BOOST_OUTCOME_TRY(auto uri, boost::urls::parse_uri(url));

	if (!uri.has_scheme() || (uri.scheme() != "ws" && uri.scheme() != "wss")) {
		return boost::system::errc::invalid_argument;
	}

	auto host = uri.host();
	auto path = uri.path() + (uri.query().empty() ? "" : "?") + uri.query();
	tcp::resolver resolver{yield.get_executor()};
	auto ssl = uri.scheme() == "wss";

	if (path.empty()) { path = "/"; }

	BOOST_OUTCOME_TRY(auto stream, create_stream(ssl, yield.get_executor()));
	BOOST_OUTCOME_TRY(
		auto r,
		std::visit(
			[&host, &path, &yield, &resolver, ssl](auto &&s) {
				return connect_stream(resolver, s, host, path, ssl, yield);
			},
			stream));

	return WebSocketClient{std::move(resolver), std::move(stream),
						   std::move(host), std::move(path), ssl};
}
bool WebSocketClient::is_open() const {
	return std::visit([](auto &&s) { return s.is_open(); }, m_stream);
}

Result<std::string> WebSocketClient::read(
	const boost::asio::yield_context &yield) {
	boost::system::error_code ec;

	std::visit([this, &yield,
				&ec](auto &&stream) { stream.async_read(m_buffer, yield[ec]); },
			   m_stream);

	if (ec) { return ec; }

	std::string ret{boost::asio::buffer_cast<const char *>(m_buffer.data()),
					m_buffer.size()};

	m_buffer.consume(m_buffer.size());

	return ret;
}

Result<> WebSocketClient::close(ws::close_reason reason,
								const boost::asio::yield_context &yield) {
	if (m_closing) { return boost::system::errc::operation_in_progress; }

	return std::visit(
		[this, r = std::move(reason), &yield](auto &&stream) -> Result<> {
			boost::system::error_code ec;
			m_closing = true;
			stream.async_close(r, yield[ec]);
			m_closing = false;
			if (ec) { return ec; }
			return outcome::success();
		},
		m_stream);
}

Result<> WebSocketClient::send(std::string_view message,
							   const boost::asio::yield_context &yield) {
	boost::system::error_code ec;

	std::visit(
		[this, message, &yield, &ec](auto &&stream) {
			stream.async_write(boost::asio::buffer(message), yield[ec]);
		},
		m_stream);

	if (ec) { return ec; }

	return outcome::success();
}

WebSocketClient::WebSocketClient(
	tcp::resolver resolver,
	std::variant<ws::stream<tcp_stream>, ws::stream<ssl_stream>> ws,
	std::string host, std::string path, bool ssl)
	: m_resolver{std::move(resolver)},
	  m_stream{std::move(ws)},
	  m_host(std::move(host)),
	  m_path(std::move(path)),
	  m_ssl(ssl) {}
}  // namespace ekizu::net