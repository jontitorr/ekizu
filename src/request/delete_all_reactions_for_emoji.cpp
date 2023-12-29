#include <boost/url/encode.hpp>
#include <boost/url/grammar/all_chars.hpp>
#include <ekizu/json_util.hpp>
#include <ekizu/request/delete_all_reactions_for_emoji.hpp>

namespace ekizu {
DeleteAllReactionsForEmoji::DeleteAllReactionsForEmoji(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context&)>& make_request,
	Snowflake channel_id, Snowflake message_id, RequestReaction emoji)
	: m_channel_id{channel_id},
	  m_emoji{std::move(emoji)},
	  m_message_id{message_id},
	  m_make_request{make_request} {}

DeleteAllReactionsForEmoji::operator net::HttpRequest() const {
	auto url = std::visit(
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

	return {net::HttpMethod::delete_,
			fmt::format("/channels/{}/messages/{}/reactions/{}", m_channel_id,
						m_message_id),
			11};
}

Result<void> DeleteAllReactionsForEmoji::send(
	const asio::yield_context& yield) const {
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