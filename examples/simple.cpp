#include <dotenv/dotenv.h>
#include <ekizu/lru_cache.hpp>
#include <ekizu/shard.hpp>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

using namespace ekizu;

void handle_event(Event e, SnowflakeLruCache<Message> &message_cache);

int main()
{
	dotenv::init();

	const auto token = dotenv::getenv("DISCORD_TOKEN");

	if (token.empty()) {
		fmt::print("No token :(\n");
		return 1;
	}

	const auto intents = Intents::AllIntents;
	auto shard = Shard{ ShardId::ONE, token, intents };
	auto message_cache = SnowflakeLruCache<Message>{ 500 };

	if (const auto res = shard.run([&message_cache](Event e) {
		    handle_event(std::move(e), message_cache);
	    });
	    !res) {
		fmt::print("Failed to run shard: {}\n", res.error().message());
		return 1;
	}
}

template <typename... Func> struct overload : Func... {
	using Func::operator()...;
};

template <typename... Func> overload(Func...) -> overload<Func...>;

void handle_event(Event ev, SnowflakeLruCache<Message> &message_cache)
{
	std::visit(
		overload{
			[](const Ready &r) {
				fmt::print("{} is ready!", r.user.username);
			},
			[&message_cache](MessageCreate msg) {
				fmt::print("{}'s message history:\n",
					   msg.message.author.username);

				// cache the message.
				message_cache.emplace(msg.message.id,
						      std::move(msg.message));

				//   Print the author's message history.
				for (const auto &[id, m] : message_cache) {
					fmt::print("{}: {}\n", id, m.content);
				}
			},
			[](Resumed) { std::cout << "Resumed Event Called\n"; },
			[](const auto &e) {
				fmt::print("Unhandled event: {}\n",
					   typeid(e).name());
			} },
		ev);
}