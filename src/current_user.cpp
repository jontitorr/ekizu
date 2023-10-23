#include <ekizu/current_user.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const CurrentUser &u)
{
	serialize(j, "id", u.id);
	serialize(j, "username", u.username);
	serialize(j, "discriminator", u.discriminator);
	serialize(j, "avatar", u.avatar);
	serialize(j, "bot", u.bot);
	serialize(j, "mfa_enabled", u.mfa_enabled);
	serialize(j, "banner", u.banner);
	serialize(j, "accent_color", u.accent_color);
	serialize(j, "locale", u.locale);
	serialize(j, "verified", u.verified);
	serialize(j, "flags", u.flags);
	serialize(j, "premium_type", u.premium_type);
	serialize(j, "public_flags", u.public_flags);
}

void from_json(const nlohmann::json &j, CurrentUser &u)
{
	deserialize(j, "id", u.id);
	deserialize(j, "username", u.username);
	deserialize(j, "discriminator", u.discriminator);
	deserialize(j, "avatar", u.avatar);
	deserialize(j, "bot", u.bot);
	deserialize(j, "mfa_enabled", u.mfa_enabled);
	deserialize(j, "banner", u.banner);
	deserialize(j, "accent_color", u.accent_color);
	deserialize(j, "locale", u.locale);
	deserialize(j, "verified", u.verified);
	deserialize(j, "flags", u.flags);
	deserialize(j, "premium_type", u.premium_type);
	deserialize(j, "public_flags", u.public_flags);
}

} // namespace ekizu
