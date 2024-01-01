#include <ekizu/json_util.hpp>
#include <ekizu/request/modify_current_member.hpp>

namespace ekizu {
using json_util::serialize;

void to_json(nlohmann::json &j, const ModifyCurrentMemberFields &m) {
	serialize(j, "nick", m.nick);
}

ModifyCurrentMember::ModifyCurrentMember(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id)
	: m_guild_id{guild_id}, m_make_request{make_request} {}

ModifyCurrentMember::operator net::HttpRequest() const {
	net::HttpRequest req{net::HttpMethod::patch,
						 fmt::format("/guilds/{}/members/@me", m_guild_id), 11,
						 static_cast<nlohmann::json>(m_fields).dump()};

	req.set(net::http::field::content_type, "application/json");
	req.prepare_payload();

	return req;
}

Result<GuildMember> ModifyCurrentMember::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<GuildMember>(res.body());
}
}  // namespace ekizu
