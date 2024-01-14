#include <ekizu/json_util.hpp>
#include <ekizu/request/edit_channel_permissions.hpp>

namespace ekizu {
EditChannelPermissions::EditChannelPermissions(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake channel_id, PermissionOverwrite overwrite)
	: m_channel_id{channel_id},
	  m_overwrite{overwrite},
	  m_make_request{make_request} {}

EditChannelPermissions::operator net::HttpRequest() const {
	net::HttpRequest req{net::HttpMethod::put,
						 fmt::format("/channels/{}/permissions/{}",
									 m_channel_id, m_overwrite.id),
						 11, static_cast<nlohmann::json>(m_overwrite).dump()};

	req.set(net::http::field::content_type, "application/json");
	req.prepare_payload();

	return req;
}

Result<> EditChannelPermissions::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));

	if (res.result() == net::HttpStatus::no_content) {
		return outcome::success();
	}

	return boost::system::errc::invalid_argument;
}
}  // namespace ekizu
