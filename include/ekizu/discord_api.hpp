#ifndef EKIZU_DISCORD_API_HPP
#define EKIZU_DISCORD_API_HPP

#include <ekizu/discord_request_manager.hpp>
#include <nlohmann/json_fwd.hpp>

namespace ekizu
{
struct GetGateway {
	std::string url;
};

/**
 * @brief The DiscordApi class is the main class of the library. It is used
 * to interact with the Discord API.
 */
struct DiscordApi {
	explicit DiscordApi(
		std::string_view token, bool is_bot,
		std::optional<std::function<void(LogLevel, std::string_view)> >
			log_fn = {});

	/**
     * @brief Get the gateway URL.
     *
     * @return An HTTP response containing the gateway URL if successful.
     */
	[[nodiscard]] EKIZU_EXPORT Result<net::HttpResponse> get_gateway();

    private:
	[[nodiscard]] Result<DiscordApiRequest> build_request();

	std::string m_token;
	bool m_is_bot;
	DiscordRequestManager m_request_manager;
};
} // namespace ekizu

#endif // EKIZU_DISCORD_API_HPP
