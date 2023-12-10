#include <ekizu/gateway/guild_scheduled_event_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
void to_json(nlohmann::json &j, const GuildScheduledEventUpdate &c) {
	j = c.event;
}

void from_json(const nlohmann::json &j, GuildScheduledEventUpdate &c) {
	json_util::detail::deserialize_impl(j, c.event);
}
}  // namespace ekizu
