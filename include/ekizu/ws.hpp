#ifndef EKIZU_WS_HPP
#define EKIZU_WS_HPP

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/core/span.hpp>
#include <deque>
#include <ekizu/util.hpp>
#include <variant>

namespace ekizu::net {
namespace asio = boost::asio;
namespace beast = boost::beast;
using asio::ip::tcp;
namespace ws = beast::websocket;
using tcp_stream = beast::tcp_stream;
using ssl_stream = beast::ssl_stream<tcp_stream>;
using WebSocketCloseCode = ws::close_code;

struct WebSocketClientBuilder;

struct WebSocketClient {
	WebSocketClient &set_auto_reconnect(bool auto_reconnect) {
		m_auto_reconnect = auto_reconnect;
		return *this;
	}

	Result<void> run(asio::io_context &ioc);
	void close(ws::close_reason reason);
	void send(std::string_view message);

   private:
	friend WebSocketClientBuilder;

	explicit WebSocketClient(
		bool auto_reconnect, std::function<void()> on_connect,
		std::function<void(ws::close_reason)> on_close,
		std::function<void(boost::system::error_code, std::string_view)>
			on_error,
		std::function<void(boost::span<const std::byte>)> on_message,
		std::string url);

	void do_send(const asio::yield_context &yield);
	void fail(boost::system::error_code ec, std::string_view what);

	boost::optional<asio::io_context &> m_ctx;
	bool m_auto_reconnect{};
	std::optional<tcp::resolver> m_resolver;
	boost::optional<
		std::variant<ws::stream<tcp_stream>, ws::stream<ssl_stream>>>
		m_ws;
	beast::flat_buffer m_buffer;
	std::string m_host;
	std::string m_path;
	std::function<void()> m_on_connect;
	std::function<void(ws::close_reason)> m_on_close;
	std::function<void(boost::system::error_code, std::string_view)> m_on_error;
	std::function<void(boost::span<const std::byte>)> m_on_message;
	std::deque<std::string> m_queue;
	std::string m_url;
};

struct WebSocketClientBuilder {
	[[nodiscard]] WebSocketClient build();

	WebSocketClientBuilder &with_auto_reconnect(bool auto_reconnect) {
		m_auto_reconnect = auto_reconnect;
		return *this;
	}

	WebSocketClientBuilder &with_on_connect(std::function<void()> on_connect) {
		m_on_connect = std::move(on_connect);
		return *this;
	}

	WebSocketClientBuilder &with_on_close(
		std::function<void(ws::close_reason)> on_close) {
		m_on_close = std::move(on_close);
		return *this;
	}

	WebSocketClientBuilder &with_on_error(
		std::function<void(boost::system::error_code, std::string_view)>
			on_error) {
		m_on_error = std::move(on_error);
		return *this;
	}

	WebSocketClientBuilder &with_on_message(
		std::function<void(boost::span<const std::byte>)> on_message) {
		m_on_message = std::move(on_message);
		return *this;
	}

	WebSocketClientBuilder &with_url(std::string_view url) {
		m_url = url;
		return *this;
	}

   private:
	bool m_auto_reconnect{};
	std::function<void()> m_on_connect;
	std::function<void(ws::close_reason)> m_on_close;
	std::function<void(boost::system::error_code, std::string_view)> m_on_error;
	std::function<void(boost::span<const std::byte>)> m_on_message;
	std::string m_url;
};
}  // namespace ekizu::net

#endif	// EKIZU_WS_HPP
