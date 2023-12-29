#ifndef EKIZU_REQUEST_EDIT_CHANNEL_PERMISSIONS_HPP
#define EKIZU_REQUEST_EDIT_CHANNEL_PERMISSIONS_HPP

#include <ekizu/channel.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct EditChannelPermissions {
	EditChannelPermissions(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id, PermissionOverwrite overwrite);

	operator net::HttpRequest() const;

	Result<> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	PermissionOverwrite m_overwrite;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_EDIT_CHANNEL_PERMISSIONS_HPP
