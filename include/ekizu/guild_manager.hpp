#ifndef EKIZU_GUILD_MANAGER_HPP
#define EKIZU_GUILD_MANAGER_HPP

#include <ekizu/function_view.hpp>
#include <ekizu/guild.hpp>
#include <ekizu/lru_cache.hpp>
#include <mutex>

namespace ekizu
{
struct Client;
struct GuildManagerAttorney;

struct GuildManager {
	struct CreateKey {
	    private:
		CreateKey() = default;
	};

	GuildManager(CreateKey, Client &client);

    private:
	friend GuildManagerAttorney;

	void add(const nlohmann::json &data, FunctionView<void(Guild &)> cb);

	Client &m_client;
	std::optional<LruCache<Snowflake, Guild, Snowflake::HashFunction> >
		m_cache;
	std::unique_ptr<std::mutex> m_mtx{ std::make_unique<std::mutex>() };
};

struct GuildCreate;

struct GuildManagerAttorney {
    private:
	friend GuildCreate;

	static void add(GuildManager &manager, const nlohmann::json &data,
			FunctionView<void(Guild &)> cb)
	{
		manager.add(data, cb);
	}
};
} // namespace ekizu

#endif // EKIZU_GUILD_MANAGER_HPP
