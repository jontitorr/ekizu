#include <ekizu/gateway/guild_scheduled_event_delete.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
void to_json(nlohmann::json &j, const GuildScheduledEventDelete &c) {
	j = c.event;
}

void from_json(const nlohmann::json &j, GuildScheduledEventDelete &c) {
	json_util::detail::deserialize_impl(j, c.event);
}
}  // namespace ekizu
