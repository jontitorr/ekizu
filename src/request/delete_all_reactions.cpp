#include <ekizu/json_util.hpp>
#include <ekizu/request/delete_all_reactions.hpp>

namespace ekizu {
DeleteAllReactions::DeleteAllReactions(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context&)>& make_request,
	Snowflake channel_id, Snowflake message_id)
	: m_channel_id{channel_id},
	  m_message_id{message_id},
	  m_make_request{make_request} {}

DeleteAllReactions::operator net::HttpRequest() const {
	return {net::HttpMethod::delete_,
			fmt::format("/channels/{}/messages/{}/reactions/{}", m_channel_id,
						m_message_id),
			11};
}

Result<> DeleteAllReactions::send(const asio::yield_context& yield) const {
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