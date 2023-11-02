#include <ekizu/gateway/thread_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
void to_json(nlohmann::json &j, const ThreadUpdate &t)
{
	j = t.thread;
}

void from_json(const nlohmann::json &j, ThreadUpdate &t)
{
	json_util::detail::deserialize_impl(j, t.thread);
}
} // namespace ekizu
