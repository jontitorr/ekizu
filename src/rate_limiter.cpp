#include <ekizu/json_util.hpp>
#include <ekizu/rate_limiter.hpp>
#include <thread>

namespace ekizu
{
RateLimiter::RateLimiter(
	std::function<Result<net::HttpResponse>(net::HttpRequest)> send_fn)
	: m_send_fn{ std::move(send_fn) }

{
}

Result<net::HttpResponse> RateLimiter::send(const DiscordApiRequest &req)
{
	static constexpr uint8_t max_retries{ 5 };

	for (uint8_t i{}; i < max_retries; ++i) {
		std::unique_lock lk{ m_mtx };
		auto &rate_limits = m_rate_limits[req.inner.method];

		if (rate_limits.find(req.inner.path) != rate_limits.end()) {
			auto &rate_limit = rate_limits[req.inner.path];

			if (rate_limit.remaining == 0 &&
			    std::chrono::system_clock::now() <
				    rate_limit.reset_time) {
				lk.unlock();
				std::this_thread::sleep_until(
					rate_limit.reset_time);
				lk.lock();
			}
		}

		if (!m_send_fn) {
			return tl::make_unexpected(std::make_error_code(
				std::errc::operation_not_permitted));
		}

		auto res = m_send_fn(req.inner);

		if (!res) {
			return tl::make_unexpected(res.error());
		}

		if (res->headers.find("X-RateLimit-Limit") !=
			    res->headers.end() &&
		    res->headers.find("X-RateLimit-Remaining") !=
			    res->headers.end() &&
		    res->headers.find("X-RateLimit-Reset") !=
			    res->headers.end()) {
			rate_limits.insert_or_assign(
				req.inner.path,
				RateLimit{
					static_cast<uint16_t>(
						std::stoul(res->headers.at(
							"X-RateLimit-Limit"))),
					static_cast<uint16_t>(
						std::stoul(res->headers.at(
							"X-RateLimit-Remaining"))),
					std::chrono::system_clock::from_time_t(
						std::stol(res->headers.at(
							"X-RateLimit-Reset"))),
				});
		}

		if (res->status_code >= net::HttpStatus::Ok &&
		    res->status_code < net::HttpStatus::MultipleChoices) {
			m_rate_limited = false;
			return *res;
		}

		if (res->status_code == net::HttpStatus::TooManyRequests) {
			m_rate_limited = true;
			continue;
		}

		break;
	}

	return tl::make_unexpected(
		std::make_error_code(std::errc::protocol_error));
}
} // namespace ekizu
