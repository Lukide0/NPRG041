#ifndef KOTERM_DIMENSIONS_H
#define KOTERM_DIMENSIONS_H

#include "koterm/unit.h"

namespace koterm {

struct Dimensions {
    unit_t width;
    unit_t height;

    constexpr bool operator==(const Dimensions& other) const { return other.width == width && other.height == height; }
};

}
#endif
