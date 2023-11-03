#ifndef EKIZU_RATE_LIMITER_HPP
#define EKIZU_RATE_LIMITER_HPP

#include <chrono>
#include <ekizu/export.h>
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
	bool bot{ true };
};

/**
 * @brief The RateLimiter class is responsible for rate limiting requests to the Discord API.
 */
struct RateLimiter {
	EKIZU_EXPORT explicit RateLimiter(
		std::function<Result<net::HttpResponse>(net::HttpRequest)>
			send_fn);

	[[nodiscard]] Result<net::HttpResponse>
	send(const DiscordApiRequest &req);

    private:
	std::function<Result<net::HttpResponse>(net::HttpRequest)> m_send_fn;
	bool m_rate_limited{};

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
} // namespace ekizu

#endif // EKIZU_RATE_LIMITER_HPP
