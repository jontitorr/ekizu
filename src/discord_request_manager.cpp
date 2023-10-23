#include <ekizu/discord_request_manager.hpp>
#include <ekizu/json_util.hpp>
#include <fmt/format.h>
#include <thread>

namespace
{
constexpr const char *DISCORD_BASE_URL = "https://discord.com";

nlohmann::json to_json(const ekizu::DiscordApiRequest &req)
{
	using ekizu::json_util::serialize;

	nlohmann::json j;

	serialize(j, "method", net::method_name(req.inner.method));
	serialize(j, "path", req.inner.path);
	serialize(j, "body", req.inner.body);
	serialize(j, "headers", req.inner.headers);
	serialize(j, "bot", req.bot);

	return j;
}
} // namespace

namespace ekizu
{
DiscordRequestManager::DiscordRequestManager(
	std::optional<std::function<void(LogLevel, std::string_view)> > log_fn)
	: m_log_fn{ std::move(log_fn) }

{
}

Result<net::HttpResponse>
DiscordRequestManager::send_request(const DiscordApiRequest &req)
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

		auto conn = net::HttpConnection::connect(DISCORD_BASE_URL);

		if (!conn) {
			return tl::make_unexpected(conn.error());
		}

		if (!m_http_connection) {
			m_http_connection = std::move(*conn);
		}

		log(LogLevel::Debug,
		    fmt::format("Sending request: {}", to_json(req).dump()));

		auto res = m_http_connection->request(req.inner);

		if (!res) {
			log(LogLevel::Error,
			    fmt::format("Failed to send request to {}: {}",
					req.inner.path, res.error().message()));

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

			log(LogLevel::Warn,
			    fmt::format("Rate limited, retrying request to {}",
					req.inner.path));

			continue;
		}

		log(LogLevel::Debug,
		    fmt::format("Failed to send request to {}: {}",
				req.inner.path,
				static_cast<uint16_t>(res->status_code)));

		break;
	}

	return tl::make_unexpected(
		std::make_error_code(std::errc::protocol_error));
}

void DiscordRequestManager::log(LogLevel level, std::string_view message)
{
	if (m_log_fn) {
		(*m_log_fn)(level, message);
	}
}
} // namespace ekizu
