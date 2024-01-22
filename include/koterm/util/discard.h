#ifndef KOTERM_UTIL_DISCARD
#define KOTERM_UTIL_DISCARD

namespace koterm::util {

struct discard_t {
    template <typename T> constexpr void operator=(T&& /*unused*/) const noexcept { }
};

inline constexpr discard_t discard;

}

#endif
