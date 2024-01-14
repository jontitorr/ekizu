#ifndef EKIZU_LRU_CACHE_HPP
#define EKIZU_LRU_CACHE_HPP

#include <boost/optional/optional.hpp>
#include <cstddef>
#include <ekizu/snowflake.hpp>
#include <list>
#include <unordered_map>

namespace ekizu {
/**
 * @brief A simple LRU cache implementation.
 *
 * @tparam K The key type.
 * @tparam V The value type.
 * @tparam Hash The hash function to use.
 */
template <typename K, typename V, typename Hash = std::hash<K> >
struct LruCache {
	using KeyValuePair = std::pair<K, V>;
	using ListIterator = typename std::list<KeyValuePair>::iterator;

	explicit LruCache(size_t capacity) : m_capacity{capacity} {}

	/**
	 * @brief Puts a key-value pair into the cache.
	 *
	 * @tparam Key The key type.
	 * @tparam Value The value type.
	 * @param key The key.
	 * @param value The value.
	 */
	template <typename Key, typename Value>
	void put(Key &&key, Value &&value) {
		static_assert(std::is_same_v<std::decay_t<Key>, K>);
		static_assert(std::is_same_v<std::decay_t<Value>, V>);

		m_cache_list.emplace_front(
			std::piecewise_construct,
			std::forward_as_tuple(std::forward<Key>(key)),
			std::forward_as_tuple(std::forward<Value>(value)));

		update_lru(key);
	}

	/**
	 * @brief Puts a key-value pair into the cache, this will construct the
	 * value in-place.
	 *
	 * @tparam Key The key type.
	 * @tparam Args The value constructor argument types.
	 * @param key The key.
	 * @param args The value constructor arguments.
	 */
	template <typename Key, typename... Args>
	void emplace(Key &&key, Args &&...args) {
		static_assert(std::is_same_v<std::decay_t<Key>, K>);
		static_assert(std::is_constructible_v<V, Args...>);

		m_cache_list.emplace_front(
			std::piecewise_construct, std::forward_as_tuple(key),
			std::forward_as_tuple(std::forward<Args>(args)...));

		update_lru(std::forward<Key>(key));
	}

	/**
	 * @brief Gets a value from the cache.
	 *
	 * @tparam Callback The callback type.
	 * @param key The key.
	 * @param cb The callback to call if the value is found.
	 */
	boost::optional<V &> get(const K &key) {
		auto it = m_cache_map.find(key);

		if (it == m_cache_map.end()) { return boost::none; }

		m_cache_list.splice(m_cache_list.begin(), m_cache_list, it->second);

		return it->second->second;
	}

	boost::optional<V &> operator[](const K &key) { return get(key); }

	/**
	 * @brief Removed a key-value pair from the cache.
	 *
	 * @param key The key.
	 */
	void remove(const K &key) {
		auto it = m_cache_map.find(key);

		if (it == m_cache_map.end()) { return; }

		m_cache_list.erase(it->second);
		m_cache_map.erase(it);
	}

	/**
	 * @brief Checks if a key exists in the cache.
	 *
	 * @param key The key.
	 * @return Whether the key exists in the cache.
	 */
	[[nodiscard]] bool has(const K &key) {
		return m_cache_map.find(key) != m_cache_map.end();
	}

	/**
	 * @brief Returns the size of the cache.
	 * @return The size of the cache.
	 */
	[[nodiscard]] size_t size() const { return m_cache_map.size(); }

	/**
	 * @brief Returns the capacity of the cache.
	 *
	 * @return The capacity of the cache.
	 */
	[[nodiscard]] size_t capacity() const { return m_capacity; }

	/**
	 * @brief Checks if the cache is empty.
	 *
	 * @return Whether the cache is empty.
	 */
	[[nodiscard]] bool empty() const { return m_cache_map.empty(); }

	typename std::list<KeyValuePair>::iterator begin() {
		return m_cache_list.begin();
	}

	typename std::list<KeyValuePair>::iterator end() {
		return m_cache_list.end();
	}

   private:
	template <typename Key>
	void update_lru(Key &&key) {
		if (auto it = m_cache_map.find(key); it != m_cache_map.end()) {
			m_cache_list.erase(it->second);
			m_cache_map.erase(it);
		}

		m_cache_map.insert_or_assign(
			std::forward<Key>(key), m_cache_list.begin());

		if (m_cache_map.size() > m_capacity) {
			m_cache_map.erase(m_cache_list.back().first);
			m_cache_list.pop_back();
		}
	}

	size_t m_capacity;
	std::list<KeyValuePair> m_cache_list;
	std::unordered_map<K, ListIterator, Hash> m_cache_map;
};

template <typename T>
using SnowflakeLruCache = LruCache<Snowflake, T>;
}  // namespace ekizu

#endif	// EKIZU_LRU_CACHE_HPP
