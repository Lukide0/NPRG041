#ifndef KOTERM_UTIL_TYPE_H
#define KOTERM_UTIL_TYPE_H

#include "koterm/util/debug.h"
#include <concepts>
#include <type_traits>
namespace koterm::util {

template <typename T> struct is_enum_or_integral {
    static constexpr bool value = std::is_integral_v<T> || std::is_enum_v<T>;
};

template <typename T> using is_enum_or_integral_v = is_enum_or_integral<T>::value;

template <typename T>
concept enum_or_integral = is_enum_or_integral<T>::value;

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

template <typename T>
concept bit_flags_type = requires {
    requires enum_or_integral<T>;
    requires bitwise_ops<T>;
};

#ifdef __cpp_lib_is_scoped_enum
using std::is_scoped_enum;
#else
template <typename E> struct is_scoped_enum : std::bool_constant < requires {
    requires std::is_enum_v<E>;
    requires !std::is_convertible_v<E, std::underlying_type_t<E>>;
} > {};

#endif

template <typename T>
concept scoped_enum = is_scoped_enum<T>::value;

template <typename T>
concept enum_type = std::is_enum_v<T>;

template <enum_type T> constexpr std::underlying_type_t<T> to_underlying_type(T value) {
    return static_cast<std::underlying_type_t<T>>(value);
}

template <typename Iftrue, typename Iffalse> using debug_type = std::conditional_t<debug_mode(), Iftrue, Iffalse>;

}

#endif
