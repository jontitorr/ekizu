#include <ekizu/attachment.hpp>
#include <ekizu/json_util.hpp>
#include <net/http.hpp>

namespace ekizu
{
Result<std::string> Attachment::download() const
{
	const auto res = net::http::get(url);

	if (!res) {
		return tl::make_unexpected(res.error());
	}

	return res->body;
}

void to_json(nlohmann::json &j, const Attachment &a)
{
	using json_util::serialize;

	serialize(j, "id", a.id);
	serialize(j, "filename", a.filename);
	serialize(j, "description", a.description);
	serialize(j, "content_type", a.content_type);
	serialize(j, "size", a.size);
	serialize(j, "url", a.url);
	serialize(j, "proxy_url", a.proxy_url);
	serialize(j, "height", a.height);
	serialize(j, "width", a.width);
	serialize(j, "ephemeral", a.ephemeral);
}

void from_json(const nlohmann::json &j, Attachment &a)
{
	using json_util::deserialize;

	deserialize(j, "id", a.id);
	deserialize(j, "filename", a.filename);
	deserialize(j, "description", a.description);
	deserialize(j, "content_type", a.content_type);
	deserialize(j, "size", a.size);
	deserialize(j, "url", a.url);
	deserialize(j, "proxy_url", a.proxy_url);
	deserialize(j, "height", a.height);
	deserialize(j, "width", a.width);
	deserialize(j, "ephemeral", a.ephemeral);
}
} // namespace ekizu
