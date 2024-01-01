#ifndef EKIZU_REQUEST_MODIFY_GUILD_MEMBER_HPP
#define EKIZU_REQUEST_MODIFY_GUILD_MEMBER_HPP

#include <ekizu/guild_member.hpp>
#include <ekizu/http.hpp>
#include <ekizu/member_flags.hpp>

namespace ekizu {
struct ModifyGuildMemberFields {
	std::optional<std::string> nick;
	std::optional<std::vector<Snowflake>> roles;
	std::optional<bool> mute;
	std::optional<bool> deaf;
	std::optional<Snowflake> channel_id;
	std::optional<std::string> communication_disabled_until;
	std::optional<MemberFlags> flags;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ModifyGuildMemberFields &m);

struct ModifyGuildMember {
	ModifyGuildMember(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake guild_id, Snowflake user_id);

	operator net::HttpRequest() const;

	ModifyGuildMember &nick(std::string_view nick) {
		m_fields.nick = nick;
		return *this;
	}

	ModifyGuildMember &roles(std::vector<Snowflake> roles) {
		m_fields.roles = roles;
		return *this;
	}

	ModifyGuildMember &mute(bool mute) {
		m_fields.mute = mute;
		return *this;
	}

	ModifyGuildMember &deaf(bool deaf) {
		m_fields.deaf = deaf;
		return *this;
	}

	ModifyGuildMember &channel_id(Snowflake channel_id) {
		m_fields.channel_id = channel_id;
		return *this;
	}

	ModifyGuildMember &communication_disabled_until(std::string_view until) {
		m_fields.communication_disabled_until = until;
		return *this;
	}

	ModifyGuildMember &flags(MemberFlags flags) {
		m_fields.flags = flags;
		return *this;
	}

	EKIZU_EXPORT Result<GuildMember> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_guild_id;
	Snowflake m_user_id;
	ModifyGuildMemberFields m_fields;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_MODIFY_GUILD_MEMBER_HPP
