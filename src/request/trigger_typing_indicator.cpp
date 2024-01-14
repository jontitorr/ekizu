#include <ekizu/request/trigger_typing_indicator.hpp>

namespace ekizu {
TriggerTypingIndicator::TriggerTypingIndicator(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake channel_id)
	: m_channel_id{channel_id}, m_make_request{make_request} {}

TriggerTypingIndicator::operator net::HttpRequest() const {
	return {net::HttpMethod::post,
			fmt::format("/channels/{}/typing", m_channel_id), 11};
}

Result<> TriggerTypingIndicator::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));

	if (res.result() == net::HttpStatus::no_content) {
		return outcome::success();
	}

	return boost::system::errc::operation_not_permitted;
}
}  // namespace ekizu
