#ifndef KOTERM_UNIT_H
#define KOTERM_UNIT_H

#include <cstdint>
#include <limits>

namespace koterm {

using unit_t = std::uint32_t;

struct unit {
    static constexpr unit_t invalid = std::numeric_limits<unit_t>::max();
};

struct point_t {
    unit_t x;
    unit_t y;

    bool operator==(const point_t& other) const { return other.x == x && other.y == y; }
};

}

#endif
