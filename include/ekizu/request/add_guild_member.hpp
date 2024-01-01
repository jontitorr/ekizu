#ifndef EKIZU_REQUEST_ADD_GUILD_MEMBER_HPP
#define EKIZU_REQUEST_ADD_GUILD_MEMBER_HPP

#include <ekizu/guild_member.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct AddGuildMemberFields {
	std::string access_token;
	std::optional<std::string> nick;
	std::optional<std::vector<Snowflake>> roles;
	std::optional<bool> mute;
	std::optional<bool> deaf;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const AddGuildMemberFields &m);

struct AddGuildMember {
	AddGuildMember(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake guild_id, Snowflake user_id, std::string_view access_token);

	operator net::HttpRequest() const;

	AddGuildMember &nick(std::string nick) {
		m_fields.nick = std::move(nick);
		return *this;
	}

	AddGuildMember &roles(std::vector<Snowflake> roles) {
		m_fields.roles = std::move(roles);
		return *this;
	}

	AddGuildMember &mute(bool mute) {
		m_fields.mute = mute;
		return *this;
	}

	AddGuildMember &deaf(bool deaf) {
		m_fields.deaf = deaf;
		return *this;
	}

	EKIZU_EXPORT Result<GuildMember> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	Snowflake m_user_id;
	AddGuildMemberFields m_fields;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_ADD_GUILD_MEMBER_HPP
