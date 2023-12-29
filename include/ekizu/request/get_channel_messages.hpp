#ifndef EKIZU_REQUEST_GET_CHANNEL_MESSAGES_HPP
#define EKIZU_REQUEST_GET_CHANNEL_MESSAGES_HPP

#include <ekizu/http.hpp>
#include <ekizu/message.hpp>

namespace ekizu {
struct GetChannelMessages;

struct GetChannelMessagesConfigured {
	enum class Type { Around, Before, After };

	EKIZU_EXPORT GetChannelMessagesConfigured(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id, std::optional<uint16_t> limit, Type type,
		Snowflake message_id);

	operator net::HttpRequest() const;

	GetChannelMessagesConfigured &limit(uint16_t limit) {
		m_limit = limit;
		return *this;
	}

	EKIZU_EXPORT [[nodiscard]] Result<std::vector<Message>> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	std::optional<uint16_t> m_limit;
	Type m_type;
	Snowflake m_message_id;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};

struct GetChannelMessages {
	EKIZU_EXPORT GetChannelMessages(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		Snowflake channel_id);

	operator net::HttpRequest() const;

	[[nodiscard]] GetChannelMessagesConfigured around(
		Snowflake message_id) const {
		return GetChannelMessagesConfigured{
			m_make_request, m_channel_id,
			m_limit,		GetChannelMessagesConfigured::Type::Around,
			message_id,
		};
	}

	[[nodiscard]] GetChannelMessagesConfigured before(
		Snowflake message_id) const {
		return GetChannelMessagesConfigured{
			m_make_request, m_channel_id,
			m_limit,		GetChannelMessagesConfigured::Type::Before,
			message_id,
		};
	}

	[[nodiscard]] GetChannelMessagesConfigured after(
		Snowflake message_id) const {
		return GetChannelMessagesConfigured{
			m_make_request, m_channel_id,
			m_limit,		GetChannelMessagesConfigured::Type::After,
			message_id,
		};
	}

	GetChannelMessages &limit(uint16_t limit) {
		m_limit = limit;
		return *this;
	}

	EKIZU_EXPORT [[nodiscard]] Result<std::vector<Message>> send(
		const asio::yield_context &yield) const;

   private:
	Snowflake m_channel_id;
	std::optional<uint16_t> m_limit;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_GET_CHANNEL_MESSAGES_HPP
