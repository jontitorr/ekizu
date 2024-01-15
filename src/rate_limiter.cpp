#include <boost/asio/steady_timer.hpp>
#include <ekizu/json_util.hpp>
#include <ekizu/rate_limiter.hpp>

namespace ekizu {
RateLimiter::RateLimiter(std::function<Result<net::HttpResponse>(
							 net::HttpRequest, const asio::yield_context &)>
							 send_fn)
	: m_send_fn{std::move(send_fn)} {}

Result<net::HttpResponse> RateLimiter::send(
	const DiscordApiRequest &req, const boost::asio::yield_context &yield) {
	{
		std::unique_lock lk{m_mtx};
		auto &rate_limits = m_rate_limits[req.inner.method()];

		if (rate_limits.find(req.inner.target()) != rate_limits.end()) {
			auto &rate_limit = rate_limits[req.inner.target()];

			if (auto now = std::chrono::system_clock::now();
				rate_limit.remaining == 0 && now < rate_limit.reset_time) {
				lk.unlock();
				asio::steady_timer t{yield.get_executor()};
				t.expires_after(rate_limit.reset_time - now);
				t.async_wait(yield);
				lk.lock();
			}
		}
	}

	if (!m_send_fn) { return boost::system::errc::operation_not_permitted; }

	EKIZU_TRY(auto res, m_send_fn(req.inner, yield));

	std::lock_guard lk{m_mtx};
	auto &rate_limits = m_rate_limits[req.inner.method()];
	const auto &headers = res.base();

	if (headers.find("X-RateLimit-Limit") != headers.end() &&
		headers.find("X-RateLimit-Remaining") != headers.end() &&
		headers.find("X-RateLimit-Reset") != headers.end()) {
		rate_limits.insert_or_assign(
			req.inner.target(),
			RateLimit{static_cast<uint16_t>(
						  std::stoul(headers.at("X-RateLimit-Limit"))),
					  static_cast<uint16_t>(
						  std::stoul(headers.at("X-RateLimit-Remaining"))),
					  std::chrono::system_clock::from_time_t(
						  std::stol(headers.at("X-RateLimit-Reset")))});
	}

	auto status = res.result();

	if (status >= net::HttpStatus::ok &&
		status < net::HttpStatus::multiple_choices) {
		m_rate_limited = false;
		return res;
	}

	if (status == net::HttpStatus::too_many_requests) { m_rate_limited = true; }

	return outcome::success();
}
}  // namespace ekizu
