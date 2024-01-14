#include <ekizu/json_util.hpp>
#include <ekizu/request/modify_guild_channel_positions.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const ModifyGuildChannelPosition &f) {
	serialize(j, "id", f.id);
	serialize(j, "position", f.position);
	serialize(j, "lock_permissions", f.lock_permissions);
	serialize(j, "parent_id", f.parent_id);
}

void from_json(const nlohmann::json &j, ModifyGuildChannelPosition &f) {
	deserialize(j, "id", f.id);
	deserialize(j, "position", f.position);
	deserialize(j, "lock_permissions", f.lock_permissions);
	deserialize(j, "parent_id", f.parent_id);
}

ModifyGuildChannelPositions::ModifyGuildChannelPositions(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id, std::vector<ModifyGuildChannelPosition> channels)
	: m_guild_id{guild_id},
	  m_channels{std::move(channels)},
	  m_make_request{make_request} {}

ModifyGuildChannelPositions::operator net::HttpRequest() const {
	net::HttpRequest req{
		net::HttpMethod::patch, fmt::format("/guilds/{}/channels", m_guild_id),
		11, static_cast<nlohmann::json>(m_channels).dump()};

	req.set(net::http::field::content_type, "application/json");
	req.prepare_payload();

	return req;
}

Result<> ModifyGuildChannelPositions::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	EKIZU_TRY(auto res, m_make_request(*this, yield));

	if (res.result() == net::HttpStatus::no_content) {
		return outcome::success();
	}

	return boost::system::errc::operation_not_permitted;
}
}  // namespace ekizu
