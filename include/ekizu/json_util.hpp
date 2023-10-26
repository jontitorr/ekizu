#ifndef EKIZU_JSON_UTIL_HPP
#define EKIZU_JSON_UTIL_HPP

#ifndef _WIN32
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
#endif
#include <nlohmann/json.hpp>
#ifndef _WIN32
#pragma GCC diagnostic pop
#endif
#include <optional>
#include <variant>

namespace ekizu::json_util
{
namespace detail
{
template <typename T> struct IsOptional : std::false_type {};

template <typename T> struct IsOptional<std::optional<T> > : std::true_type {};

template <typename T> constexpr bool IS_OPTIONAL_V = IsOptional<T>::value;

template <typename T> struct IsVariant : std::false_type {};

template <typename... Ts>
struct IsVariant<std::variant<Ts...> > : std::true_type {};

template <typename T> constexpr bool IS_VARIANT_V = IsVariant<T>::value;

template <typename T> struct IsVector : std::false_type {};

template <typename T> struct IsVector<std::vector<T> > : std::true_type {};

template <typename T> constexpr bool IS_VECTOR_V = IsVector<T>::value;

template <typename T, typename ToDeserialize = T>
void deserialize_impl(const nlohmann::json &value, T &dest)
{
	if (value.is_discarded() || value.is_null()) {
		return;
	}

	if constexpr (std::is_same_v<ToDeserialize, bool>) {
		if (!value.is_boolean()) {
			return;
		}
	} else if constexpr (std::is_same_v<ToDeserialize, std::string> ||
			     std::is_same_v<ToDeserialize, std::string_view>) {
		if (!value.is_string()) {
			return;
		}
	} else if constexpr (std::is_integral_v<ToDeserialize> ||
			     std::is_enum_v<ToDeserialize>) {
		if (!value.is_number_integer()) {
			return;
		}
	} else if constexpr (std::is_floating_point_v<ToDeserialize>) {
		if (!value.is_number_float()) {
			return;
		}
	} else if constexpr (IS_VECTOR_V<ToDeserialize>) {
		if (!value.is_array()) {
			return;
		}
	}

	// We have checked all possible cases.
	dest = value.get<ToDeserialize>();
}
} // namespace detail

/**
 * @brief Checks if all the keys are present in the JSON data.
 *
 * @tparam T The type of the first key, automatically deduced by the compiler.
 * @tparam Ts The types of the remaining keys, automatically deduced by the
 * @param data The JSON data.
 * @param key The first key.
 * @param keys The remaining keys.
 * @return true if all keys are present, false otherwise.
 */
template <typename T, typename... Ts>
bool not_null_all(const nlohmann::json &data, T &&key, Ts &&...keys)
{
	return ((data.contains(key) && !data[key].is_null()) && ... &&
		(data.contains(keys) && !data[keys].is_null()));
}

/**
 * @brief Checks recursively if all keys are present and not null. For example,
 * when passed ["a", "b", "c"], it will check if data["a"]["b"]["c"] is present
 * and not null.
 *
 * @tparam T The type of the first key, automatically deduced by the compiler.
 * @tparam Ts The types of the remaining keys, automatically deduced by the
 * compiler.
 * @param data The JSON data.
 * @param key The first key.
 * @param keys The remaining keys.
 * @return true if all keys are present and not null, false otherwise.
 */
template <typename T, typename... Ts>
inline bool not_null_recursive(const nlohmann::json &data, T &&key,
			       Ts &&...keys)
{
	if (!not_null_all(data, key)) {
		return false;
	}

	if constexpr (sizeof...(keys) == 0) {
		return true;
	} else {
		return not_null_recursive(data[key], keys...);
	}
}

/**
 * @brief Serializes a value to JSON.
 *
 * @tparam T The type of the value, automatically deduced by the compiler.
 * @param data The JSON data.
 * @param key The key to access the value.
 * @param value The value to serialize.
 */
template <typename T>
void serialize(nlohmann::json &data, std::string_view key, const T &value)
{
	if constexpr (std::is_enum_v<T>) {
		data[key] = static_cast<std::underlying_type_t<T> >(value);
	} else if constexpr (detail::IS_OPTIONAL_V<T>) {
		if (!value) {
			return;
		}

		return serialize(data, key, *value);
	} else if constexpr (detail::IS_VARIANT_V<T>) {
		return std::visit(
			[&data, &key](auto &&v) {
				return serialize(data, key, std::forward(v));
			},
			value);
	} else {
		data[key] = value;
	}
}

/**
 * @brief Sets value from JSON accessed by key to a variable, if it exists.
 *
 * @tparam T The type of the variable, automatically deduced by the compiler.
 * @param data The JSON data.
 * @param key The key to access the value.
 * @param value The variable to set the value to.
 *
 * @return true if the value was set, false otherwise.
 */
template <typename T>
bool deserialize(const nlohmann::json &data, std::string_view key, T &value)
{
	if (!not_null_all(data, key)) {
		return false;
	}

	if constexpr (detail::IS_OPTIONAL_V<T>) {
		detail::deserialize_impl<T, typename T::value_type>(data[key],
								    value);
	} else if constexpr (detail::IS_VARIANT_V<T>) {
		std::visit(
			[&data, &key](auto &v) {
				using V = std::decay_t<decltype(v)>;
				detail::deserialize_impl<V>(data[key], v);
			},
			value);
	} else {
		detail::deserialize_impl(data[key], value);
	}

	return true;
}

/**
 * @brief Gets a value from JSON accessed by key, or a default value if it does
 * not exist.
 *
 * @tparam T The type of the value.
 * @param data The JSON data.
 * @param key The key to access the value.
 * @return T The value, or a default value if it does not exist.
 */
template <typename T>
T get_or_default(const nlohmann::json &data, std::string_view key)
{
	if (not_null_all(data, key)) {
		return data[key].get<T>();
	}

	return T{};
}
} // namespace ekizu::json_util

#endif // EKIZU_JSON_UTIL_HPP
