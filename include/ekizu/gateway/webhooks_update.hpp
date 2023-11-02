#ifndef EKIZU_GATEWAY_WEBHOOKS_UPDATE_HPP
#define EKIZU_GATEWAY_WEBHOOKS_UPDATE_HPP

#include <ekizu/snowflake.hpp>

namespace ekizu
{
struct WebhooksUpdate {
	/// ID of the guild
	Snowflake guild_id;
	/// ID of the channel
	Snowflake channel_id;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const WebhooksUpdate &w);
EKIZU_EXPORT void from_json(const nlohmann::json &j, WebhooksUpdate &w);
} // namespace ekizu

#endif // EKIZU_GATEWAY_WEBHOOKS_UPDATE_HPP
