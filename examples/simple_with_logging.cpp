#include <dotenv/dotenv.h>
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

void handle_event(Event ev)
{
	const auto &logger = get_logger();

	std::visit(
		overload{ [&logger](const Ready &payload) {
				 logger->info(
					 fmt::format("Logged in as {}",
						     payload.user.username));

				 logger->info(fmt::format("API version: {}",
							  payload.v));
				 logger->info(fmt::format(
					 "Guilds: {}", payload.guilds.size()));

				 logger->info(fmt::format(
					 "Ready object: {}",
					 nlohmann::json{ payload }.dump()));
			 },
			  [&logger](const MessageCreate &payload) {
				  const auto &[msg] = payload;

				  logger->info(fmt::format("Message: {}",
							   msg.content));
			  },
			  [&logger](const Log &log) {
				  logger->debug(
					  fmt::format("Log: {}", log.message));
			  },
			  [&logger](const auto &e) {
				  logger->info(fmt::format("Unknown event: {}",
							   typeid(e).name()));
			  } },
		ev);
}

int main()
{
	dotenv::init();

	const auto token = dotenv::getenv("DISCORD_TOKEN");

	if (token.empty()) {
		get_logger()->error("No token :(");
		return 1;
	}

	const auto intents = Intents::AllIntents;
	auto shard = Shard{ ShardId::ONE, token, intents };

	if (const auto res = shard.run(handle_event); !res) {
		get_logger()->error("Failed to run shard: {}",
				    res.error().message());
		return 1;
	}
}