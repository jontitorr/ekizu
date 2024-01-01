#ifndef EKIZU_REQUEST_CREATE_GUILD_HPP
#define EKIZU_REQUEST_CREATE_GUILD_HPP

#include <ekizu/channel.hpp>
#include <ekizu/guild.hpp>
#include <ekizu/http.hpp>

namespace ekizu {
struct RoleFields {
	Snowflake id;
	std::string name;
	std::optional<uint32_t> color{};
	std::optional<bool> hoist{};
	std::optional<uint64_t> position{};
	std::optional<Permissions> permissions{};
	std::optional<bool> mentionable{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const RoleFields &f);

struct CategoryChannelFields {
	Snowflake id;
	ChannelType type;
	std::string name;
	std::optional<std::vector<PermissionOverwrite>> permission_overwrites{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const CategoryChannelFields &f);

struct TextChannelFields {
	Snowflake id;
	ChannelType type;
	std::string name;
	std::optional<std::string> topic{};
	std::optional<bool> nsfw{};
	std::optional<uint16_t> rate_limit_per_user{};
	std::optional<Snowflake> parent_id{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const TextChannelFields &f);

struct VoiceChannelFields {
	Snowflake id;
	ChannelType type;
	std::optional<std::vector<PermissionOverwrite>> permission_overwrites{};
	std::string name;
	std::optional<uint32_t> bitrate{};
	std::optional<uint16_t> user_limit{};
	std::optional<Snowflake> parent_id{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const VoiceChannelFields &f);

using GuildChannelFields =
	std::variant<CategoryChannelFields, TextChannelFields, VoiceChannelFields>;

EKIZU_EXPORT void to_json(nlohmann::json &j, const GuildChannelFields &f);

struct CreateGuildFields {
	std::string name;
	std::optional<std::string> region{};
	std::optional<std::string> icon{};
	std::optional<VerificationLevel> verification_level{};
	std::optional<DefaultMessageNotificationLevel>
		default_message_notifications{};
	std::optional<ExplicitContentFilter> explicit_content_filter{};
	std::optional<std::vector<RoleFields>> roles{};
	std::optional<std::vector<GuildChannelFields>> channels{};
	std::optional<Snowflake> afk_channel_id{};
	std::optional<uint16_t> afk_timeout{};
	std::optional<Snowflake> system_channel_id{};
	std::optional<SystemChannelFlags> system_channel_flags{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const CreateGuildFields &f);

struct CreateGuild {
	explicit CreateGuild(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request,
		std::string_view name);

	operator net::HttpRequest() const;

	CreateGuild &afk_channel_id(Snowflake afk_channel_id) {
		m_fields.afk_channel_id = afk_channel_id;
		return *this;
	}

	CreateGuild &afk_timeout(uint16_t afk_timeout) {
		m_fields.afk_timeout = afk_timeout;
		return *this;
	}

	CreateGuild &channels(std::vector<GuildChannelFields> channels) {
		m_fields.channels = std::move(channels);
		return *this;
	}

	CreateGuild &default_message_notifications(
		DefaultMessageNotificationLevel default_message_notifications) {
		m_fields.default_message_notifications = default_message_notifications;
		return *this;
	}

	CreateGuild &explicit_content_filter(
		ExplicitContentFilter explicit_content_filter) {
		m_fields.explicit_content_filter = explicit_content_filter;
		return *this;
	}

	CreateGuild &icon(std::string_view icon) {
		m_fields.icon = std::string{icon};
		return *this;
	}

	CreateGuild &system_channel_id(Snowflake system_channel_id) {
		m_fields.system_channel_id = system_channel_id;
		return *this;
	}

	CreateGuild &system_channel_flags(SystemChannelFlags system_channel_flags) {
		m_fields.system_channel_flags = system_channel_flags;
		return *this;
	}

	[[nodiscard]] EKIZU_EXPORT Result<Guild> send(
		const asio::yield_context &yield) const;

   private:
	CreateGuildFields m_fields;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_CREATE_GUILD_HPP
