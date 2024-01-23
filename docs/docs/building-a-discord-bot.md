# Building a Discord Bot

## Prerequisites

You will need to have the following things installed:

- [CMake](https://cmake.org/download/) (version >= 3.16)
- Compiler with C++17 support, i.e. MSVC, GCC, Clang
- [Git](https://git-scm.com/)
- [ekizu](https://github.com/jontitorr/ekizu) (see [Installation](getting-started.md))

## Create a CMake project

In an empty directory, create the following files and directories:

- `CMakeLists.txt`
- `main.cpp`

You'll need to modify the `CMakeLists.txt` to tell CMake what it's looking for, and other information.
Here is an example CMake configuration, you can adapt it according to your needs:

```cmake
cmake_minimum_required(VERSION 3.16)
project(discord-bot VERSION 0.1.0)

# Create an executable
add_executable(discord-bot main.cpp)

# Find ekizu
find_package(ekizu REQUIRED)

# Link to the ekizu library
target_link_libraries(discord-bot ekizu::ekizu)
```

## The program

Ekizu makes use of the [asio](https://github.com/chriskohlhoff/asio) library for
its asynchronous I/O. This allows us to achieve concurrency similar to other languages like JavaScript. For an easy wrapper around the interface, we take advantage of the `async_main` macro which wraps what would be your main function in a coroutine.

It is recommended to learn more about how asio works and to have a some [examples](https://github.com/chriskohlhoff/asio/tree/master/asio/src/examples) for more clarification.

Below is an example of a simple bot, utilizing the `async_main` macro.

```cpp
#include <ekizu/async_main.hpp>
#include <ekizu/http_client.hpp>
#include <ekizu/shard.hpp>

using namespace ekizu;

Result<> handle_event(
    const Event& ev, const HttpClient& http, const asio::yield_context& yield);

// Runs main in a coroutine, similar to Rust's #[tokio::main].
async_main(const asio::yield_context& yield)
{
    // Place your token here.
    const std::string token { "YOUR_DISCORD_TOKEN_HERE" };
    HttpClient http { token };
    Shard shard { ShardId::ONE, token, Intents::AllIntents };

    while (true) {
        // Wait for the next gateway event.
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

        // Run this event in a coroutine.
        asio::spawn(
            yield,
            [e = std::move(res.value()), &http](
                auto y) { (void)handle_event(e, http, y); },
            asio::detached);
    }

    return outcome::success();
}

Result<> handle_event(
    const Event& ev, const HttpClient& http, const asio::yield_context& yield)
{
    std::visit(
        [&](auto&& event) mutable {
            using T = std::decay_t<decltype(event)>;

            // Catch the `MessageCreate` event, which contains our message.
            if constexpr (std::is_same_v<T, MessageCreate>) {
                const auto& [msg] = event;

                // If the message is "ping", respond with "pong".
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

For more feature-complete examples, see [examples](https://github.com/jontitorr/ekizu/tree/main/examples) or my bot [Saber](https://github.com/jontitorr/saber).

## Build the bot

For this process we need to do two things:

1. Configure CMake

    ```bash
    cmake -S. -Bbuild
    ```

2. Build the bot

    ```bash
    cmake --build build # add -j <# of cores> if you would like to use more cores when building
    ```

After that has finished, you can call:

```bash
build/discord-bot
```

???+ note
    The path of the executable will differ based on your configuration environment.

If everything went well, you should see your bot come online!
