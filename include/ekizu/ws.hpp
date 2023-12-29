#ifndef EKIZU_WS_HPP
#define EKIZU_WS_HPP

#include <ekizu/export.h>

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

struct WebSocketMessage {
	/// The message payload.
	std::string payload;
	/// Whether or not the message is binary.
	bool is_binary;
};

struct WebSocketClient {
	EKIZU_EXPORT [[nodiscard]] static Result<WebSocketClient> connect(
		std::string_view url, const asio::yield_context &yield);
	EKIZU_EXPORT [[nodiscard]] bool is_open() const;
	EKIZU_EXPORT Result<> close(ws::close_reason reason,
								const asio::yield_context &yield);
	EKIZU_EXPORT Result<WebSocketMessage> read(
		const asio::yield_context &yield);
	EKIZU_EXPORT Result<> send(std::string_view message,
							   const asio::yield_context &yield);

   private:
	friend WebSocketClientBuilder;

	explicit WebSocketClient(
		tcp::resolver resolver,
		std::variant<ws::stream<tcp_stream>, ws::stream<ssl_stream>> ws,
		std::string host, std::string path, bool ssl);

	boost::optional<asio::io_context &> m_ctx;
	std::optional<tcp::resolver> m_resolver;
	std::variant<ws::stream<tcp_stream>, ws::stream<ssl_stream>> m_stream;
	beast::flat_buffer m_buffer;
	std::string m_host;
	std::string m_path;
	bool m_ssl{};
	std::deque<std::string> m_queue;
	bool m_closing{};
};
}  // namespace ekizu::net

#endif	// EKIZU_WS_HPP
