#ifndef EKIZU_SELECT_OPTIONS_HPP
#define EKIZU_SELECT_OPTIONS_HPP

#include <ekizu/emoji.hpp>

namespace ekizu
{
struct EKIZU_EXPORT SelectOptions {
	std::string label;
	std::string value;
	std::optional<std::string> description;
	std::optional<PartialEmoji> emoji;
	std::optional<bool> default_;
};
} // namespace ekizu

#endif // EKIZU_SELECT_OPTIONS_HPP
