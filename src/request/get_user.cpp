#include <ekizu/json_util.hpp>
#include <ekizu/request/get_user.hpp>

namespace ekizu {

GetUser::GetUser(const std::function<void(
					 net::HttpRequest, std::function<void(net::HttpResponse)>)>
					 &make_request,
				 Snowflake user_id)
	: m_user_id{user_id}, m_make_request{make_request} {}

GetUser::operator net::HttpRequest() const {
	return net::HttpRequest{
		net::HttpMethod::get, fmt::format("/users/{}", m_user_id), 11};
}

void GetUser::send(std::function<void(User)> cb) const {
	if (!m_make_request) { return; }

	m_make_request(*this, [cb = std::move(cb)](net::HttpResponse res) {
		auto user = json_util::deserialize<User>(res.body());
		if (cb && user) { cb(user.value()); }
	});
}
}  // namespace ekizu