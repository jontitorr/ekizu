#include <ekizu/json_util.hpp>
#include <ekizu/request/get_current_user.hpp>

namespace ekizu {
GetCurrentUser::GetCurrentUser(
	const std::function<void(net::HttpRequest,
							 std::function<void(net::HttpResponse)>)>
		&make_request)
	: m_make_request{make_request} {}

GetCurrentUser::operator net::HttpRequest() const {
	return net::HttpRequest{net::HttpMethod::get, "/users/@me", 11};
}

void GetCurrentUser::send(std::function<void(User)> cb) const {
	if (!m_make_request) { return; }

	m_make_request(*this, [cb = std::move(cb)](const net::HttpResponse &res) {
		auto msg = json_util::deserialize<User>(res.body());
		if (cb && msg) { cb(msg.value()); }
	});
}
}  // namespace ekizu
