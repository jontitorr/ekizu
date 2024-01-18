#ifndef EKIZU_REQUEST_INTERACTION_CREATE_RESPONSE_HPP
#define EKIZU_REQUEST_INTERACTION_CREATE_RESPONSE_HPP

#include <ekizu/http.hpp>
#include <ekizu/message.hpp>

namespace ekizu {
enum class InteractionResponseType : uint8_t {
	Pong = 1,
	ChannelMessageWithSource = 4,
	DeferredChannelMessageWithSource = 5,
	DeferredUpdateMessage = 6,
	UpdateMessage = 7,
	ApplicationCommandAutoCompleteResult = 8,
	Modal = 9
};

struct InteractionResponseData {
	std::optional<bool> tts;
	std::optional<std::string> content;
	std::optional<std::vector<Embed>> embeds;
	std::optional<AllowedMentions> allowed_mentions;
	std::optional<MessageFlags> flags;
	std::optional<std::vector<MessageComponent>> components;
	std::optional<std::vector<PartialAttachment>> attachments;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const InteractionResponseData &d);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
							InteractionResponseData &d);

struct InteractionResponse {
	InteractionResponseType type;
	std::optional<InteractionResponseData> data;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const InteractionResponse &r);
EKIZU_EXPORT void from_json(const nlohmann::json &j, InteractionResponse &r);

struct InteractionResponseBuilder {
	[[nodiscard]] InteractionResponse build() { return m_response; }

	InteractionResponseBuilder &type(InteractionResponseType type) {
		m_response.type = type;
		return *this;
	}

	InteractionResponseBuilder &tts(bool tts) {
		if (!m_response.data) { m_response.data.emplace(); }
		m_response.data->tts = tts;
		return *this;
	}

	InteractionResponseBuilder &content(std::string_view content) {
		if (!m_response.data) { m_response.data.emplace(); }
		m_response.data->content = content;
		return *this;
	}

	InteractionResponseBuilder &embeds(const std::vector<Embed> &embeds) {
		if (!m_response.data) { m_response.data.emplace(); }
		m_response.data->embeds = embeds;
		return *this;
	}

	InteractionResponseBuilder &allowed_mentions(
		const AllowedMentions &allowed_mentions) {
		if (!m_response.data) { m_response.data.emplace(); }
		m_response.data->allowed_mentions = allowed_mentions;
		return *this;
	}

	InteractionResponseBuilder &flags(MessageFlags flags) {
		if (!m_response.data) { m_response.data.emplace(); }
		m_response.data->flags = flags;
		return *this;
	}

	InteractionResponseBuilder &components(
		const std::vector<MessageComponent> &components) {
		if (!m_response.data) { m_response.data.emplace(); }
		m_response.data->components = components;
		return *this;
	}

	InteractionResponseBuilder &attachments(
		const std::vector<PartialAttachment> &attachments) {
		if (!m_response.data) { m_response.data.emplace(); }
		m_response.data->attachments = attachments;
		return *this;
	}

   private:
	InteractionResponse m_response;
};

struct CreateResponse {
	CreateResponse(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake interaction_id, std::string_view interaction_token,
		InteractionResponse response);

	operator net::HttpRequest() const;

	EKIZU_EXPORT Result<> send(const asio::yield_context &yield) const;

   private:
	Snowflake m_interaction_id;
	std::string m_interaction_token;
	InteractionResponse m_response;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_INTERACTION_CREATE_RESPONSE_HPP
