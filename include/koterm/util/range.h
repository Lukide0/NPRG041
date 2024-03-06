#ifndef KOTERM_UTIL_RANGE
#define KOTERM_UTIL_RANGE

namespace koterm::util {

struct range {
    /**
     * @brief Checks if a value lies within a specified range.
     *
     * @tparam T The type of the value and the range boundaries.
     * @param value The value to check.
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     * @return True if the value lies within the range [min, max], otherwise false.
     */
    template <typename T> static constexpr bool inside(T value, T min, T max) { return min <= value && value <= max; }
};

}

#endif
