#include <ekizu/json_util.hpp>
#include <ekizu/request/interaction/create_response.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const InteractionResponseData &d) {
	serialize(j, "tts", d.tts);
	serialize(j, "content", d.content);
	serialize(j, "embeds", d.embeds);
	serialize(j, "allowed_mentions", d.allowed_mentions);
	serialize(j, "flags", d.flags);
	serialize(j, "components", d.components);
	serialize(j, "attachments", d.attachments);
}

void from_json(const nlohmann::json &j, InteractionResponseData &d) {
	deserialize(j, "tts", d.tts);
	deserialize(j, "content", d.content);
	deserialize(j, "embeds", d.embeds);
	deserialize(j, "allowed_mentions", d.allowed_mentions);
	deserialize(j, "flags", d.flags);
	deserialize(j, "components", d.components);
	deserialize(j, "attachments", d.attachments);
}

void to_json(nlohmann::json &j, const InteractionResponse &r) {
	serialize(j, "type", r.type);
	serialize(j, "data", r.data);
}

void from_json(const nlohmann::json &j, InteractionResponse &r) {
	deserialize(j, "type", r.type);
	deserialize(j, "data", r.data);
}

CreateResponse::CreateResponse(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake interaction_id, std::string_view interaction_token,
	InteractionResponse response)
	: m_make_request{make_request},
	  m_interaction_id{interaction_id},
	  m_interaction_token{interaction_token},
	  m_response{std::move(response)} {}

CreateResponse::operator net::HttpRequest() const {
	net::HttpRequest req{net::HttpMethod::post,
						 fmt::format("/interactions/{}/{}/callback",
									 m_interaction_id, m_interaction_token),
						 11, static_cast<nlohmann::json>(m_response).dump()};

	req.set(net::http::field::content_type, "application/json");
	req.prepare_payload();

	return req;
}

Result<> CreateResponse::send(const asio::yield_context &yield) const {
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
