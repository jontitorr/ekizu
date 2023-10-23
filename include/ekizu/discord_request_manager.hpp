#ifndef EKIZU_DISCORD_REQUEST_MANAGER_HPP
#define EKIZU_DISCORD_REQUEST_MANAGER_HPP

#include <chrono>
#include <ekizu/export.h>
#include <ekizu/log_level.hpp>
#include <ekizu/util.hpp>
#include <mutex>
#include <net/http.hpp>

namespace ekizu
{
/**
 * @brief Represents a Discord API request.
 */
struct DiscordApiRequest {
	/// The actual HTTP request to send to the Discord API.
	net::HttpRequest inner;
	/// Whether this request is for a bot account.
	bool bot{};
};

struct Client;
struct DiscordApi;
struct DiscordRequestManagerAttorney;

/**
 * @brief The DiscordRequestManager class is responsible for sending requests to
 * the Discord API. It also handles rate limits.
 */
struct DiscordRequestManager {
	EKIZU_EXPORT explicit DiscordRequestManager(
		std::optional<std::function<void(LogLevel, std::string_view)> >
			log_fn = {});

    private:
	friend DiscordRequestManagerAttorney;

	[[nodiscard]] Result<net::HttpResponse>
	send_request(const DiscordApiRequest &req);

	void log(LogLevel level, std::string_view message);

	std::optional<net::HttpConnection> m_http_connection;
	bool m_rate_limited{};
	std::optional<std::function<void(LogLevel, std::string_view)> > m_log_fn;

	struct RateLimit {
		uint16_t limit{};
		uint16_t remaining{};
		std::chrono::system_clock::time_point reset_time;
	};

	std::mutex m_mtx;
	std::unordered_map<net::HttpMethod,
			   std::unordered_map<std::string, RateLimit> >
		m_rate_limits;
};

struct DiscordRequestManagerAttorney {
	friend DiscordApi;

	[[nodiscard]] static Result<net::HttpResponse>
	send_request(DiscordRequestManager &manager,
		     const DiscordApiRequest &req)
	{
		return manager.send_request(req);
	}
};
} // namespace ekizu

#endif // EKIZU_DISCORD_REQUEST_MANAGER_HPP
