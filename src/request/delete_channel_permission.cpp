#include <ekizu/request/delete_channel_permission.hpp>

namespace ekizu {
DeleteChannelPermission::DeleteChannelPermission(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake channel_id, Snowflake overwrite_id)
	: m_channel_id{channel_id},
	  m_overwrite_id{overwrite_id},
	  m_make_request{make_request} {}

DeleteChannelPermission::operator net::HttpRequest() const {
	return {net::HttpMethod::delete_,
			fmt::format(
				"/channels/{}/permissions/{}", m_channel_id, m_overwrite_id),
			11};
}

Result<> DeleteChannelPermission::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	if (res.result() == net::HttpStatus::no_content) {
		return outcome::success();
	}

	return boost::system::errc::operation_not_permitted;
}
}  // namespace ekizu
