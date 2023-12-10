#include <ekizu/json_util.hpp>
#include <ekizu/request/get_user.hpp>

namespace ekizu {

GetUser::GetUser(
	const std::function<Result<net::HttpResponse>(net::HttpRequest)>
		&make_request,
	Snowflake user_id)
	: m_user_id{user_id}, m_make_request{make_request} {}

GetUser::operator net::HttpRequest() const {
	return net::HttpRequest{
		net::HttpMethod::Get, fmt::format("/users/{}", m_user_id), {}, {}};
}

Result<User> GetUser::send() const {
	if (!m_make_request) {
		return tl::make_unexpected(
			std::make_error_code(std::errc::operation_not_permitted));
	}

	return m_make_request(*this).and_then([](auto res) {
		return json_util::try_parse(res.body).and_then(
			[](auto json) -> Result<User> {
				User user;
				json_util::detail::deserialize_impl<User>(json, user);
				return user;
			});
	});
}
}  // namespace ekizu