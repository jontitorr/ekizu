// #include <ekizu/client.hpp>
// #include <ekizu/json_util.hpp>
// #include <ekizu/message_manager.hpp>

// namespace ekizu {
// MessageManager::MessageManager(
//     CreateKey /*unused*/, Client& client, Channel& channel)
//     : m_client { client }
//     , m_channel { channel }
// {
//     if (const auto& opts = client.options().cache_options; opts) {
//         m_cache.emplace(opts->messages_capacity);
//     }
// }

// void MessageManager::add(
//     const nlohmann::json& data, FunctionView<void(Message&)> cb)
// {
//     if (!json_util::not_null_recursive(data, "id")) {
//         return;
//     }

//     if (const Snowflake id = data["id"]; m_cache) {
//         std::scoped_lock lk { *m_mtx };

//         if (!m_cache->has(id)) {
//             m_cache->emplace(id, data);
//         }

//         return m_cache->get(id, [&data, &cb](Message& message) {
//             from_json(data, message);

//             if (cb) {
//                 cb(message);
//             }
//         });
//     }

//     Message message = data;

//     if (cb) {
//         cb(message);
//     }
// }
// } // namespace ekizu
