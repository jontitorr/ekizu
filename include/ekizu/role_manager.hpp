#ifndef EKIZU_ROLE_MANAGER_HPP
#define EKIZU_ROLE_MANAGER_HPP

#include <ekizu/function_view.hpp>
#include <ekizu/lru_cache.hpp>
#include <ekizu/role.hpp>
#include <mutex>
#include <optional>

namespace ekizu
{
struct Client;
struct Guild;
struct RoleManagerAttorney;

struct RoleManager {
	struct CreateKey {
	    private:
		friend Guild;
		CreateKey() = default;
	};

	RoleManager(CreateKey, Client &client);

    private:
	friend RoleManagerAttorney;

	void add(const nlohmann::json &data, FunctionView<void(Role &)> cb);

	Client &m_client;
	std::optional<LruCache<Snowflake, Role, Snowflake::HashFunction> >
		m_cache;
	std::unique_ptr<std::recursive_mutex> m_mtx{
		std::make_unique<std::recursive_mutex>()
	};
};

struct Ready;

struct RoleManagerAttorney {
    private:
	friend Ready;

	static void add(RoleManager &manager, const nlohmann::json &data,
			FunctionView<void(Role &)> cb)
	{
		manager.add(data, cb);
	}
};
} // namespace ekizu

#endif // EKIZU_ROLE_MANAGER_HPP
