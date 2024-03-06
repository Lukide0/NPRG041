#ifndef KOTERM_UTIL_DISCARD
#define KOTERM_UTIL_DISCARD

namespace koterm::util {

struct discard_t {
    template <typename T> constexpr void operator=(T&& /*unused*/) const noexcept { }
};

/**
 * @brief Instance of `discard_t`.
 *
 * This instance of `discard_t` is typically used to discard values in code where a value is intentionally unused to
 * avoid compiler warnings.
 */
inline constexpr discard_t discard;

}

#endif
