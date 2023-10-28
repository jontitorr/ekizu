#include <ekizu/json_util.hpp>
#include <ekizu/mention.hpp>

namespace ekizu
{
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const Mention &m)
{
	serialize(j, "id", m.id);
	serialize(j, "username", m.username);
	serialize(j, "discriminator", m.discriminator);
	serialize(j, "global_name", m.global_name);
	serialize(j, "avatar", m.avatar);
	serialize(j, "bot", m.bot);
	serialize(j, "system", m.system);
	serialize(j, "mfa_enabled", m.mfa_enabled);
	serialize(j, "banner", m.banner);
	serialize(j, "accent_color", m.accent_color);
	serialize(j, "locale", m.locale);
	serialize(j, "verified", m.verified);
	serialize(j, "email", m.email);
	serialize(j, "flags", m.flags);
	serialize(j, "premium_type", m.premium_type);
	serialize(j, "public_flags", m.public_flags);
	serialize(j, "avatar_decoration", m.avatar_decoration);
}

void from_json(const nlohmann::json &j, Mention &m)
{
	deserialize(j, "id", m.id);
	deserialize(j, "username", m.username);
	deserialize(j, "discriminator", m.discriminator);
	deserialize(j, "global_name", m.global_name);
	deserialize(j, "avatar", m.avatar);
	deserialize(j, "bot", m.bot);
	deserialize(j, "system", m.system);
	deserialize(j, "mfa_enabled", m.mfa_enabled);
	deserialize(j, "banner", m.banner);
	deserialize(j, "accent_color", m.accent_color);
	deserialize(j, "locale", m.locale);
	deserialize(j, "verified", m.verified);
	deserialize(j, "email", m.email);
	deserialize(j, "flags", m.flags);
	deserialize(j, "premium_type", m.premium_type);
	deserialize(j, "public_flags", m.public_flags);
	deserialize(j, "avatar_decoration", m.avatar_decoration);
}
} // namespace ekizu
