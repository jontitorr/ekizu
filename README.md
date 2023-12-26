# Ekizu

A WIP C++ library for Discord applications.

## Features

- Fully asynchronous, perfect for single-threaded applications
- Low memory footprint
- Discord API v10 support
- Gateway support, with auto-reconnect
- JSON support, convert and serialize events/structures to/from JSON

## Usage/Examples

```cpp
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
 std::visit(
  [&](auto &&event) {
   using T = std::decay_t<decltype(event)>;

   if constexpr (std::is_same_v<T, MessageCreate>) {
    const auto &[msg_a] = event;
    const Message &msg = msg_a;

    if (msg.content == "ping") {
     (void)http.create_message(msg.channel_id)
      .content("pong")
      .send(yield);
    }
   }
  },
  ev);

 return outcome::success();
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
- [Boost (ASIO, Beast, Outcome, URL)](https://github.com/boostorg/boost) (comes bundled with project, unless you have it installed)
- [nlohmann_json](https://github.com/nlohmann/json) (comes bundled with project)
- [zlib](https://github.com/madler/zlib) (comes bundled with project, unless you have it installed)

## License

[MIT](https://choosealicense.com/licenses/mit/)
