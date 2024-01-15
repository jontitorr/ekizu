#include <boost/asio/ip/udp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <ekizu/udp.hpp>

namespace ekizu::net {
using asio::ip::udp;

struct UdpClient::Impl {
	Impl(udp::socket socket, udp::endpoint ep)
		: m_socket{std::move(socket)}, m_endpoint{std::move(ep)} {}

	~Impl() { (void)close(); }

	Result<> close() {
		boost::system::error_code ec;
		if (m_socket.close(ec)) { return ec; }
		return outcome::success();
	}

	Result<size_t> send(boost::span<const std::byte> data,
						const asio::yield_context& yield) {
		boost::system::error_code ec;
		auto sent =
			m_socket.async_send_to(asio::buffer(data), m_endpoint, yield[ec]);
		if (ec) { return ec; }
		return sent;
	}

	Result<std::string> receive(const asio::yield_context& yield) {
		boost::system::error_code ec;
		auto received = m_socket.async_receive_from(
			asio::buffer(m_buffer), m_endpoint, yield[ec]);
		if (ec) { return ec; }

		return {m_buffer.data(), received};
	}

   private:
	std::array<char, 1024> m_buffer{};
	udp::socket m_socket;
	udp::endpoint m_endpoint;
};

UdpClient::UdpClient(std::unique_ptr<Impl> impl) : m_impl{std::move(impl)} {}

UdpClient::UdpClient(UdpClient&&) noexcept = default;
UdpClient& UdpClient::operator=(UdpClient&&) noexcept = default;
UdpClient::~UdpClient() = default;

Result<UdpClient> UdpClient::create(std::string_view host,
									std::string_view port,
									const asio::yield_context& yield) {
	boost::system::error_code ec;
	udp::resolver resolver{yield.get_executor()};
	const auto results = resolver.async_resolve(host, port, yield[ec]);
	if (ec) { return ec; }

	udp::socket socket{yield.get_executor()};
	if (socket.open(udp::v4(), ec)) { return ec; }

	return UdpClient{std::make_unique<Impl>(std::move(socket), *results)};
}

Result<> UdpClient::close() { return m_impl->close(); }

Result<size_t> UdpClient::send(boost::span<const std::byte> data,
							   const asio::yield_context& yield) {
	return m_impl->send(data, yield);
}

Result<std::string> UdpClient::receive(const asio::yield_context& yield) {
	return m_impl->receive(yield);
}
}  // namespace ekizu::net
