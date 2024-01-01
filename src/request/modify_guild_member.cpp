#include <ekizu/json_util.hpp>
#include <ekizu/request/modify_guild_member.hpp>

namespace ekizu {
using json_util::serialize;

void to_json(nlohmann::json &j, const ModifyGuildMemberFields &m) {
	serialize(j, "nick", m.nick);
	serialize(j, "roles", m.roles);
	serialize(j, "mute", m.mute);
	serialize(j, "deaf", m.deaf);
	serialize(j, "channel_id", m.channel_id);
	serialize(
		j, "communication_disabled_until", m.communication_disabled_until);
	serialize(j, "flags", m.flags);
}

ModifyGuildMember::ModifyGuildMember(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id, Snowflake user_id)
	: m_guild_id{guild_id}, m_user_id{user_id}, m_make_request{make_request} {}

ModifyGuildMember::operator net::HttpRequest() const {
	net::HttpRequest req{
		net::HttpMethod::patch,
		fmt::format("/guilds/{}/members/{}", m_guild_id, m_user_id), 11,
		static_cast<nlohmann::json>(m_fields).dump()};

	req.set(net::http::field::content_type, "application/json");
	req.prepare_payload();

	return req;
}

Result<GuildMember> ModifyGuildMember::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<GuildMember>(res.body());
}
}  // namespace ekizu
