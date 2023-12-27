#include <boost/url/encode.hpp>
#include <boost/url/grammar/all_chars.hpp>
#include <ekizu/json_util.hpp>
#include <ekizu/request/get_reactions.hpp>

namespace ekizu {
GetReactions::GetReactions(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context&)>& make_request,
	Snowflake channel_id, Snowflake message_id, RequestReaction emoji)
	: m_channel_id{channel_id},
	  m_emoji{std::move(emoji)},
	  m_message_id{message_id},
	  m_make_request{make_request} {}

GetReactions::operator net::HttpRequest() const {
	auto query_str = [this] {
		std::string ret;

		if (m_after) { ret = fmt::format("?after={}", *m_after); }
		if (m_limit) {
			ret += fmt::format("{}limit={}", ret.empty() ? '?' : '&', *m_limit);
		}

		return ret;
	}();

	auto path = std::visit(
		[this](auto&& emoji) {
			using T = std::decay_t<decltype(emoji)>;

			if constexpr (std::is_same_v<T, CustomEmoji>) {
				return fmt::format(
					"/channels/{}/messages/{}/reactions/{}", m_channel_id,
					m_message_id,
					boost::urls::encode(
						fmt::format("{}:{}", emoji.name, emoji.id),
						boost::urls::grammar::all_chars));
			} else if constexpr (std::is_same_v<T, std::string>) {
				return fmt::format("/channels/{}/messages/{}/reactions/{}",
								   m_channel_id, m_message_id,
								   boost::urls::encode(
									   emoji, boost::urls::grammar::all_chars));
			} else {
				static_assert(sizeof(T) == 0);
			}
		},
		m_emoji);

	auto url = fmt::format("{}{}", path, query_str);

	return {net::HttpMethod::get, url, 11};
}

Result<void> GetReactions::send(const asio::yield_context& yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	if (res.result() == net::HttpStatus::no_content) {
		return outcome::success();
	}

	return boost::system::errc::invalid_argument;
}
}  // namespace ekizu