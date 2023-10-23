#ifndef EKIZU_APPLICATION_FLAGS_HPP
#define EKIZU_APPLICATION_FLAGS_HPP

#include <cstdint>
#include <type_traits>

namespace ekizu
{
enum class ApplicationFlags : uint64_t {
	/// Indicates if an app uses the Auto Moderation API.
	ApplicationAutoModerationRuleCreateBadge = 1 << 6,
	/// Intent required for bots in 100 or more servers to receive
	/// presence_update events.
	GatewayPresence = 1 << 12,
	/// Intent required for bots in under 100 servers to receive presence_update
	/// events, found on the Bot page in your app's settings.
	GatewayPresenceLimited = 1 << 13,
	/// Intent required for bots in 100 or more servers to receive
	/// member-related events like guild_member_add. See the list of
	/// member-related events under GUILD_MEMBERS.
	GatewayGuildMembers = 1 << 14,
	/// Intent required for bots in under 100 servers to receive member-related
	/// events like guild_member_add, found on the Bot page in your app's
	/// settings. See the list of member-related events under GUILD_MEMBERS.
	GatewayGuildMembersLimited = 1 << 15,
	/// Indicates unusual growth of an app that prevents verification.
	VerificationPendingGuildLimit = 1 << 16,
	/// Indicates if an app is embedded within the Discord client (currently
	/// unavailable publicly).
	Embedded = 1 << 17,
	/// Intent required for bots in 100 or more servers to receive message
	/// content.
	GatewayMessageContent = 1 << 18,
	/// Intent required for bots in under 100 servers to receive message
	/// content, found on the Bot page in your app's settings.
	GatewayMessageContentLimited = 1 << 19,
	/// Indicates if an app has registered global application commands.
	ApplicationCommandBadge = 1 << 23
};

constexpr ApplicationFlags operator|(ApplicationFlags lhs, ApplicationFlags rhs)
{
	return static_cast<ApplicationFlags>(
		static_cast<std::underlying_type_t<ApplicationFlags> >(lhs) |
		static_cast<std::underlying_type_t<ApplicationFlags> >(rhs));
}

constexpr ApplicationFlags operator&(ApplicationFlags lhs, ApplicationFlags rhs)
{
	return static_cast<ApplicationFlags>(
		static_cast<std::underlying_type_t<ApplicationFlags> >(lhs) &
		static_cast<std::underlying_type_t<ApplicationFlags> >(rhs));
}
} // namespace ekizu

#endif // EKIZU_APPLICATION_FLAGS_HPP
