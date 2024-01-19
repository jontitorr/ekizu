# Creating a Discord Bot

Ekizu makes use of the [asio](https://github.com/chriskohlhoff/asio) library for
its asynchronous I/O. This allows us to achieve concurrency similar to other languages like JavaScript. For an easy wrapper around the interface, we take advantage of the `async_main` macro which wraps what would be your main function in a coroutine.

It is recommended to learn more about how asio works and to have a some [examples](https://github.com/chriskohlhoff/asio/tree/master/asio/src/examples) for more clarification.

Below is an example of a simple bot, utilizing the `async_main` macro.

```cpp
#include <ekizu/async_main.hpp>
#include <ekizu/http_client.hpp>
#include <ekizu/shard.hpp>

using namespace ekizu;

Result<> handle_event(const Event &ev, const HttpClient &http,
       const asio::yield_context &yield);

// Runs main in a coroutine, similar to Rust's #[tokio::main].
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

    // Catch the `MessageCreate` event, which contains our message.
   if constexpr (std::is_same_v<T, MessageCreate>) {
    const auto &[msg] = event;

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

Congratulations, you've successfully set up a bot!

To see what else you can do with this library, see [examples](https://github.com/jontitorr/ekizu/tree/main/examples).
