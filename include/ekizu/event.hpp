#ifndef EKIZU_EVENT_HPP
#define EKIZU_EVENT_HPP

#include <ekizu/gateway/ban_add.hpp>
#include <ekizu/gateway/ban_remove.hpp>
#include <ekizu/gateway/channel_create.hpp>
#include <ekizu/gateway/channel_delete.hpp>
#include <ekizu/gateway/channel_pins_update.hpp>
#include <ekizu/gateway/channel_update.hpp>
#include <ekizu/gateway/guild_create.hpp>
#include <ekizu/gateway/guild_delete.hpp>
#include <ekizu/gateway/guild_emojis_update.hpp>
#include <ekizu/gateway/guild_integrations_update.hpp>
#include <ekizu/gateway/guild_scheduled_event_create.hpp>
#include <ekizu/gateway/guild_scheduled_event_delete.hpp>
#include <ekizu/gateway/guild_scheduled_event_update.hpp>
#include <ekizu/gateway/guild_scheduled_event_user_add.hpp>
#include <ekizu/gateway/guild_scheduled_event_user_remove.hpp>
#include <ekizu/gateway/guild_stickers_update.hpp>
#include <ekizu/gateway/guild_update.hpp>
#include <ekizu/gateway/log.hpp>
#include <ekizu/gateway/message_create.hpp>
#include <ekizu/gateway/ready.hpp>
#include <ekizu/gateway/resumed.hpp>

namespace ekizu
{
using Event =
	std::variant<BanAdd, BanRemove, ChannelCreate, ChannelDelete,
		     ChannelPinsUpdate, ChannelUpdate, GuildCreate, GuildDelete,
		     GuildEmojisUpdate, GuildIntegrationsUpdate,
		     GuildScheduledEventCreate, GuildScheduledEventDelete,
		     GuildScheduledEventUpdate, GuildScheduledEventUserAdd,
		     GuildScheduledEventUserRemove, GuildStickersUpdate,
		     GuildUpdate, Log, MessageCreate, Ready, Resumed>;
} // namespace ekizu

#endif // EKIZU_EVENT_HPP
