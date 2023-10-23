#ifndef EKIZU_MESSAGE_COMPONENT_HPP
#define EKIZU_MESSAGE_COMPONENT_HPP

#include <ekizu/select_options.hpp>
#include <variant>

namespace ekizu
{
enum class ComponentType : uint8_t {
	ActionRow = 1,
	Button = 2,
	SelectMenu = 3,
	TextInput = 4,
};

struct EKIZU_EXPORT Button {
	ComponentType type{ ComponentType::Button };
	uint8_t style{};
	std::optional<std::string> label;
	std::optional<PartialEmoji> emoji;
	std::optional<std::string> custom_id;
	std::optional<std::string> url;
	std::optional<bool> disabled;
};

struct EKIZU_EXPORT SelectMenu {
	SelectMenu &set_custom_id(std::string custom_id_)
	{
		custom_id = std::move(custom_id_);
		return *this;
	}

	SelectMenu &set_options(const std::vector<SelectOptions> &options_)
	{
		options = options_;
		return *this;
	}

	SelectMenu &set_placeholder(std::string placeholder_)
	{
		placeholder = std::move(placeholder_);
		return *this;
	}

	SelectMenu &set_min_values(uint8_t min_values_)
	{
		min_values = min_values_;
		return *this;
	}

	SelectMenu &set_max_values(uint8_t max_values_)
	{
		max_values = max_values_;
		return *this;
	}

	SelectMenu &set_disabled(bool disabled_)
	{
		disabled = disabled_;
		return *this;
	}

	SelectMenu &add_option(const SelectOptions &option_)
	{
		options.push_back(option_);
		return *this;
	}

	SelectMenu &add_options(const std::vector<SelectOptions> &options_)
	{
		options.insert(options.end(), options_.begin(), options_.end());
		return *this;
	}

	/// The type of the component.
	ComponentType type{ ComponentType::SelectMenu };
	/// A unique identifier for the component (max 100 characters).
	std::string custom_id;
	/// The options for the select menu (max 25).
	std::vector<SelectOptions> options;
	/// The placeholder text for the select menu (max 150 characters).
	std::optional<std::string> placeholder;
	/// The minimum number of values that must be selected (default 1, min 0,
	/// max 25).
	std::optional<uint8_t> min_values;
	/// The maximum number of values that can be selected (default 1, max 25).
	std::optional<uint8_t> max_values;
	/// Whether the select menu is disabled (default false).
	std::optional<bool> disabled;
};

using ActionRowComponent = std::variant<Button, SelectMenu>;

/**
 * @brief A non-interactive container for component that holds other types of
 * components. You can have up to 5 Action Rows in a message, and Action Rows
 * cannot contain other Action Rows.
 *
 * @see
 * https://discord.com/developers/docs/interactions/message-components#action-rows
 *
 * @param client The client that created this Action Row.
 * @param data The JSON data to create this Action Row with.
 */
struct EKIZU_EXPORT ActionRow {
	ActionRow &add_component(const ActionRowComponent &component);
	ActionRow &
	add_components(const std::vector<ActionRowComponent> &components);

	ComponentType type{ ComponentType::ActionRow };
	std::vector<ActionRowComponent> components{};
};

using MessageComponent = std::variant<ActionRow, Button, SelectMenu>;
} // namespace ekizu

#endif // EKIZU_MESSAGE_COMPONENT_HPP
