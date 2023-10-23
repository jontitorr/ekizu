#include <ekizu/gateway/message_create.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
void to_json(nlohmann::json &j, const MessageCreate &m)
{
	j = m.message;
}

void from_json(const nlohmann::json &j, MessageCreate &m)
{
	json_util::detail::deserialize_impl(j, m.message);
}
} // namespace ekizu
