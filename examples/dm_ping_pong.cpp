#include <ekizu/async_main.hpp>
#include <ekizu/http_client.hpp>
#include <ekizu/shard.hpp>

using namespace ekizu;

Result<> handle_event(const Event &ev, const HttpClient &http,
					  const asio::yield_context &yield);

async_main(const asio::yield_context &yield) {
	const std::string token{std::getenv("DISCORD_TOKEN")};
	HttpClient http{token};
	Shard shard{ShardId::ONE, token, Intents::AllIntents};

	while (true) {
		auto res = shard.next_event(yield);

		if (!res) {
			if (res.error().failed()) {
				fmt::println(
					"Failed to get next event: {}", res.error().message());
				return res.error();
			}
			// Could be handling a non-dispatch event.
			continue;
		}

		asio::spawn(
			yield,
			[e = std::move(res.value()), &http](auto y) {
				(void)handle_event(e, http, y);
			},
			asio::detached);
	}

	return outcome::success();
}

Result<> handle_event(const Event &ev, const HttpClient &http,
					  const asio::yield_context &yield) {
	return std::visit(
		[&](auto &&event) -> Result<> {
			using T = std::decay_t<decltype(event)>;

			if constexpr (std::is_same_v<T, MessageCreate>) {
				const auto &[msg_a] = event;
				const Message &msg = msg_a;

				if (msg.content == "ping") {
					EKIZU_TRY(
						auto dm, http.create_dm(msg.author.id).send(yield));
					EKIZU_TRY(
						auto m,
						http.create_message(dm.id).content("pong").send(yield));
				}
			}

			return outcome::success();
		},
		ev);
}