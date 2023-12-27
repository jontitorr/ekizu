#ifndef EKIZU_REQUEST_MODIFY_CURRENT_USER_HPP
#define EKIZU_REQUEST_MODIFY_CURRENT_USER_HPP

#include <ekizu/http.hpp>
#include <ekizu/user.hpp>

namespace ekizu {
struct ModifyCurrentUserFields {
	std::optional<std::string> avatar{};
	std::optional<std::string> username{};
};

EKIZU_EXPORT void to_json(nlohmann::json &j, const ModifyCurrentUserFields &f);
EKIZU_EXPORT void from_json(const nlohmann::json &j,
							ModifyCurrentUserFields &f);

struct ModifyCurrentUser {
	explicit ModifyCurrentUser(
		const std::function<Result<net::HttpResponse>(
			net::HttpRequest, const asio::yield_context &)> &make_request);

	operator net::HttpRequest() const;

	ModifyCurrentUser &avatar(std::string_view avatar) {
		m_fields.avatar = avatar;
		return *this;
	}

	ModifyCurrentUser &username(std::string_view username) {
		// TODO: Validate username
		m_fields.username = username;
		return *this;
	}

	Result<User> send(const asio::yield_context &yield) const;

   private:
	ModifyCurrentUserFields m_fields;
	std::function<Result<net::HttpResponse>(
		net::HttpRequest, const asio::yield_context &)>
		m_make_request;
};
}  // namespace ekizu

#endif	// EKIZU_REQUEST_MODIFY_CURRENT_USER_HPP
