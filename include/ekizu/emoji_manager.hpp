// #ifndef EKIZU_EMOJI_MANAGER_HPP
// #define EKIZU_EMOJI_MANAGER_HPP

// #include <ekizu/emoji.hpp>
// #include <ekizu/function_view.hpp>
// #include <ekizu/lru_cache.hpp>
// #include <mutex>
// #include <optional>

// namespace ekizu {
// struct Client;
// struct EmojiManagerAttorney;

// struct EmojiManager {

//     EmojiManager(Client& client);

// private:
//     friend EmojiManagerAttorney;

//     void add(const nlohmann::json& data, FunctionView<void(Emoji&)> cb);

//     Client& m_client;
//     std::optional<LruCache<Snowflake, Emoji, Snowflake::HashFunction>>
//     m_cache; std::unique_ptr<std::mutex> m_mtx {
//     std::make_unique<std::mutex>() };
// };

// struct Ready;

// struct EmojiManagerAttorney {
// private:
//     friend Ready;

//     static void add(EmojiManager& manager, const nlohmann::json& data,
//         FunctionView<void(Emoji&)> cb)
//     {
//         manager.add(data, cb);
//     }
// };
// } // namespace ekizu

// #endif // EKIZU_EMOJI_MANAGER_HPP
