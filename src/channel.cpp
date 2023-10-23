#include <ekizu/channel.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const DefaultReaction &r)
{
	serialize(j, "emoji_id", r.emoji_id);
	serialize(j, "emoji_name", r.emoji_name);
}

void from_json(const nlohmann::json &j, DefaultReaction &r)
{
	deserialize(j, "emoji_id", r.emoji_id);
	deserialize(j, "emoji_name", r.emoji_name);
}

void to_json(nlohmann::json &j, const ForumTag &t)
{
	serialize(j, "id", t.id);
	serialize(j, "name", t.name);
	serialize(j, "moderated", t.moderated);
	serialize(j, "emoji_id", t.emoji_id);
	serialize(j, "emoji_name", t.emoji_name);
}

void from_json(const nlohmann::json &j, ForumTag &t)
{
	deserialize(j, "id", t.id);
	deserialize(j, "name", t.name);
	deserialize(j, "moderated", t.moderated);
	deserialize(j, "emoji_id", t.emoji_id);
	deserialize(j, "emoji_name", t.emoji_name);
}

void to_json(nlohmann::json &j, const PermissionOverwrite &p)
{
	serialize(j, "id", p.id);
	serialize(j, "type", p.type);
	serialize(j, "allow", p.allow);
	serialize(j, "deny", p.deny);
}

void from_json(const nlohmann::json &j, PermissionOverwrite &p)
{
	deserialize(j, "id", p.id);
	deserialize(j, "type", p.type);
	deserialize(j, "allow", p.allow);
	deserialize(j, "deny", p.deny);
}

void to_json(nlohmann::json &j, const ThreadMetadata &t)
{
	serialize(j, "archived", t.archived);
	serialize(j, "auto_archive_duration", t.auto_archive_duration);
	serialize(j, "archive_timestamp", t.archive_timestamp);
	serialize(j, "locked", t.locked);
	serialize(j, "invitable", t.invitable);
	serialize(j, "create_timestamp", t.create_timestamp);
}

void from_json(const nlohmann::json &j, ThreadMetadata &t)
{
	deserialize(j, "archived", t.archived);
	deserialize(j, "auto_archive_duration", t.auto_archive_duration);
	deserialize(j, "archive_timestamp", t.archive_timestamp);
	deserialize(j, "locked", t.locked);
	deserialize(j, "invitable", t.invitable);
	deserialize(j, "create_timestamp", t.create_timestamp);
}

void to_json(nlohmann::json &j, const ThreadMember &m)
{
	serialize(j, "id", m.id);
	serialize(j, "user_id", m.user_id);
	serialize(j, "join_timestamp", m.join_timestamp);
	serialize(j, "flags", m.flags);
	serialize(j, "member", m.member);
}

void from_json(const nlohmann::json &j, ThreadMember &m)
{
	deserialize(j, "id", m.id);
	deserialize(j, "user_id", m.user_id);
	deserialize(j, "join_timestamp", m.join_timestamp);
	deserialize(j, "flags", m.flags);
	deserialize(j, "member", m.member);
}

void to_json(nlohmann::json &j, const Channel &c)
{
	serialize(j, "id", c.id);
	serialize(j, "type", c.type);
	serialize(j, "guild_id", c.guild_id);
	serialize(j, "position", c.position);
	serialize(j, "permission_overwrites", c.permission_overwrites);
	serialize(j, "name", c.name);
	serialize(j, "topic", c.topic);
	serialize(j, "nsfw", c.nsfw);
	serialize(j, "last_message_id", c.last_message_id);
	serialize(j, "bitrate", c.bitrate);
	serialize(j, "user_limit", c.user_limit);
	serialize(j, "rate_limit_per_user", c.rate_limit_per_user);
	serialize(j, "recipients", c.recipients);
	serialize(j, "icon", c.icon);
	serialize(j, "owner_id", c.owner_id);
	serialize(j, "application_id", c.application_id);
	serialize(j, "managed", c.managed);
	serialize(j, "parent_id", c.parent_id);
	serialize(j, "last_pin_timestamp", c.last_pin_timestamp);
	serialize(j, "rtc_region", c.rtc_region);
	serialize(j, "video_quality_mode", c.video_quality_mode);
	serialize(j, "message_count", c.message_count);
	serialize(j, "member_count", c.member_count);
	serialize(j, "thread_metadata", c.thread_metadata);
	serialize(j, "member", c.member);
	serialize(j, "default_auto_archive_duration",
		  c.default_auto_archive_duration);
	serialize(j, "permissions", c.permissions);
	serialize(j, "flags", c.flags);
	serialize(j, "total_messages_sent", c.total_messages_sent);
	serialize(j, "available_tags", c.available_tags);
	serialize(j, "applied_tags", c.applied_tags);
	serialize(j, "default_reaction_emoji", c.default_reaction_emoji);
	serialize(j, "default_thread_rate_limit_per_user",
		  c.default_thread_rate_limit_per_user);
	serialize(j, "default_sort_order", c.default_sort_order);
	serialize(j, "default_forum_layout", c.default_forum_layout);
}

void from_json(const nlohmann::json &j, Channel &c)
{
	deserialize(j, "id", c.id);
	deserialize(j, "type", c.type);
	deserialize(j, "guild_id", c.guild_id);
	deserialize(j, "position", c.position);
	deserialize(j, "permission_overwrites", c.permission_overwrites);
	deserialize(j, "name", c.name);
	deserialize(j, "topic", c.topic);
	deserialize(j, "nsfw", c.nsfw);
	deserialize(j, "last_message_id", c.last_message_id);
	deserialize(j, "bitrate", c.bitrate);
	deserialize(j, "user_limit", c.user_limit);
	deserialize(j, "rate_limit_per_user", c.rate_limit_per_user);
	deserialize(j, "recipients", c.recipients);
	deserialize(j, "icon", c.icon);
	deserialize(j, "owner_id", c.owner_id);
	deserialize(j, "application_id", c.application_id);
	deserialize(j, "managed", c.managed);
	deserialize(j, "parent_id", c.parent_id);
	deserialize(j, "last_pin_timestamp", c.last_pin_timestamp);
	deserialize(j, "rtc_region", c.rtc_region);
	deserialize(j, "video_quality_mode", c.video_quality_mode);
	deserialize(j, "message_count", c.message_count);
	deserialize(j, "member_count", c.member_count);
	deserialize(j, "thread_metadata", c.thread_metadata);
	deserialize(j, "member", c.member);
	deserialize(j, "default_auto_archive_duration",
		    c.default_auto_archive_duration);
	deserialize(j, "permissions", c.permissions);
	deserialize(j, "flags", c.flags);
	deserialize(j, "total_messages_sent", c.total_messages_sent);
	deserialize(j, "available_tags", c.available_tags);
	deserialize(j, "applied_tags", c.applied_tags);
	deserialize(j, "default_reaction_emoji", c.default_reaction_emoji);
	deserialize(j, "default_thread_rate_limit_per_user",
		    c.default_thread_rate_limit_per_user);
	deserialize(j, "default_sort_order", c.default_sort_order);
	deserialize(j, "default_forum_layout", c.default_forum_layout);
}

} // namespace ekizu