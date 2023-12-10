#include <ekizu/gateway/thread_create.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
void to_json(nlohmann::json &j, const ThreadCreate &t) { j = t.thread; }

void from_json(const nlohmann::json &j, ThreadCreate &t) {
	json_util::detail::deserialize_impl(j, t.thread);
}
}  // namespace ekizu
