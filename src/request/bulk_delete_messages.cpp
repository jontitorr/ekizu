#include <ekizu/request/bulk_delete_messages.hpp>
#include <nlohmann/json.hpp>

namespace ekizu {
BulkDeleteMessages::BulkDeleteMessages(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake channel_id, const std::vector<Snowflake> &message_ids)
	: m_channel_id{channel_id},
	  m_message_ids{message_ids},
	  m_make_request{make_request} {}

BulkDeleteMessages::operator net::HttpRequest() const {
	auto req = net::HttpRequest{
		net::HttpMethod::post,
		fmt::format("/channels/{}/messages/bulk-delete", m_channel_id), 11,
		nlohmann::json{{"messages", m_message_ids}}.dump()};

	req.set(net::http::field::content_type, "application/json");
	req.prepare_payload();

	return req;
}

Result<> BulkDeleteMessages::send(const asio::yield_context &yield) const {
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
