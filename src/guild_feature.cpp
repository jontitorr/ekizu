#include <ekizu/guild_feature.hpp>
#include <nlohmann/json.hpp>

namespace
{
using namespace ekizu;

std::string to_str(GuildFeature feature)
{
	switch (feature) {
	case GuildFeature::AnimatedBanner:
		return "ANIMATED_BANNER";
	case GuildFeature::AnimatedIcon:
		return "ANIMATED_ICON";
	case GuildFeature::AutoModeration:
		return "AUTO_MODERATION";
	case GuildFeature::Banner:
		return "BANNER";
	case GuildFeature::Community:
		return "COMMUNITY";
	case GuildFeature::CreatorMonetizableProvisional:
		return "CREATOR_MONETIZABLE_PROVISIONAL";
	case GuildFeature::CreatorStorePage:
		return "CREATOR_STORE_PAGE";
	case GuildFeature::DeveloperSupportServer:
		return "DEVELOPER_SUPPORT_SERVER";
	case GuildFeature::Discoverable:
		return "DISCOVERABLE";
	case GuildFeature::Featurable:
		return "FEATURABLE";
	case GuildFeature::InvitesDisabled:
		return "INVITES_DISABLED";
	case GuildFeature::InviteSplash:
		return "INVITE_SPLASH";
	case GuildFeature::MemberVerificationGateEnabled:
		return "MEMBER_VERIFICATION_GATE_ENABLED";
	case GuildFeature::MonetizationEnabled:
		return "MONETIZATION_ENABLED";
	case GuildFeature::MoreStickers:
		return "MORE_STICKERS";
	case GuildFeature::News:
		return "NEWS";
	case GuildFeature::Partnered:
		return "PARTNERED";
	case GuildFeature::PreviewEnabled:
		return "PREVIEW_ENABLED";
	case GuildFeature::PrivateThreads:
		return "PRIVATE_THREADS";
	case GuildFeature::RoleIcons:
		return "ROLE_ICONS";
	case GuildFeature::RoleSubscriptionsAvailableForPurchase:
		return "ROLE_SUBSCRIPTIONS_AVAILABLE_FOR_PURCHASE";
	case GuildFeature::RoleSubscriptionsEnabled:
		return "ROLE_SUBSCRIPTIONS_ENABLED";
	case GuildFeature::TicketedEventsEnabled:
		return "TICKETED_EVENTS_ENABLED";
	case GuildFeature::VanityUrl:
		return "VANITY_URL";
	case GuildFeature::Verified:
		return "VERIFIED";
	case GuildFeature::VipRegions:
		return "VIP_REGIONS";
	case GuildFeature::WelcomeScreenEnabled:
		return "WELCOME_SCREEN_ENABLED";
	default:
		return "UNKNOWN";
	}
}

GuildFeature from_str(const std::string &str)
{
	if (str == "ANIMATED_BANNER") {
		return GuildFeature::AnimatedBanner;
	}
	if (str == "ANIMATED_ICON") {
		return GuildFeature::AnimatedIcon;
	}
	if (str == "AUTO_MODERATION") {
		return GuildFeature::AutoModeration;
	}
	if (str == "BANNER") {
		return GuildFeature::Banner;
	}
	if (str == "COMMUNITY") {
		return GuildFeature::Community;
	}
	if (str == "CREATOR_MONETIZABLE_PROVISIONAL") {
		return GuildFeature::CreatorMonetizableProvisional;
	}
	if (str == "CREATOR_STORE_PAGE") {
		return GuildFeature::CreatorStorePage;
	}
	if (str == "DEVELOPER_SUPPORT_SERVER") {
		return GuildFeature::DeveloperSupportServer;
	}
	if (str == "DISCOVERABLE") {
		return GuildFeature::Discoverable;
	}
	if (str == "FEATURABLE") {
		return GuildFeature::Featurable;
	}
	if (str == "INVITES_DISABLED") {
		return GuildFeature::InvitesDisabled;
	}
	if (str == "INVITE_SPLASH") {
		return GuildFeature::InviteSplash;
	}
	if (str == "MEMBER_VERIFICATION_GATE_ENABLED") {
		return GuildFeature::MemberVerificationGateEnabled;
	}
	if (str == "MONETIZATION_ENABLED") {
		return GuildFeature::MonetizationEnabled;
	}
	if (str == "MORE_STICKERS") {
		return GuildFeature::MoreStickers;
	}
	if (str == "NEWS") {
		return GuildFeature::News;
	}
	if (str == "PARTNERED") {
		return GuildFeature::Partnered;
	}
	if (str == "PREVIEW_ENABLED") {
		return GuildFeature::PreviewEnabled;
	}
	if (str == "PRIVATE_THREADS") {
		return GuildFeature::PrivateThreads;
	}
	if (str == "ROLE_ICONS") {
		return GuildFeature::RoleIcons;
	}
	if (str == "ROLE_SUBSCRIPTIONS_AVAILABLE_FOR_PURCHASE") {
		return GuildFeature::RoleSubscriptionsAvailableForPurchase;
	}
	if (str == "ROLE_SUBSCRIPTIONS_ENABLED") {
		return GuildFeature::RoleSubscriptionsEnabled;
	}
	if (str == "TICKETED_EVENTS_ENABLED") {
		return GuildFeature::TicketedEventsEnabled;
	}
	if (str == "VANITY_URL") {
		return GuildFeature::VanityUrl;
	}
	if (str == "VERIFIED") {
		return GuildFeature::Verified;
	}
	if (str == "VIP_REGIONS") {
		return GuildFeature::VipRegions;
	}
	if (str == "WELCOME_SCREEN_ENABLED") {
		return GuildFeature::WelcomeScreenEnabled;
	}
	return GuildFeature::Unknown;
}
} // namespace

namespace ekizu
{
void to_json(nlohmann::json &j, const GuildFeature &f)
{
	j = to_str(f);
}

void from_json(const nlohmann::json &j, GuildFeature &f)
{
	if (j.is_string()) {
		f = from_str(j.get<std::string>());
	}
}
} // namespace ekizu