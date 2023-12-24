#include <boost/asio/detached.hpp>
#include <boost/certify/https_verification.hpp>
#include <boost/outcome/try.hpp>
#include <boost/url/parse.hpp>
#include <ekizu/ws.hpp>

namespace ekizu::net {
namespace http = beast::http;
namespace outcome = boost::outcome_v2;
namespace ssl = boost::asio::ssl;

Result<void> WebSocketClient::run(asio::io_context &ioc) {
	BOOST_OUTCOME_TRY(auto uri, boost::urls::parse_uri(m_url));

	if (!uri.has_scheme() || (uri.scheme() != "ws" && uri.scheme() != "wss")) {
		auto ec = boost::asio::error::make_error_code(
			boost::asio::error::invalid_argument);
		fail(ec, "invalid scheme");
		return ec;
	}

	m_ctx = ioc;
	m_resolver.emplace(ioc);
	std::string_view port;

	if (uri.scheme() == "ws") {
		m_ws.emplace(ws::stream<tcp_stream>(ioc));
		port = "80";
	} else {
		ssl::context ctx{ssl::context::tlsv12_client};
		boost::system::error_code ec;

		if (ctx.set_verify_mode(ssl::context::verify_peer |
									ssl::context::verify_fail_if_no_peer_cert,
								ec) ||
			ctx.set_default_verify_paths(ec)) {
			fail(ec, "set_default_verify_paths");
			return ec;
		}

		boost::certify::enable_native_https_server_verification(ctx);

		m_ws.emplace(ws::stream<ssl_stream>(ioc, ctx));
		port = "443";
	}

	m_host = uri.host();
	m_path = uri.path() + (uri.query().empty() ? "" : "?") + uri.query();

	if (m_path.empty()) { m_path = "/"; }

	std::visit(
		[this, port](auto &&stream) {
			boost::asio::spawn(
				*m_ctx,
				[this, port, &stream](const boost::asio::yield_context &yield) {
					do {
						boost::system::error_code ec;

						// Look up the domain name
						const auto results =
							m_resolver->async_resolve(m_host, port, yield[ec]);

						if (ec) { return fail(ec, "resolve"); }

						// Set the timeout for the operation
						beast::get_lowest_layer(stream).expires_after(
							std::chrono::seconds(30));

						// Make the connection on the IP address we get
						// from a lookup
						const auto ep =
							beast::get_lowest_layer(stream).async_connect(
								results, yield[ec]);

						if (ec) { return fail(ec, "connect"); }

						// Set SNI Hostname (many hosts need this to
						// handshake successfully)
						using T = std::decay_t<decltype(stream)>;

						if constexpr (std::is_same_v<T,
													 ws::stream<ssl_stream>>) {
							if (!SSL_set_tlsext_host_name(
									stream.next_layer().native_handle(),
									m_host.c_str())) {
								ec = beast::error_code(
									static_cast<int>(::ERR_get_error()),
									asio::error::get_ssl_category());
								return fail(ec, "connect");
							}
						}

						// Perform the SSL handshake
						if constexpr (std::is_same_v<T,
													 ws::stream<ssl_stream>>) {
							// Set a timeout on the operation
							beast::get_lowest_layer(stream).expires_after(
								std::chrono::seconds(30));
							stream.next_layer().async_handshake(
								ssl::stream_base::client, yield[ec]);
						}

						if (ec) { return fail(ec, "ssl_handshake"); }

						// Turn off the timeout on the tcp_stream,
						// because the websocket stream has its own
						// timeout system.
						beast::get_lowest_layer(stream).expires_never();

						// Set suggested timeout settings for the
						// websocket
						stream.set_option(ws::stream_base::timeout::suggested(
							beast::role_type::client));

						// Set a decorator to change the User-Agent of
						// the handshake
						stream.set_option(ws::stream_base::decorator(
							[](ws::request_type &req) {
								req.set(
									http::field::user_agent,
									std::string(BOOST_BEAST_VERSION_STRING) +
										" websocket-client-async-"
										"ssl");
							}));

						// Perform the websocket handshake
						stream.async_handshake(
							m_host + ':' + std::to_string(ep.port()), m_path,
							yield[ec]);

						if (ec) { return fail(ec, "handshake"); }
						if (m_on_connect) { m_on_connect(); }

						stream.async_read(m_buffer, yield[ec]);

						while (stream.is_open()) {
							if (ec) { return fail(ec, "read"); }
							if (m_on_message) {
								m_on_message(boost::span<const std::byte>{
									reinterpret_cast<const std::byte *>(
										m_buffer.data().data()),
									m_buffer.size()});
							}
							m_buffer.consume(m_buffer.size());
							stream.async_read(m_buffer, yield[ec]);
						}

						if (m_on_close) { m_on_close(stream.reason()); }
					} while (m_auto_reconnect);
				},
				boost::asio::detached);
		},
		*m_ws);

	return outcome::success();
}

void WebSocketClient::close(ws::close_reason reason) {
	if (!m_ctx || !m_ws) { return; }

	boost::asio::spawn(
		*m_ctx,
		[this,
		 reason = std::move(reason)](const boost::asio::yield_context &yield) {
			std::visit(
				[this, &reason, &yield](auto &&stream) {
					boost::system::error_code ec;
					stream.async_close(reason, yield[ec]);

					if (ec) { fail(ec, "close"); }
				},
				*m_ws);
		},
		boost::asio::detached);
}

void WebSocketClient::do_send(const boost::asio::yield_context &yield) {
	if (!m_ws) { return; }

	std::visit(
		[this, &yield](auto &&stream) {
			boost::system::error_code ec;

			stream.async_write(asio::buffer(m_queue.front()), yield[ec]);

			if (ec) { return fail(ec, "write"); }

			m_queue.pop_front();

			if (!m_queue.empty()) { do_send(yield); }
		},
		*m_ws);
}

void WebSocketClient::fail(boost::system::error_code ec,
						   std::string_view what) {
	if (m_on_error) { m_on_error(ec, what); }
}

void WebSocketClient::send(std::string_view message) {
	boost::asio::spawn(
		*m_ctx,
		[this, message](const boost::asio::yield_context &yield) {
			bool write_in_progress = !m_queue.empty();
			m_queue.emplace_back(message);
			if (!write_in_progress) { do_send(yield); }
		},
		boost::asio::detached);
}

WebSocketClient::WebSocketClient(
	bool auto_reconnect, std::function<void()> on_connect,
	std::function<void(ws::close_reason)> on_close,
	std::function<void(boost::system::error_code, std::string_view)> on_error,
	std::function<void(boost::span<const std::byte>)> on_message,
	std::string url)
	: m_auto_reconnect{auto_reconnect},
	  m_on_connect{std::move(on_connect)},
	  m_on_close{std::move(on_close)},
	  m_on_error{std::move(on_error)},
	  m_on_message{std::move(on_message)},
	  m_url{std::move(url)} {}

WebSocketClient WebSocketClientBuilder::build() {
	return WebSocketClient(
		m_auto_reconnect, std::move(m_on_connect), std::move(m_on_close),
		std::move(m_on_error), std::move(m_on_message), std::move(m_url));
}
}  // namespace ekizu::net