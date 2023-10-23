#ifndef EKIZU_BAN_MANAGER_HPP
#define EKIZU_BAN_MANAGER_HPP

#include <ekizu/ban.hpp>
#include <ekizu/function_view.hpp>
#include <ekizu/lru_cache.hpp>
#include <mutex>
#include <optional>

namespace ekizu
{
struct Client;
struct Guild;
struct BanManagerAttorney;

struct BanManager {
	struct CreateKey {
	    private:
		friend Guild;
		CreateKey() = default;
	};

	BanManager(CreateKey, Client &client, Guild &guild);

    private:
	friend BanManagerAttorney;

	void add(const nlohmann::json &data, FunctionView<void(Ban &)> cb);

	Client &m_client;
	Guild &m_guild;
	std::optional<LruCache<Snowflake, Ban, Snowflake::HashFunction> >
		m_cache;
	std::unique_ptr<std::mutex> m_mtx{ std::make_unique<std::mutex>() };
};

struct Ready;

struct BanManagerAttorney {
    private:
	friend Ready;

	static void add(BanManager &manager, const nlohmann::json &data,
			FunctionView<void(Ban &)> cb)
	{
		manager.add(data, cb);
	}
};
} // namespace ekizu

#endif // EKIZU_BAN_MANAGER_HPP
