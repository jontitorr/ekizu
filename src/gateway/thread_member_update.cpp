#include <ekizu/gateway/thread_member_update.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
void to_json(nlohmann::json &j, const ThreadMemberUpdate &m)
{
	j = m.member;
}

void from_json(const nlohmann::json &j, ThreadMemberUpdate &m)
{
	json_util::detail::deserialize_impl(j, m.member);
}
} // namespace ekizu
