#ifndef KOTERM_UTIL_RANGE
#define KOTERM_UTIL_RANGE

namespace koterm::util {

struct range {
    template <typename T> static constexpr bool inside(T value, T min, T max) { return min <= value && value <= max; }
};

}

#endif
