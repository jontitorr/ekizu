#ifndef EKIZU_EVENT_HPP
#define EKIZU_EVENT_HPP

#include <ekizu/gateway/channel_create.hpp>
#include <ekizu/gateway/channel_delete.hpp>
#include <ekizu/gateway/channel_pins_update.hpp>
#include <ekizu/gateway/channel_update.hpp>
#include <ekizu/gateway/guild_ban_add.hpp>
#include <ekizu/gateway/guild_ban_remove.hpp>
#include <ekizu/gateway/guild_create.hpp>
#include <ekizu/gateway/guild_delete.hpp>
#include <ekizu/gateway/guild_emojis_update.hpp>
#include <ekizu/gateway/guild_integrations_update.hpp>
#include <ekizu/gateway/guild_member_add.hpp>
#include <ekizu/gateway/guild_member_remove.hpp>
#include <ekizu/gateway/guild_member_update.hpp>
#include <ekizu/gateway/guild_members_chunk.hpp>
#include <ekizu/gateway/guild_role_create.hpp>
#include <ekizu/gateway/guild_role_delete.hpp>
#include <ekizu/gateway/guild_role_update.hpp>
#include <ekizu/gateway/guild_scheduled_event_create.hpp>
#include <ekizu/gateway/guild_scheduled_event_delete.hpp>
#include <ekizu/gateway/guild_scheduled_event_update.hpp>
#include <ekizu/gateway/guild_scheduled_event_user_add.hpp>
#include <ekizu/gateway/guild_scheduled_event_user_remove.hpp>
#include <ekizu/gateway/guild_stickers_update.hpp>
#include <ekizu/gateway/guild_update.hpp>
#include <ekizu/gateway/integration_create.hpp>
#include <ekizu/gateway/integration_delete.hpp>
#include <ekizu/gateway/integration_update.hpp>
#include <ekizu/gateway/interaction_create.hpp>
#include <ekizu/gateway/invite_create.hpp>
#include <ekizu/gateway/invite_delete.hpp>
#include <ekizu/gateway/log.hpp>
#include <ekizu/gateway/message_create.hpp>
#include <ekizu/gateway/message_delete.hpp>
#include <ekizu/gateway/message_delete_bulk.hpp>
#include <ekizu/gateway/message_reaction_add.hpp>
#include <ekizu/gateway/message_reaction_remove.hpp>
#include <ekizu/gateway/message_reaction_remove_all.hpp>
#include <ekizu/gateway/message_reaction_remove_emoji.hpp>
#include <ekizu/gateway/message_update.hpp>
#include <ekizu/gateway/presence_update.hpp>
#include <ekizu/gateway/ready.hpp>
#include <ekizu/gateway/resumed.hpp>
#include <ekizu/gateway/stage_instance_create.hpp>
#include <ekizu/gateway/stage_instance_delete.hpp>
#include <ekizu/gateway/stage_instance_update.hpp>
#include <ekizu/gateway/thread_create.hpp>
#include <ekizu/gateway/thread_delete.hpp>
#include <ekizu/gateway/thread_list_sync.hpp>
#include <ekizu/gateway/thread_member_update.hpp>
#include <ekizu/gateway/thread_members_update.hpp>
#include <ekizu/gateway/thread_update.hpp>
#include <ekizu/gateway/typing_start.hpp>
#include <ekizu/gateway/user_update.hpp>
#include <ekizu/gateway/voice_server_update.hpp>
#include <ekizu/gateway/voice_state_update.hpp>
#include <ekizu/gateway/webhooks_update.hpp>

namespace ekizu {
using Event = std::variant<
	GuildBanAdd, GuildBanRemove, ChannelCreate, ChannelDelete,
	ChannelPinsUpdate, ChannelUpdate, GuildCreate, GuildDelete,
	GuildEmojisUpdate, GuildIntegrationsUpdate, GuildMemberAdd,
	GuildMemberRemove, GuildMemberUpdate, GuildMembersChunk, GuildRoleCreate,
	GuildRoleDelete, GuildRoleUpdate, GuildScheduledEventCreate,
	GuildScheduledEventDelete, GuildScheduledEventUpdate,
	GuildScheduledEventUserAdd, GuildScheduledEventUserRemove,
	GuildStickersUpdate, GuildUpdate, IntegrationCreate, IntegrationDelete,
	IntegrationUpdate, InteractionCreate, InviteCreate, InviteDelete, Log,
	MessageCreate, MessageDelete, MessageDeleteBulk, MessageReactionAdd,
	MessageReactionRemove, MessageReactionRemoveAll, MessageReactionRemoveEmoji,
	MessageUpdate, PresenceUpdate, Ready, Resumed, StageInstanceCreate,
	StageInstanceDelete, StageInstanceUpdate, ThreadCreate, ThreadDelete,
	ThreadListSync, ThreadMemberUpdate, ThreadMembersUpdate, ThreadUpdate,
	TypingStart, UserUpdate, VoiceServerUpdate, VoiceStateUpdate,
	WebhooksUpdate>;
}  // namespace ekizu

#endif	// EKIZU_EVENT_HPP
