# Ekizu

A WIP C++ library for Discord applications.

## Features

- Low memory footprint
- Discord API v10 support
- Gateway support, with auto-reconnect
- JSON support, convert and serialize events/structures to/from JSON

## Usage/Examples

```cpp
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
								"You said: {}",
								msg.content))
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
```

See [examples](https://github.com/Xminent/ekizu/tree/dev/examples).

## Getting Started

### Prerequisites

- [CMake](https://cmake.org/download/) (version >= 3.16)
- Compiler with C++17 support, i.e. MSVC, GCC, Clang

### Installing

This library uses [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) to manage dependencies. It is an amazing package manager for CMake projects and allows us to install the entire library using the following commands:

```bash
  git clone https://www.github.com/xminent/ekizu
  cd ekizu
  cmake -S . -B build
  cmake --build build --target install
```

From there you can simply integrate it into your CMake project like so:

```cmake
    find_package(ekizu REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ekizu::ekizu)
```

## Contributing

Contributions, issues and feature requests are welcome. After cloning and setting up project locally, you can just submit
a PR to this repo and it will be deployed once it's accepted.

Take full advantage of the [.clang-format](.clang-format) file located in the root of the project to ensure that your code is properly formatted.

## Dependencies

### Third party Dependencies

- [fmt](https://github.com/fmtlib/fmt) (comes bundled with project)
- [net](https://github.com/xminent/net) (comes bundled with project)
- [nlohmann_json](https://github.com/nlohmann/json) (comes bundled with project)
- [zlib](https://github.com/madler/zlib) (comes bundled with project, unless you have it installed)

## License

[MIT](https://choosealicense.com/licenses/mit/)