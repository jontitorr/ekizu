#include <ekizu/json_util.hpp>
#include <ekizu/request/get_current_user.hpp>

namespace ekizu {
GetCurrentUser::GetCurrentUser(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request)
	: m_make_request{make_request} {}

GetCurrentUser::operator net::HttpRequest() const {
	return net::HttpRequest{net::HttpMethod::get, "/users/@me", 11};
}

Result<User> GetCurrentUser::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<User>(res.body());
}
}  // namespace ekizu
