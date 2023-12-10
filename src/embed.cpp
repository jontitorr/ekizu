#include <ekizu/embed.hpp>
#include <ekizu/json_util.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const EmbedImage &i) {
	serialize(j, "url", i.url);
	serialize(j, "proxy_url", i.proxy_url);
	serialize(j, "height", i.height);
	serialize(j, "width", i.width);
}

void from_json(const nlohmann::json &j, EmbedImage &i) {
	deserialize(j, "url", i.url);
	deserialize(j, "proxy_url", i.proxy_url);
	deserialize(j, "height", i.height);
	deserialize(j, "width", i.width);
}

void to_json(nlohmann::json &j, const EmbedProvider &p) {
	serialize(j, "name", p.name);
	serialize(j, "url", p.url);
}

void from_json(const nlohmann::json &j, EmbedProvider &p) {
	deserialize(j, "name", p.name);
	deserialize(j, "url", p.url);
}

void to_json(nlohmann::json &j, const EmbedAuthor &a) {
	serialize(j, "name", a.name);
	serialize(j, "url", a.url);
	serialize(j, "icon_url", a.icon_url);
	serialize(j, "proxy_icon_url", a.proxy_icon_url);
}

void from_json(const nlohmann::json &j, EmbedAuthor &a) {
	deserialize(j, "name", a.name);
	deserialize(j, "url", a.url);
	deserialize(j, "icon_url", a.icon_url);
	deserialize(j, "proxy_icon_url", a.proxy_icon_url);
}

void to_json(nlohmann::json &j, const EmbedFooter &f) {
	serialize(j, "text", f.text);
	serialize(j, "icon_url", f.icon_url);
	serialize(j, "proxy_icon_url", f.proxy_icon_url);
}

void from_json(const nlohmann::json &j, EmbedFooter &f) {
	deserialize(j, "text", f.text);
	deserialize(j, "icon_url", f.icon_url);
	deserialize(j, "proxy_icon_url", f.proxy_icon_url);
}

void to_json(nlohmann::json &j, const EmbedField &f) {
	serialize(j, "name", f.name);
	serialize(j, "value", f.value);
	serialize(j, "inline", f.inline_);
}

void from_json(const nlohmann::json &j, EmbedField &f) {
	deserialize(j, "name", f.name);
	deserialize(j, "value", f.value);
	deserialize(j, "inline", f.inline_);
}

void to_json(nlohmann::json &j, const Embed &e) {
	serialize(j, "title", e.title);
	serialize(j, "description", e.description);
	serialize(j, "url", e.url);
	serialize(j, "timestamp", e.timestamp);
	serialize(j, "color", e.color);
	serialize(j, "author", e.author);
	serialize(j, "footer", e.footer);
	serialize(j, "thumbnail", e.thumbnail);
	serialize(j, "image", e.image);
	serialize(j, "provider", e.provider);
	serialize(j, "video", e.video);
	serialize(j, "fields", e.fields);
}

void from_json(const nlohmann::json &j, Embed &e) {
	deserialize(j, "title", e.title);
	deserialize(j, "description", e.description);
	deserialize(j, "url", e.url);
	deserialize(j, "timestamp", e.timestamp);
	deserialize(j, "color", e.color);
	deserialize(j, "author", e.author);
	deserialize(j, "footer", e.footer);
	deserialize(j, "thumbnail", e.thumbnail);
	deserialize(j, "image", e.image);
	deserialize(j, "provider", e.provider);
	deserialize(j, "video", e.video);
	deserialize(j, "fields", e.fields);
}
}  // namespace ekizu
