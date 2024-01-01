#ifndef EKIZU_REQUEST_GET_GUILD_BANS_HPP
#define EKIZU_REQUEST_GET_GUILD_BANS_HPP

#include <ekizu/ban.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct GetGuildBansFields {
	std::optional<uint64_t> limit;
	std::optional<Snowflake> before;
	std::optional<Snowflake> after;
};

struct GetGuildBans {
	GetGuildBans(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake guild_id);

	operator net::HttpRequest() const;

	GetGuildBans &limit(uint64_t limit) {
		m_fields.limit = limit;
		return *this;
	}

	GetGuildBans &before(Snowflake before) {
		m_fields.before = before;
		return *this;
	}

	GetGuildBans &after(Snowflake after) {
		m_fields.after = after;
		return *this;
	}

	EKIZU_EXPORT Result<std::vector<Ban>> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	GetGuildBansFields m_fields;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_GUILD_BANS_HPP
