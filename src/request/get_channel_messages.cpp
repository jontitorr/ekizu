#include <ekizu/json_util.hpp>
#include <ekizu/request/get_channel_messages.hpp>

namespace ekizu {
GetChannelMessagesConfigured::GetChannelMessagesConfigured(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake channel_id, std::optional<uint16_t> limit, Type type,
	Snowflake message_id)
	: m_channel_id{channel_id},
	  m_limit{limit},
	  m_type{type},
	  m_message_id{message_id},
	  m_make_request{make_request} {}

GetChannelMessagesConfigured::operator net::HttpRequest() const {
	std::string_view type_str = [this] {
		switch (m_type) {
			case Type::Around: return "around";
			case Type::Before: return "before";
			case Type::After: return "after";
		}

		return "";
	}();

	auto url =
		m_limit ? fmt::format("/channels/{}/messages?limit={}&{}={}",
							  m_channel_id, *m_limit, type_str, m_message_id)
				: fmt::format("/channels/{}/messages?{}={}", m_channel_id,
							  type_str, m_message_id);

	return {net::HttpMethod::get, url, 11};
}

Result<std::vector<Message>> GetChannelMessagesConfigured::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<std::vector<Message>>(res.body());
}

GetChannelMessages::GetChannelMessages(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake channel_id)
	: m_channel_id{channel_id}, m_make_request{make_request} {}

GetChannelMessages::operator net::HttpRequest() const {
	auto url = m_limit ? fmt::format("/channels/{}/messages?limit={}",
									 m_channel_id, *m_limit)
					   : fmt::format("/channels/{}/messages", m_channel_id);

	return {net::HttpMethod::get, url, 11};
}

Result<std::vector<Message>> GetChannelMessages::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<std::vector<Message>>(res.body());
}
}  // namespace ekizu
