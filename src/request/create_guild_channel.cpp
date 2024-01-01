#include <ekizu/json_util.hpp>
#include <ekizu/request/create_guild_channel.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const CreateGuildChannelFields &f) {
	serialize(j, "name", f.name);
	serialize(j, "type", f.type);
	serialize(j, "topic", f.topic);
	serialize(j, "bitrate", f.bitrate);
	serialize(j, "user_limit", f.user_limit);
	serialize(j, "rate_limit_per_user", f.rate_limit_per_user);
	serialize(j, "position", f.position);
	serialize(j, "permission_overwrites", f.permission_overwrites);
	serialize(j, "parent_id", f.parent_id);
	serialize(j, "nsfw", f.nsfw);
	serialize(j, "rtc_region", f.rtc_region);
	serialize(j, "video_quality_mode", f.video_quality_mode);
	serialize(
		j, "default_auto_archive_duration", f.default_auto_archive_duration);
	serialize(j, "default_reaction_emoji", f.default_reaction_emoji);
	serialize(j, "available_tags", f.available_tags);
	serialize(j, "default_sort_order", f.default_sort_order);
	serialize(j, "default_forum_layout", f.default_forum_layout);
	serialize(j, "default_thread_rate_limit_per_user",
			  f.default_thread_rate_limit_per_user);
}

void from_json(const nlohmann::json &j, CreateGuildChannelFields &f) {
	deserialize(j, "name", f.name);
	deserialize(j, "type", f.type);
	deserialize(j, "topic", f.topic);
	deserialize(j, "bitrate", f.bitrate);
	deserialize(j, "user_limit", f.user_limit);
	deserialize(j, "rate_limit_per_user", f.rate_limit_per_user);
	deserialize(j, "position", f.position);
	deserialize(j, "permission_overwrites", f.permission_overwrites);
	deserialize(j, "parent_id", f.parent_id);
	deserialize(j, "nsfw", f.nsfw);
	deserialize(j, "rtc_region", f.rtc_region);
	deserialize(j, "video_quality_mode", f.video_quality_mode);
	deserialize(
		j, "default_auto_archive_duration", f.default_auto_archive_duration);
	deserialize(j, "default_reaction_emoji", f.default_reaction_emoji);
	deserialize(j, "available_tags", f.available_tags);
	deserialize(j, "default_sort_order", f.default_sort_order);
	deserialize(j, "default_forum_layout", f.default_forum_layout);
	deserialize(j, "default_thread_rate_limit_per_user",
				f.default_thread_rate_limit_per_user);
}

CreateGuildChannel::CreateGuildChannel(
	const std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)> &make_request,
	Snowflake guild_id, std::string_view name)
	: m_guild_id{guild_id},
	  m_fields{std::string{name}},
	  m_make_request{make_request} {}

CreateGuildChannel::operator net::HttpRequest() const {
	fmt::println("Fields: {}", static_cast<nlohmann::json>(m_fields).dump());

	net::HttpRequest req{
		net::HttpMethod::post, fmt::format("/guilds/{}/channels", m_guild_id),
		11, static_cast<nlohmann::json>(m_fields).dump()};

	req.set(net::http::field::content_type, "application/json");
	req.prepare_payload();

	return req;
}

Result<Channel> CreateGuildChannel::send(
	const asio::yield_context &yield) const {
	if (!m_make_request) {
		return boost::system::errc::operation_not_permitted;
	}

	BOOST_OUTCOME_TRY(auto res, m_make_request(*this, yield));

	fmt::println("Body: {}", res.body());

	return json_util::deserialize<Channel>(res.body());
}
}  // namespace ekizu
