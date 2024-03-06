#ifndef KOTERM_UTIL_TYPE_H
#define KOTERM_UTIL_TYPE_H

#include "koterm/util/debug.h"
#include <concepts>
#include <type_traits>
namespace koterm::util {

/**
 * @brief Determines if a type is an enumeration or integral type.
 *
 * This struct provides a compile-time boolean value indicating whether the given type is an enumeration or integral
 * type.
 *
 * @tparam T The type to check.
 */
template <typename T> struct is_enum_or_integral {
    static constexpr bool value = std::is_integral_v<T> || std::is_enum_v<T>;
};

/**
 * @brief Helper variable template for checking if a type is an enumeration or integral type.
 *
 * This variable template provides a boolean value indicating whether the given type is an enumeration or integral type.
 *
 * @tparam T The type to check.
 */
template <typename T> using is_enum_or_integral_v = is_enum_or_integral<T>::value;

/**
 * @brief Concept for types that are either enumeration or integral.
 *
 * This concept checks whether a type is either an enumeration or an integral type.
 *
 * @tparam T The type to check.
 */
template <typename T>
concept enum_or_integral = is_enum_or_integral<T>::value;

/**
 * @brief Concept for types that support bitwise operations.
 *
 * This concept checks whether a type supports bitwise operations such as OR, AND, NOT, XOR, and bit shifting.
 *
 * @tparam T The type to check.
 */
template <typename T>
concept bitwise_ops = requires(T a, T b) {
    // or
    { a | b } -> std::same_as<T>;
    // and
    { a& b } -> std::same_as<T>;
    // not
    { ~a } -> std::same_as<T>;
    // xor
    { a ^ b } -> std::same_as<T>;
    // shift left
    { a << 1 } -> std::same_as<T>;
    // shift right
    { a >> 1 } -> std::same_as<T>;
};

/**
 * @brief Concept for types that represent bit flags.
 *
 * This concept checks whether a type is suitable for use as a set of bit flags,
 * i.e., it is either an enumeration or integral type and supports bitwise operations.
 *
 * @tparam T The type to check.
 */
template <typename T>
concept bit_flags_type = requires {
    requires enum_or_integral<T>;
    requires bitwise_ops<T>;
};

/**
 * @brief Determines if a type is a scoped enumeration.
 *
 * This struct provides a compile-time boolean value indicating whether the given type is a scoped enumeration.
 *
 * @tparam E The type to check.
 */
#ifdef __cpp_lib_is_scoped_enum
using std::is_scoped_enum;
#else
template <typename E> struct is_scoped_enum : std::bool_constant < requires {
    requires std::is_enum_v<E>;
    requires !std::is_convertible_v<E, std::underlying_type_t<E>>;
} > {};

#endif

/**
 * @brief Concept for types that represent scoped enumerations.
 *
 * This concept checks whether a type is a scoped enumeration.
 *
 * @tparam T The type to check.
 */
template <typename T>
concept scoped_enum = is_scoped_enum<T>::value;

/**
 * @brief Concept for types that are enumerations.
 *
 * This concept checks whether a type is an enumeration.
 *
 * @tparam T The type to check.
 */
template <typename T>
concept enum_type = std::is_enum_v<T>;

/**
 * @brief Converts an enumeration value to its underlying integral type.
 *
 * This function converts an enumeration value to its underlying integral type.
 *
 * @tparam T The enumeration type.
 * @param value The enumeration value.
 * @return The underlying integral value.
 */
template <enum_type T> inline constexpr std::underlying_type_t<T> to_underlying_type(T value) {
    return static_cast<std::underlying_type_t<T>>(value);
}

/**
 * @brief Conditional type selection based on debug mode.
 *
 * This alias template selects one type if debug mode is enabled, otherwise selects another type.
 *
 * @tparam Iftrue The type to select if debug mode is enabled.
 * @tparam Iffalse The type to select if debug mode is disabled.
 */
template <typename Iftrue, typename Iffalse> using debug_type = std::conditional_t<debug_mode(), Iftrue, Iffalse>;

}

#endif
