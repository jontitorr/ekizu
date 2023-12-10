#include <ekizu/json_util.hpp>
#include <ekizu/message_component.hpp>

namespace ekizu {
using json_util::deserialize;
using json_util::serialize;

void to_json(nlohmann::json &j, const Button &b) {
	serialize(j, "type", b.type);
	serialize(j, "style", b.style);
	serialize(j, "label", b.label);
	serialize(j, "emoji", b.emoji);
	serialize(j, "custom_id", b.custom_id);
	serialize(j, "url", b.url);
	serialize(j, "disabled", b.disabled);
}

void from_json(const nlohmann::json &j, Button &b) {
	deserialize(j, "type", b.type);
	deserialize(j, "style", b.style);
	deserialize(j, "label", b.label);
	deserialize(j, "emoji", b.emoji);
	deserialize(j, "custom_id", b.custom_id);
	deserialize(j, "url", b.url);
	deserialize(j, "disabled", b.disabled);
}

void to_json(nlohmann::json &j, const SelectOptions &o) {
	serialize(j, "label", o.label);
	serialize(j, "value", o.value);
	serialize(j, "description", o.description);
	serialize(j, "emoji", o.emoji);
	serialize(j, "default", o.default_);
}

void from_json(const nlohmann::json &j, SelectOptions &o) {
	deserialize(j, "label", o.label);
	deserialize(j, "value", o.value);
	deserialize(j, "description", o.description);
	deserialize(j, "emoji", o.emoji);
	deserialize(j, "default", o.default_);
}

void to_json(nlohmann::json &j, const SelectMenu &s) {
	serialize(j, "type", s.type);
	serialize(j, "custom_id", s.custom_id);
	serialize(j, "options", s.options);
	serialize(j, "placeholder", s.placeholder);
	serialize(j, "min_values", s.min_values);
	serialize(j, "max_values", s.max_values);
	serialize(j, "disabled", s.disabled);
}

void from_json(const nlohmann::json &j, SelectMenu &s) {
	deserialize(j, "type", s.type);
	deserialize(j, "custom_id", s.custom_id);
	deserialize(j, "options", s.options);
	deserialize(j, "placeholder", s.placeholder);
	deserialize(j, "min_values", s.min_values);
	deserialize(j, "max_values", s.max_values);
	deserialize(j, "disabled", s.disabled);
}

void to_json(nlohmann::json &j, const ActionRowComponent &c) {
	std::visit([&j](const auto &v) { to_json(j, v); }, c);
}

void from_json(const nlohmann::json &j, ActionRowComponent &c) {
	json_util::detail::deserialize_impl(j, c);
}

void to_json(nlohmann::json &j, const ActionRow &a) {
	serialize(j, "type", a.type);
	serialize(j, "components", a.components);
}

void from_json(const nlohmann::json &j, ActionRow &a) {
	deserialize(j, "type", a.type);
	deserialize(j, "components", a.components);
}

void to_json(nlohmann::json &j, const MessageComponent &c) {
	std::visit([&j](const auto &v) { to_json(j, v); }, c);
}

void from_json(const nlohmann::json &j, MessageComponent &c) {
	json_util::detail::deserialize_impl(j, c);
}
}  // namespace ekizu
