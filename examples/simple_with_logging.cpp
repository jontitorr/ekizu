#include <dotenv/dotenv.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <ekizu/embed_builder.hpp>
#include <ekizu/http_client.hpp>
#include <ekizu/shard.hpp>
#include <nlohmann/json.hpp>

using namespace ekizu;

const std::shared_ptr<spdlog::logger> &get_logger() {
	static const auto ret = [] {
		auto logger = spdlog::stdout_color_mt("logger");
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
		spdlog::set_level(spdlog::level::debug);

		return logger;
	}();

	return ret;
}

template <typename... Func>
struct overload : Func... {
	using Func::operator()...;
};

template <typename... Func>
overload(Func...) -> overload<Func...>;

std::function<void(Event)> handle_event(HttpClient &http);

int main() {
	dotenv::init();

	const auto token = dotenv::getenv("DISCORD_TOKEN");

	if (token.empty()) {
		get_logger()->error("No token :(");
		return 1;
	}

	boost::asio::io_context ctx;
	auto http = HttpClient{ctx, token};
	auto shard = Shard{ctx, ShardId::ONE, token, Intents::AllIntents};

	if (const auto res = shard.connect(handle_event(http)); !res) {
		get_logger()->error("Failed to run shard: {}", res.error().message());
		return 1;
	}

	ctx.run();
}

std::function<void(Event)> handle_event(HttpClient &http) {
	return [&http, bot_id = Snowflake{}](Event ev) mutable {
		const auto &logger = get_logger();

		std::visit(
			overload{
				[&logger, &bot_id](const Ready &payload) {
					const auto &user = payload.user;
					logger->info("Logged in as {}", user.username);
					bot_id = user.id;
					logger->info("API version: {}", payload.v);
					logger->info("Guilds: {}", payload.guilds.size());
				},
				[&logger, &http, &bot_id](const MessageCreate &payload) {
					const auto &[msg] = payload;
					if (msg.author.id == bot_id) { return; }

					http.create_message(msg.channel_id)
						.content(fmt::format(
							"{} said: {}\nAvatar: {}", msg.author.username,
							msg.content,
							msg.author.avatar ? *msg.author.avatar : "null"))
						.send([&logger](auto m) {
							logger->info("Sent message: {}", m.content);
						});

					if (msg.content == "delete me") {
						http.delete_message(msg.channel_id, msg.id).send();
					}
				},
				[&logger](const Log &log) { logger->debug(log.message); },
				[&logger](const auto &e) {
					logger->info("Uncaught {} event: {}", typeid(e).name(),
								 nlohmann::json{e}.dump());
				}},
			ev);
	};
}