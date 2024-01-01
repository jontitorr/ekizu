#ifndef EKIZU_REQUEST_MODIFY_CURRENT_MEMBER_HPP
#define EKIZU_REQUEST_MODIFY_CURRENT_MEMBER_HPP

#include <ekizu/guild_member.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct ModifyCurrentMemberFields {
	/// Value to set user's nickname to.
	std::optional<std::string> nick;
};

EKIZU_EXPORT void to_json(nlohmann::json &j,
						  const ModifyCurrentMemberFields &m);

struct ModifyCurrentMember {
	ModifyCurrentMember(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake guild_id);

	operator net::HttpRequest() const;

	ModifyCurrentMember &nick(std::string_view nick) {
		m_fields.nick = nick;
		return *this;
	}

	EKIZU_EXPORT Result<GuildMember> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	ModifyCurrentMemberFields m_fields;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_MODIFY_CURRENT_MEMBER_HPP
