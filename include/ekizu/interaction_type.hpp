#ifndef EKIZU_INTERACTION_TYPE_HPP
#define EKIZU_INTERACTION_TYPE_HPP

#include <cstdint>

namespace ekizu
{
enum class InteractionType : uint8_t {
	Ping = 1,
	ApplicationCommand,
	MessageComponent,
	ApplicationCommandAutocomplete,
	ModalSubmit,
};
} // namespace ekizu

#endif // EKIZU_INTERACTION_TYPE_HPP
