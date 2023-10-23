#include <ekizu/discord_api.hpp>

namespace
{
constexpr const char *DISCORD_BOT_GATEWAY_ENDPOINT = "/api/v10/gateway/bot";
constexpr const char *DISCORD_USER_GATEWAY_ENDPOINT = "/api/v10/gateway";
} // namespace

namespace ekizu
{
DiscordApi::DiscordApi(
	std::string_view token, bool is_bot,
	std::optional<std::function<void(LogLevel, std::string_view)> > log_fn)
	: m_token{ token }
	, m_is_bot{ is_bot }
	, m_request_manager{ std::move(log_fn) }
{
}

Result<net::HttpResponse> DiscordApi::get_gateway()
{
	auto req = build_request();

	if (!req) {
		return tl::make_unexpected(req.error());
	}

	req->inner.method = net::HttpMethod::Get;
	req->inner.path = m_is_bot ? DISCORD_BOT_GATEWAY_ENDPOINT :
				     DISCORD_USER_GATEWAY_ENDPOINT;

	return DiscordRequestManagerAttorney::send_request(m_request_manager,
							   *req);
}

Result<DiscordApiRequest> DiscordApi::build_request()
{
	if (m_token.empty()) {
		return tl::make_unexpected(std::make_error_code(
			std::errc::operation_not_permitted));
	}

	return DiscordApiRequest{
		{ net::HttpRequest{
			{},
			{},
			{},
			{
				{ "Authorization",
				  m_is_bot ? "Bot " + m_token : m_token },
			} } },
		m_is_bot,
	};
}
} // namespace ekizu
