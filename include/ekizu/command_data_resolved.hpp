#ifndef EKIZU_COMMAND_DATA_RESOLVED_HPP
#define EKIZU_COMMAND_DATA_RESOLVED_HPP

#include <ekizu/member_flags.hpp>
#include <ekizu/message.hpp>

namespace ekizu {
struct InteractionChannel {
	/// ID of the channel.
	Snowflake id;
	/// Type of the channel.
	ChannelType type;
	/// Name of the channel.
	std::string name;
	/// ID of the channel the thread was created in.
	std::optional<Snowflake> parent_id;
	/// Computed permissions, including overwrites, for the invoking user in the
	/// channel.
	Permissions permissions;
	/// Metadata about a thread.
	std::optional<ThreadMetadata> thread_metadata;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const InteractionChannel &c);
EKIZU_EXPORT void from_json(const nlohmann::json &j, InteractionChannel &c);

struct InteractionMember {
	/// Member's guild avatar.
	std::optional<std::string> avatar;
	/// If the member is timed out, when the timeout will expire.
	std::optional<std::string> communication_disabled_until;
	/// Flags for the member.
	MemberFlags flags;
	/// Member guild join date.
	std::string joined_at;
	/// Member nickname.
	std::optional<std::string> nick;
	/// Whether the user has yet to pass the guild's Membership Screening
	/// requirements.
	bool pending;
	/// Total permissions of the member in this channel, including overwrites.
	Permissions permissions;
	std::optional<std::string> premium_since;
	/// Member roles.
	std::vector<Snowflake> roles;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const InteractionMember &m);
EKIZU_EXPORT void from_json(const nlohmann::json &j, InteractionMember &m);

struct CommandInteractionDataResolved {
	/// Map of resolved attachments.
	std::unordered_map<Snowflake, Attachment> attachments;
	/// Map of resolved channels.
	std::unordered_map<Snowflake, InteractionChannel> channels;
	/// Map of resolved members.
	///
	/// Resolved members' ID will map to a resolved user as well.
	std::unordered_map<Snowflake, InteractionMember> members;
	/// Map of resolved messages.
	std::unordered_map<Snowflake, Message> messages;
	/// Map of resolved roles.
	std::unordered_map<Snowflake, Role> roles;
	/// Map of resolved users.
	std::unordered_map<Snowflake, User> users;
};

EKIZU_EXPORT void to_json(nlohmann::json &j,
						  const CommandInteractionDataResolved &r);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
							CommandInteractionDataResolved &r);
}  // namespace ekizu

#endif	// EKIZU_COMMAND_DATA_RESOLVED_HPP
