#ifndef EKIZU_MESSAGE_UPDATE_HPP
#define EKIZU_MESSAGE_UPDATE_HPP

#include <ekizu/mention.hpp>
#include <ekizu/message.hpp>

namespace ekizu {
struct MessageUpdate {
	/// ID of the message.
	Snowflake id;
	/// ID of the channel the message was sent in.
	Snowflake channel_id;
	/// The author of this message (not guaranteed to be a valid user).
	std::optional<User> author;
	/// Contents of the message.
	std::optional<std::string> content;
	/// When this message was sent (ISO8601 timestamp).
	std::optional<std::string> timestamp;
	/// When this message was edited (or null if never) (ISO8601 timestamp).
	std::optional<std::string> edited_timestamp;
	/// Whether this was a TTS message.
	std::optional<bool> tts;
	/// Whether this message mentions everyone.
	std::optional<bool> mention_everyone;
	/// Users specifically mentioned in the message.
	std::optional<std::vector<Mention> > mentions;
	/// Roles specifically mentioned in this message.
	std::optional<std::vector<Snowflake> > mention_roles;
	/// Any attached files.
	std::optional<std::vector<Attachment> > attachments;
	/// Any embedded content.
	std::optional<std::vector<Embed> > embeds;
	/// Whether this message is pinned.
	std::optional<bool> pinned;
	/// The type of the message.
	std::optional<MessageType> type;
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const MessageUpdate &m);
EKIZU_EXPORT void from_json(const nlohmann::json &j, MessageUpdate &m);
}  // namespace ekizu

#endif	// EKIZU_MESSAGE_UPDATE_HPP
