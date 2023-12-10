#ifndef EKIZU_INTENTS_HPP
#define EKIZU_INTENTS_HPP

#include <cstdint>
#include <type_traits>

namespace ekizu {
enum class Intents : uint32_t {
	/// Intent for receipt of guilds.
	Guilds = (1 << 0),
	/// Intent for receipt of guild members.
	GuildMembers = (1 << 1),
	/// Intent for receipt of guild bans.
	GuildBans = (1 << 2),
	/// Intent for receipt of guild emojis.
	GuildEmojis = (1 << 3),
	/// Intent for receipt of guild integrations.
	GuildIntegrations = (1 << 4),
	/// Intent for receipt of guild webhooks.
	GuildWebhooks = (1 << 5),
	/// Intent for receipt of guild invites.
	GuildInvites = (1 << 6),
	/// Intent for receipt of guild voice states.
	GuildVoiceStates = (1 << 7),
	/// Intent for receipt of guild presences.
	GuildPresences = (1 << 8),
	/// Intent for receipt of guild messages.
	GuildMessages = (1 << 9),
	/// Intent for receipt of guild message reactions.
	GuildMessageReactions = (1 << 10),
	/// Intent for receipt of guild message typing.
	GuildMessageTyping = (1 << 11),
	/// Intent for receipt of direct messages.
	DirectMessages = (1 << 12),
	/// Intent for receipt of direct message reactions.
	DirectMessageReactions = (1 << 13),
	/// Intent for receipt of direct message typing.
	DirectMessageTyping = (1 << 14),
	/// Intent for receipt of message content.
	MessageContent = (1 << 15),
	/// Intent for receipt of guild scheduled events.
	GuildScheduledEvents = (1 << 16),
	/// Default ekizu intents (all non-privileged intents).
	DefaultIntents =
		Guilds | GuildBans | GuildEmojis | GuildIntegrations | GuildWebhooks |
		GuildInvites | GuildVoiceStates | GuildMessages |
		GuildMessageReactions | GuildMessageTyping | DirectMessages |
		DirectMessageReactions | DirectMessageTyping | GuildScheduledEvents,
	/// Privileged intents requiring ID.
	PrivilegedIntents = GuildMembers | GuildPresences | MessageContent,
	/// Every single intent.
	AllIntents = DefaultIntents | PrivilegedIntents
};

constexpr Intents operator|(Intents lhs, Intents rhs) {
	return static_cast<Intents>(
		static_cast<std::underlying_type_t<Intents> >(lhs) |
		static_cast<std::underlying_type_t<Intents> >(rhs));
}

constexpr Intents operator&(Intents lhs, Intents rhs) {
	return static_cast<Intents>(
		static_cast<std::underlying_type_t<Intents> >(lhs) &
		static_cast<std::underlying_type_t<Intents> >(rhs));
}
}  // namespace ekizu

#endif	// EKIZU_INTENTS_HPP
