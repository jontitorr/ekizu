#include <dotenv/dotenv.h>
#include <ekizu/shard.hpp>
#include <nlohmann/json.hpp>

using namespace ekizu;

void handle_event(Event e);

int main()
{
	dotenv::init();

	const auto intents = Intents::AllIntents;
	auto shard =
		Shard{ ShardId::ONE, dotenv::getenv("DISCORD_TOKEN"), intents };

	if (const auto res = shard.run(handle_event); !res) {
		fmt::print("Failed to run shard: {}\n", res.error().message());
		return 1;
	}
}

template <typename... Func> struct overload : Func... {
	using Func::operator()...;
};

template <typename... Func> overload(Func...) -> overload<Func...>;

void handle_event(Event ev)
{
	std::visit(
		overload{ [](const Ready &r) {
				 fmt::print("{} is ready!", r.user.username);
			 },
			  [](MessageCreate msg) {
				  fmt::print("Message: {}\n",
					     msg.message.content);
			  },
			  [](Resumed) { fmt::print("Resumed Event Called\n"); },
			  [](const auto &e) {
				  fmt::print("Unhandled event: {}\n",
					     typeid(e).name());
			  } },
		ev);
}