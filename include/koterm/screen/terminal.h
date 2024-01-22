#ifndef KOTERM_SCREEN_TERMINAL_H
#define KOTERM_SCREEN_TERMINAL_H

#include "koterm/screen/unit.h"
#include "koterm/util/bits.h"

namespace koterm::screen {

class terminal {
public:
    enum class ColorSupport {
        COLOR16,
        COLOR256,
        TRUE_COLOR,
    };

    enum class FeatureFlags : std::uint8_t {
        NONE               = 0,
        MOUSE_SUPPORT      = 1 << 0,
        ALTERNATIVE_SCREEN = 1 << 1,
        SUPPORT_TRUE_COLOR = 1 << 2,

        ALL = MOUSE_SUPPORT | ALTERNATIVE_SCREEN | SUPPORT_TRUE_COLOR,
    };

    struct Dimensions {
        unit_t width, height;
    };

    using Features = util::bit_flags<FeatureFlags>;

    static bool init(Features features);
    static ColorSupport color_support();
    static Dimensions dimensions();
    static Features features();

    terminal() = delete;
};

}

#endif
