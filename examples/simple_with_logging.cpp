#include <dotenv/dotenv.h>
#include <ekizu/http_client.hpp>
#include <ekizu/shard.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

using namespace ekizu;

const std::shared_ptr<spdlog::logger> &get_logger()
{
	static const auto ret = [] {
		auto logger = spdlog::stdout_color_mt("logger");
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
		spdlog::set_level(spdlog::level::debug);

		return logger;
	}();

	return ret;
}

template <typename... Func> struct overload : Func... {
	using Func::operator()...;
};

template <typename... Func> overload(Func...) -> overload<Func...>;

std::function<void(Event)> handle_event(HttpClient &http);

int main()
{
	dotenv::init();

	const auto token = dotenv::getenv("DISCORD_TOKEN");

	if (token.empty()) {
		get_logger()->error("No token :(");
		return 1;
	}

	auto http = HttpClient{ token };
	const auto intents = Intents::AllIntents;
	auto shard = Shard{ ShardId::ONE, token, intents };

	if (const auto res = shard.run(handle_event(http)); !res) {
		get_logger()->error("Failed to run shard: {}",
				    res.error().message());
		return 1;
	}
}

std::function<void(Event)> handle_event(HttpClient &http)
{
	return [&http, bot_id = Snowflake{}](Event ev) mutable {
		const auto &logger = get_logger();

		std::visit(
			overload{
				[&logger, &bot_id](const Ready &payload) {
					const auto &user = payload.user;

					logger->info("Logged in as {}",
						     user.username);
					bot_id = user.id;

					logger->info("API version: {}",
						     payload.v);
					logger->info("Guilds: {}",
						     payload.guilds.size());

					logger->info("Ready object: {}",
						     nlohmann::json{ payload }
							     .dump());
				},
				[&logger, &http,
				 &bot_id](const MessageCreate &payload) {
					const auto &[msg] = payload;

					if (msg.author.id == bot_id) {
						return;
					}

					logger->info("Message: {}",
						     msg.content);

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
				[&logger](const Log &log) {
					logger->debug("Log: {}", log.message);
				},
				[&logger](const auto &e) {
					logger->info(
						"Uncaught {} event: {}",
						typeid(e).name(),
						nlohmann::json{ e }.dump());
				} },
			ev);
	};
}