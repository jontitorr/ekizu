#include <ekizu/json_util.hpp>
#include <ekizu/presence.hpp>

namespace {
ekizu::Status from_str(std::string_view str) {
	if (str == "online") { return ekizu::Status::Online; }
	if (str == "dnd") { return ekizu::Status::DoNotDisturb; }
	if (str == "idle") { return ekizu::Status::Idle; }
	if (str == "offline") { return ekizu::Status::Offline; }

	return ekizu::Status::Unknown;
}

std::string to_str(ekizu::Status status) {
	switch (status) {
		case ekizu::Status::Online: return "online";
		case ekizu::Status::DoNotDisturb: return "dnd";
		case ekizu::Status::Idle: return "idle";
		case ekizu::Status::Offline: return "offline";
		default: return "unknown";
	}
}
}  // namespace

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const ActivityAssets &a) {
	serialize(j, "large_image", a.large_image);
	serialize(j, "large_text", a.large_text);
	serialize(j, "small_image", a.small_image);
	serialize(j, "small_text", a.small_text);
}

void from_json(const nlohmann::json &j, ActivityAssets &a) {
	deserialize(j, "large_image", a.large_image);
	deserialize(j, "large_text", a.large_text);
	deserialize(j, "small_image", a.small_image);
	deserialize(j, "small_text", a.small_text);
}

void to_json(nlohmann::json &j, const ActivityButton &b) {
	serialize(j, "label", b.label);
	serialize(j, "url", b.url);
}

void from_json(const nlohmann::json &j, ActivityButton &b) {
	deserialize(j, "label", b.label);
	deserialize(j, "url", b.url);
}

void to_json(nlohmann::json &j, const ActivityEmoji &e) {
	serialize(j, "name", e.name);
	serialize(j, "id", e.id);
	serialize(j, "animated", e.animated);
}

void from_json(const nlohmann::json &j, ActivityEmoji &e) {
	deserialize(j, "name", e.name);
	deserialize(j, "id", e.id);
	deserialize(j, "animated", e.animated);
}

void to_json(nlohmann::json &j, const ActivityParty &p) {
	serialize(j, "id", p.id);
	serialize(j, "size", p.size);
}

void from_json(const nlohmann::json &j, ActivityParty &p) {
	deserialize(j, "id", p.id);
	deserialize(j, "size", p.size);
}

void to_json(nlohmann::json &j, const ActivitySecrets &s) {
	serialize(j, "join", s.join);
	serialize(j, "spectate", s.spectate);
	serialize(j, "match", s.match);
}

void from_json(const nlohmann::json &j, ActivitySecrets &s) {
	deserialize(j, "join", s.join);
	deserialize(j, "spectate", s.spectate);
	deserialize(j, "match", s.match);
}

void to_json(nlohmann::json &j, const ActivityTimestamps &t) {
	serialize(j, "start", t.start);
	serialize(j, "end", t.end);
}

void from_json(const nlohmann::json &j, ActivityTimestamps &t) {
	deserialize(j, "start", t.start);
	deserialize(j, "end", t.end);
}

void to_json(nlohmann::json &j, const Activity &a) {
	serialize(j, "name", a.name);
	serialize(j, "type", static_cast<int>(a.type));
	serialize(j, "url", a.url);
	serialize(j, "created_at", a.created_at);
	serialize(j, "timestamps", a.timestamps);
	serialize(j, "application_id", a.application_id);
	serialize(j, "details", a.details);
	serialize(j, "state", a.state);
	serialize(j, "emoji", a.emoji);
	serialize(j, "party", a.party);
	serialize(j, "assets", a.assets);
	serialize(j, "secrets", a.secrets);
	serialize(j, "instance", a.instance);
	serialize(j, "flags", a.flags);
	serialize(j, "buttons", a.buttons);
}

void from_json(const nlohmann::json &j, Activity &a) {
	deserialize(j, "name", a.name);
	deserialize(j, "type", a.type);
	deserialize(j, "url", a.url);
	deserialize(j, "created_at", a.created_at);
	deserialize(j, "timestamps", a.timestamps);
	deserialize(j, "application_id", a.application_id);
	deserialize(j, "details", a.details);
	deserialize(j, "state", a.state);
	deserialize(j, "emoji", a.emoji);
	deserialize(j, "party", a.party);
	deserialize(j, "assets", a.assets);
	deserialize(j, "secrets", a.secrets);
	deserialize(j, "instance", a.instance);
	deserialize(j, "flags", a.flags);
	deserialize(j, "buttons", a.buttons);
}

void to_json(nlohmann::json &j, const Status &s) { j = to_str(s); }

void from_json(const nlohmann::json &j, Status &s) {
	if (!j.is_string()) { s = Status::Unknown; }

	s = from_str(j.get<std::string>());
}

void to_json(nlohmann::json &j, const ClientStatus &c) {
	serialize(j, "desktop", c.desktop);
	serialize(j, "mobile", c.mobile);
	serialize(j, "web", c.web);
}

void from_json(const nlohmann::json &j, ClientStatus &c) {
	deserialize(j, "desktop", c.desktop);
	deserialize(j, "mobile", c.mobile);
	deserialize(j, "web", c.web);
}

void to_json(nlohmann::json &j, const Presence &p) {
	serialize(j, "activities", p.activities);
	serialize(j, "client_status", p.client_status);
	serialize(j, "guild_id", p.guild_id);
	serialize(j, "status", p.status);
}

void from_json(const nlohmann::json &j, Presence &p) {
	deserialize(j, "activities", p.activities);
	deserialize(j, "client_status", p.client_status);
	deserialize(j, "guild_id", p.guild_id);
	deserialize(j, "status", p.status);
}
}  // namespace ekizu
