#include <ekizu/json_util.hpp>
#include <ekizu/rate_limiter.hpp>

namespace ekizu {
RateLimiter::RateLimiter(
	std::function<void(net::HttpRequest,
					   std::function<void(net::HttpResponse)>)>
		send_fn)
	: m_send_fn{std::move(send_fn)} {}

Result<void> RateLimiter::send(const DiscordApiRequest &req,
							   std::function<void(net::HttpResponse)> cb) {
	{
		std::lock_guard lk{m_mtx};
		auto &rate_limits = m_rate_limits[req.inner.method()];

		if (rate_limits.find(req.inner.target()) != rate_limits.end()) {
			auto &rate_limit = rate_limits[req.inner.target()];

			if (rate_limit.remaining == 0 &&
				std::chrono::system_clock::now() < rate_limit.reset_time) {
				// lk.unlock();
				// std::this_thread::sleep_until(rate_limit.reset_time);
				// lk.lock();
			}
		}
	}

	if (!m_send_fn) { return boost::system::errc::operation_not_permitted; }

	m_send_fn(req.inner,
			  [this, req, cb = std::move(cb)](const net::HttpResponse &res) {
				  std::lock_guard lk{m_mtx};
				  auto &rate_limits = m_rate_limits[req.inner.method()];
				  const auto &headers = res.base();

				  if (headers.find("X-RateLimit-Limit") != headers.end() &&
					  headers.find("X-RateLimit-Remaining") != headers.end() &&
					  headers.find("X-RateLimit-Reset") != headers.end()) {
					  rate_limits.insert_or_assign(
						  req.inner.target(),
						  RateLimit{
							  static_cast<uint16_t>(
								  std::stoul(headers.at("X-RateLimit-Limit"))),
							  static_cast<uint16_t>(std::stoul(
								  headers.at("X-RateLimit-Remaining"))),
							  std::chrono::system_clock::from_time_t(
								  std::stol(headers.at("X-RateLimit-Reset"))),
						  });
				  }

				  auto status = res.result();

				  if (status >= net::HttpStatus::ok &&
					  status < net::HttpStatus::multiple_choices) {
					  m_rate_limited = false;
					  if (cb) { cb(res); }
				  }

				  if (status == net::HttpStatus::too_many_requests) {
					  m_rate_limited = true;
				  }
			  });

	return outcome::success();
}
}  // namespace ekizu
