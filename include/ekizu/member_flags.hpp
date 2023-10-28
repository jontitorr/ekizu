#ifndef EKIZU_MEMBERFLAGS_HPP
#define EKIZU_MEMBERFLAGS_HPP

#include <cstdint>
#include <type_traits>

namespace ekizu
{
enum class MemberFlags : uint8_t {
	/// Member has left and rejoined the guild.
	DidRejoin = 1 << 0,
	/// Member has completed onboarding.
	CompletedOnboarding = 1 << 1,
	/// Member is exempt from guild verification requirements.
	BypassesVerification = 1 << 2,
	/// Member has started onboarding.
	StartedOnboarding = 1 << 3,
};

constexpr MemberFlags operator|(MemberFlags lhs, MemberFlags rhs)
{
	return static_cast<MemberFlags>(
		static_cast<std::underlying_type_t<MemberFlags> >(lhs) |
		static_cast<std::underlying_type_t<MemberFlags> >(rhs));
}

constexpr MemberFlags operator&(MemberFlags lhs, MemberFlags rhs)
{
	return static_cast<MemberFlags>(
		static_cast<std::underlying_type_t<MemberFlags> >(lhs) &
		static_cast<std::underlying_type_t<MemberFlags> >(rhs));
}
} // namespace ekizu

#endif // EKIZU_MEMBERFLAGS_HPP
