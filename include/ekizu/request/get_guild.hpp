#ifndef EKIZU_REQUEST_GET_GUILD_HPP
#define EKIZU_REQUEST_GET_GUILD_HPP

#include <ekizu/guild.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct GetGuildFields {
	std::optional<bool> with_counts{};
};

struct GetGuild {
	GetGuild(const std::function<Result<net::HttpResponse>(
				 net::HttpRequest, const asio::yield_context &)> &make_request,
			 Snowflake guild_id);

	operator net::HttpRequest() const;

	GetGuild &with_counts(bool with_counts) {
		m_fields.with_counts = with_counts;
		return *this;
	}

	[[nodiscard]] EKIZU_EXPORT Result<Guild> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	GetGuildFields m_fields;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_GUILD_HPP
