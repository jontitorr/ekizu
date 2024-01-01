#ifndef EKIZU_REQUEST_LIST_GUILD_MEMBERS_HPP
#define EKIZU_REQUEST_LIST_GUILD_MEMBERS_HPP

#include <ekizu/guild_member.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct ListGuildMembersFields {
	std::optional<uint16_t> limit;
	std::optional<Snowflake> after;
};

struct ListGuildMembers {
	ListGuildMembers(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake guild_id);

	operator net::HttpRequest() const;

	ListGuildMembers &after(Snowflake after) {
		m_fields.after = after;
		return *this;
	}

	ListGuildMembers &limit(uint16_t limit) {
		m_fields.limit = limit;
		return *this;
	}

	EKIZU_EXPORT Result<std::vector<GuildMember>> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	ListGuildMembersFields m_fields;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_LIST_GUILD_MEMBERS_HPP
