#include <ekizu/json_util.hpp>
#include <ekizu/request/create_dm.hpp>

namespace ekizu {
CreateDM::CreateDM(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake user_id)
	: m_make_request{make_request}, m_user_id{user_id} {}

CreateDM::operator net::HttpRequest() const {
	net::HttpRequest req{
		net::HttpMethod::post,
		"/users/@me/channels",
		11,
		nlohmann::json{
			{"recipient_id", m_user_id},
		}
			.dump(),
	};

	req.set(net::http::field::content_type, "application/json");
	req.prepare_payload();

	return req;
}

Result<Channel> CreateDM::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<Channel>(res.body());
}
}  // namespace ekizu
