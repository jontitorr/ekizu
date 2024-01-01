#ifndef EKIZU_REQUEST_SEARCH_GUILD_MEMBERS_HPP
#define EKIZU_REQUEST_SEARCH_GUILD_MEMBERS_HPP

#include <ekizu/guild_member.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct SearchGuildMembersFields {
	std::optional<std::string> query;
	std::optional<uint16_t> limit;
};

struct SearchGuildMembers {
	SearchGuildMembers(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake guild_id);

	operator net::HttpRequest() const;

	SearchGuildMembers &query(std::string_view query) {
		m_fields.query = query;
		return *this;
	}

	SearchGuildMembers &limit(uint16_t limit) {
		m_fields.limit = limit;
		return *this;
	}

	EKIZU_EXPORT Result<std::vector<GuildMember>> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	SearchGuildMembersFields m_fields;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_SEARCH_GUILD_MEMBERS_HPP
