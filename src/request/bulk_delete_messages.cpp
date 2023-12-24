#include <ekizu/request/bulk_delete_messages.hpp>
#include <nlohmann/json.hpp>

namespace ekizu {
BulkDeleteMessages::BulkDeleteMessages(
	const std::function<void(net::HttpRequest,
							 std::function<void(net::HttpResponse)>)>
		&make_request,
	Snowflake channel_id, const std::vector<Snowflake> &message_ids)
	: m_channel_id{channel_id},
	  m_message_ids{message_ids},
	  m_make_request{make_request} {}

BulkDeleteMessages::operator net::HttpRequest() const {
	auto req = net::HttpRequest{
		net::HttpMethod::post,
		fmt::format("/channels/{}/messages/bulk-delete", m_channel_id), 11};

	req.body() = nlohmann::json{{"messages", m_message_ids}}.dump();

	return req;
}

Result<void> BulkDeleteMessages::send(
	std::function<void(net::HttpResponse)> cb) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	m_make_request(*this, [cb = std::move(cb)](const net::HttpResponse &res) {
		if (cb) { cb(res); }
	});

	return outcome::success();
}
}  // namespace ekizu
