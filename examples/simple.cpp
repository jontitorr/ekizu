#include <dotenv/dotenv.h>

#include <ekizu/http_client.hpp>
#include <ekizu/shard.hpp>
#include <nlohmann/json.hpp>

using namespace ekizu;

std::function<void(Event)> handle_event(HttpClient &http);

int main() {
	dotenv::init();
	const auto token = dotenv::getenv("DISCORD_TOKEN");

	boost::asio::io_context ctx;
	auto http = HttpClient{ctx, token};
	auto shard = Shard{ctx, ShardId::ONE, token, Intents::AllIntents};

	if (const auto res = shard.connect(handle_event(http)); !res) {
		fmt::println("Failed to run shard: {}", res.error().message());
		return 1;
	}

	ctx.run();
}

template <typename... Func>
struct overload : Func... {
	using Func::operator()...;
};

template <typename... Func>
overload(Func...) -> overload<Func...>;

std::function<void(Event)> handle_event(HttpClient &http) {
	return [&http](Event ev) {
		std::visit(
			overload{[](const Ready &r) {
						 fmt::println("{} is ready!", r.user.username);
					 },
					 [&http](const MessageCreate &payload) {
						 const auto &[msg] = payload;

						 if (msg.content == "!echo") {
							 http.create_message(msg.channel_id)
								 .content(fmt::format(
									 "{} said: {}\nAvatar: {}",
									 msg.author.username, msg.content,
									 msg.author.avatar ? *msg.author.avatar
													   : "null"))
								 .send();
						 }
					 },
					 [](Resumed) { fmt::println("Resumed Event Called"); },
					 [](const auto &e) {
						 fmt::println("Uncaught {} event: {}", typeid(e).name(),
									  nlohmann::json{e}.dump());
					 }},
			ev);
	};
}