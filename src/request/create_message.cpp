#include <ekizu/json_util.hpp>
#include <ekizu/request/create_message.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const CreateMessageFields &f) {
	serialize(j, "content", f.content);
	serialize(j, "nonce", f.nonce);
	serialize(j, "tts", f.tts);
	serialize(j, "embeds", f.embeds);
	serialize(j, "allowed_mentions", f.allowed_mentions);
	serialize(j, "message_reference", f.message_reference);
	serialize(j, "components", f.components);
	serialize(j, "sticker_ids", f.sticker_ids);
	serialize(j, "payload_json", f.payload_json);
	serialize(j, "attachments", f.attachments);
	serialize(j, "flags", f.flags);
}

void from_json(const nlohmann::json &j, CreateMessageFields &f) {
	deserialize(j, "content", f.content);
	deserialize(j, "nonce", f.nonce);
	deserialize(j, "tts", f.tts);
	deserialize(j, "embeds", f.embeds);
	deserialize(j, "allowed_mentions", f.allowed_mentions);
	deserialize(j, "message_reference", f.message_reference);
	deserialize(j, "components", f.components);
	deserialize(j, "sticker_ids", f.sticker_ids);
	deserialize(j, "payload_json", f.payload_json);
	deserialize(j, "attachments", f.attachments);
	deserialize(j, "flags", f.flags);
}

CreateMessage::CreateMessage(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake channel_id)
	: m_channel_id{channel_id}, m_make_request{make_request} {}

CreateMessage::operator net::HttpRequest() const {
	net::HttpRequest req{net::HttpMethod::post,
						 fmt::format("/channels/{}/messages", m_channel_id), 11,
						 static_cast<nlohmann::json>(m_fields).dump()};

	req.set(net::http::field::content_type, "application/json");
	req.prepare_payload();

	return req;
}

Result<Message> CreateMessage::send(const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	return json_util::deserialize<Message>(res.body());
}
}  // namespace ekizu