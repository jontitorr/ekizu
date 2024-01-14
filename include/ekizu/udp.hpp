#ifndef EKIZU_UDP_HPP
#define EKIZU_UDP_HPP

#include <ekizu/export.h>

#include <boost/asio/spawn.hpp>
#include <boost/core/span.hpp>
#include <ekizu/util.hpp>

namespace ekizu::net {
struct UdpClient {
	UdpClient(const UdpClient&) = delete;
	UdpClient& operator=(const UdpClient&) = delete;
	EKIZU_EXPORT UdpClient(UdpClient&&) noexcept;
	EKIZU_EXPORT UdpClient& operator=(UdpClient&&) noexcept;
	EKIZU_EXPORT ~UdpClient();

	[[nodiscard]] EKIZU_EXPORT static Result<UdpClient> create(
		std::string_view host, std::string_view port,
		const boost::asio::yield_context& yield);

	EKIZU_EXPORT Result<> close();

	EKIZU_EXPORT Result<size_t> send(boost::span<const std::byte> data,
									 const boost::asio::yield_context& yield);

	EKIZU_EXPORT Result<std::string> receive(
		const boost::asio::yield_context& yield);

   private:
	struct Impl;

	explicit UdpClient(std::unique_ptr<Impl> impl);

	std::unique_ptr<Impl> m_impl;
};
}  // namespace ekizu::net

#endif	// EKIZU_UDP_HPP
