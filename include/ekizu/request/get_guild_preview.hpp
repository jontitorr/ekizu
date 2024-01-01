#ifndef EKIZU_REQUEST_GET_GUILD_PREVIEW_HPP
#define EKIZU_REQUEST_GET_GUILD_PREVIEW_HPP

#include <ekizu/guild_preview.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct GetGuildPreview {
	GetGuildPreview(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake guild_id);

	operator net::HttpRequest() const;

	// Add any additional parameters and methods specific to this route
	// For example:
	// GetGuildPreview &some_new_method(NewType new_param) {
	//     m_fields.new_param = new_param;
	//     return *this;
	// }

	[[nodiscard]] EKIZU_EXPORT Result<GuildPreview> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_GUILD_PREVIEW_HPP
