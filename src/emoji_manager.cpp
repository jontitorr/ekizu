// #include <ekizu/client.hpp>
// #include <ekizu/emoji_manager.hpp>
// #include <ekizu/json_util.hpp>

// namespace ekizu {
// EmojiManager::EmojiManager(CreateKey /*unused*/, Client& client, Guild&
// guild)
//     : m_client { client }
//     , m_guild { guild }
// {
//     if (const auto& opts = client.options().cache_options; opts) {
//         m_cache.emplace(opts->emojis_capacity);
//     }
// }

// void EmojiManager::add(
//     const nlohmann::json& data, FunctionView<void(Emoji&)> cb)
// {
//     if (!json_util::not_null_recursive(data, "id")) {
//         return;
//     }

//     if (const Snowflake id = data["id"]; m_cache) {
//         std::scoped_lock lk { *m_mtx };

//         if (!m_cache->has(id)) {
//             m_cache->emplace(id, data);
//         }

//         return m_cache->get(id, [&data, &cb](Emoji& emoji) {
//             from_json(data, emoji);

//             if (cb) {
//                 cb(emoji);
//             }
//         });
//     }

//     Emoji emoji = data;

//     if (cb) {
//         cb(emoji);
//     }
// }
// } // namespace ekizu
