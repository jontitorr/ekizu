#include <ekizu/json_util.hpp>
#include <ekizu/request/get_user.hpp>

namespace ekizu {

GetUser::GetUser(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake user_id)
	: m_user_id{user_id}, m_make_request{make_request} {}

GetUser::operator net::HttpRequest() const {
	return net::HttpRequest{
		net::HttpMethod::get, fmt::format("/users/{}", m_user_id), 11};
}

Result<User> GetUser::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));
	return json_util::deserialize<User>(res.body());
}
}  // namespace ekizu