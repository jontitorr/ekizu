// #ifndef EKIZU_MESSAGE_MANAGER_HPP
// #define EKIZU_MESSAGE_MANAGER_HPP

// #include <ekizu/lru_cache.hpp>
// #include <ekizu/message.hpp>
// #include <mutex>

// namespace ekizu {
// struct Channel;
// struct MessageManagerAttorney;

// struct MessageManager {
//     struct CreateKey {
//     private:
//         friend Channel;
//         CreateKey() = default;
//     };

//     MessageManager(CreateKey, Client& client, Channel& channel);

// private:
//     friend MessageManagerAttorney;

//     void add(const nlohmann::json& data, FunctionView<void(Message&)> cb);

//     Client& m_client;
//     Channel& m_channel;
//     std::optional<LruCache<Snowflake, Message, Snowflake::HashFunction>>
//         m_cache;
//     std::unique_ptr<std::mutex> m_mtx { std::make_unique<std::mutex>() };
// };

// struct MessageCreate;

// struct MessageManagerAttorney {
// private:
//     friend MessageCreate;

//     static void add(MessageManager& manager, const nlohmann::json& data,
//         FunctionView<void(Message&)> cb)
//     {
//         manager.add(data, cb);
//     }
// };
// } // namespace ekizu

// #endif // EKIZU_MESSAGE_MANAGER_HPP
