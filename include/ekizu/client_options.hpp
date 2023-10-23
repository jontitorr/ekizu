#ifndef EKIZU_CLIENT_OPTIONS_HPP
#define EKIZU_CLIENT_OPTIONS_HPP

#include <ekizu/intents.hpp>
#include <optional>

namespace ekizu
{
struct CacheOptions {
	CacheOptions &set_aggressive(bool a)
	{
		aggressive = a;
		return *this;
	}

	CacheOptions &set_bans_capacity(size_t c)
	{
		bans_capacity = c;
		return *this;
	}

	CacheOptions &set_channels_capacity(size_t c)
	{
		channels_capacity = c;
		return *this;
	}

	CacheOptions &set_emojis_capacity(size_t c)
	{
		emojis_capacity = c;
		return *this;
	}

	CacheOptions &set_guilds_capacity(size_t c)
	{
		guilds_capacity = c;
		return *this;
	}

	CacheOptions &set_invites_capacity(size_t c)
	{
		invites_capacity = c;
		return *this;
	}

	CacheOptions &set_messages_capacity(size_t c)
	{
		messages_capacity = c;
		return *this;
	}

	CacheOptions &set_reactions_capacity(size_t c)
	{
		reactions_capacity = c;
		return *this;
	}

	CacheOptions &set_roles_capacity(size_t c)
	{
		roles_capacity = c;
		return *this;
	}

	CacheOptions &set_stickers_capacity(size_t c)
	{
		stickers_capacity = c;
		return *this;
	}

	CacheOptions &set_users_capacity(size_t c)
	{
		users_capacity = c;
		return *this;
	}

	CacheOptions &set_voice_states_capacity(size_t c)
	{
		voice_states_capacity = c;
		return *this;
	}

	CacheOptions &set_sweep_interval(uint32_t i)
	{
		sweep_interval = i;
		return *this;
	}

	static constexpr size_t DEFAULT_CAPACITY = 300;

	bool aggressive{};
	size_t bans_capacity{ DEFAULT_CAPACITY };
	size_t channels_capacity{ DEFAULT_CAPACITY };
	size_t emojis_capacity{ DEFAULT_CAPACITY };
	size_t guilds_capacity{ DEFAULT_CAPACITY };
	size_t invites_capacity{ DEFAULT_CAPACITY };
	size_t messages_capacity{ DEFAULT_CAPACITY };
	size_t reactions_capacity{ DEFAULT_CAPACITY };
	size_t roles_capacity{ DEFAULT_CAPACITY };
	size_t stickers_capacity{ DEFAULT_CAPACITY };
	size_t users_capacity{ DEFAULT_CAPACITY };
	size_t voice_states_capacity{ DEFAULT_CAPACITY };
	uint32_t sweep_interval{};
};

enum class PartialSupport : uint8_t {
	/// No partial support.
	None = 0,
	/// Partial support for guilds.
	Guilds = (1 << 0),
	/// Partial support for members.
	Members = (1 << 1),
	/// Partial support for channels.
	Channels = (1 << 2),
	/// Partial support for messages.
	Messages = (1 << 3),
	/// Partial support for reactions.
	Reactions = (1 << 4),
	/// Partial support for presences.
	Presences = (1 << 5),
	/// Partial support for emojis.
	Emojis = (1 << 6),
	/// Partial support for all.
	All = Guilds | Members | Channels | Messages | Reactions | Presences |
	      Emojis
};

struct ClientOptions {
	ClientOptions &with_cache_options(const CacheOptions &c)
	{
		cache_options = c;
		return *this;
	}

	ClientOptions &with_compression(bool c)
	{
		compression = c;
		return *this;
	}

	ClientOptions &with_intents(Intents i)
	{
		intents = i;
		return *this;
	}

	ClientOptions &with_partial_support(PartialSupport p)
	{
		partial_support = p;
		return *this;
	}

	std::optional<CacheOptions> cache_options;
	bool compression{ true };
	Intents intents{ Intents::DefaultIntents };
	PartialSupport partial_support{ PartialSupport::None };
	bool is_bot{ true };
};
} // namespace ekizu

#endif // EKIZU_CLIENT_OPTIONS_HPP
