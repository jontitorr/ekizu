#include <dotenv/dotenv.h>
#include <ekizu/http_client.hpp>
#include <ekizu/shard.hpp>
#include <nlohmann/json.hpp>

using namespace ekizu;

std::function<void(Event)> handle_event(HttpClient &http);

int main()
{
	dotenv::init();
	const auto token = dotenv::getenv("DISCORD_TOKEN");

	auto http = HttpClient{ token };
	const auto intents = Intents::AllIntents;
	auto shard = Shard{ ShardId::ONE, token, intents };

	if (const auto res = shard.run(handle_event(http)); !res) {
		fmt::println("Failed to run shard: {}", res.error().message());
		return 1;
	}
}

template <typename... Func> struct overload : Func... {
	using Func::operator()...;
};

template <typename... Func> overload(Func...) -> overload<Func...>;

std::function<void(Event)> handle_event(HttpClient &http)
{
	return [&http](Event ev) {
		std::visit(
			overload{
				[](const Ready &r) {
					fmt::println("{} is ready!",
						     r.user.username);
				},
				[&http](const MessageCreate &payload) {
					const auto &[msg] = payload;

					const auto res =
						http.create_message(
							    msg.channel_id)
							.with_content(fmt::format(
								"{} said: {}\nAvatar: {}",
								msg.author
									.username,
								msg.content,
								msg.author.avatar ?
									*msg.author
										 .avatar :
									"null"))
							.send();

					if (!res) {
						fmt::println(
							"Failed to send message: {}",
							res.error().message());
					}
				},
				[](Resumed) {
					fmt::println("Resumed Event Called");
				},
				[](const auto &e) {
					fmt::println(
						"Uncaught {} event: {}",
						typeid(e).name(),
						nlohmann::json{ e }.dump());
				} },
			ev);
	};
}