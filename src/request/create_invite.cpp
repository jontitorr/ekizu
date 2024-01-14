#include <ekizu/json_util.hpp>
#include <ekizu/request/create_invite.hpp>

namespace ekizu {
void to_json(nlohmann::json &j, const CreateInviteFields &f) {}

void from_json(const nlohmann::json &j, CreateInviteFields &f) {}

CreateInvite::CreateInvite(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake channel_id)
	: m_channel_id{channel_id}, m_make_request{make_request} {}

CreateInvite::operator net::HttpRequest() const {
	auto req = net::HttpRequest{
		net::HttpMethod::post,
		fmt::format("/channels/{}/invites", m_channel_id), 11,
		static_cast<nlohmann::json>(m_fields).dump()};

	req.set(net::http::field::content_type, "application/json");
	req.prepare_payload();

	return req;
}

Result<Invite> CreateInvite::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));
	return json_util::deserialize<Invite>(res.body());
}
}  // namespace ekizu
