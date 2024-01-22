#ifndef KOTERM_SCREEN_UNIT_H
#define KOTERM_SCREEN_UNIT_H

#include <compare>
#include <cstdint>
#include <limits>
namespace koterm::screen {

using unit_t                  = std::uint32_t;
constexpr unit_t INVALID_UNIT = std::numeric_limits<unit_t>::max();

}

#endif
