#ifndef EKIZU_ACTIVITY_FLAGS_HPP
#define EKIZU_ACTIVITY_FLAGS_HPP

#include <cstdint>
#include <type_traits>

namespace ekizu
{
enum class ActivityFlags : uint64_t {
	Instance = 1 << 0,
	Join = 1 << 1,
	Spectate = 1 << 2,
	JoinRequest = 1 << 3,
	Sync = 1 << 4,
	Play = 1 << 5,
	PartyPrivacyFriends = 1 << 6,
	PartyPrivacyVoiceChannel = 1 << 7,
	Embedded = 1 << 8,
};

constexpr ActivityFlags operator|(ActivityFlags lhs, ActivityFlags rhs)
{
	return static_cast<ActivityFlags>(
		static_cast<std::underlying_type_t<ActivityFlags> >(lhs) |
		static_cast<std::underlying_type_t<ActivityFlags> >(rhs));
}

constexpr ActivityFlags operator&(ActivityFlags lhs, ActivityFlags rhs)
{
	return static_cast<ActivityFlags>(
		static_cast<std::underlying_type_t<ActivityFlags> >(lhs) &
		static_cast<std::underlying_type_t<ActivityFlags> >(rhs));
}
} // namespace ekizu

#endif // EKIZU_ACTIVITY_FLAGS_HPP
