#ifndef KOTERM_TERMINAL_TILE_H
#define KOTERM_TERMINAL_TILE_H

#include "koterm/util/ConstMap.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <compare>
#include <cstdint>
#include <limits>
#include <string_view>
#include <utility>

namespace koterm::terminal {

struct TileEncoding {
private:
    static constexpr std::uint8_t MASK = 0b11;

    static constexpr std::uint8_t TOP_OFFSET    = 0;
    static constexpr std::uint8_t RIGHT_OFFSET  = 2;
    static constexpr std::uint8_t BOTTOM_OFFSET = 4;
    static constexpr std::uint8_t LEFT_OFFSET   = 6;

    [[nodiscard]] constexpr std::uint8_t get_value(std::uint8_t offset) const { return static_cast<std::uint8_t>((encoding >> offset) & MASK); }
    [[nodiscard]] constexpr std::uint8_t get_part(std::uint8_t offset) const { return static_cast<std::uint8_t>((MASK << offset) & encoding); }

public:
    static constexpr std::uint8_t NONE       = 0;
    static constexpr std::uint8_t ALL_DOUBLE = std::numeric_limits<std::uint8_t>::max();

    constexpr TileEncoding(std::uint8_t top, std::uint8_t right, std::uint8_t bottom, std::uint8_t left)
        : encoding(
            ((top & MASK) << TOP_OFFSET) | ((right & MASK) << RIGHT_OFFSET) | ((bottom & MASK) << BOTTOM_OFFSET)
            | ((left & MASK) << LEFT_OFFSET)
        ) { }

    constexpr TileEncoding()
        : encoding(NONE) { }
    explicit constexpr TileEncoding(std::uint8_t code)
        : encoding(code) { }

    [[nodiscard]] constexpr std::uint8_t top() const { return get_value(TOP_OFFSET); }
    [[nodiscard]] constexpr std::uint8_t right() const { return get_value(RIGHT_OFFSET); }
    [[nodiscard]] constexpr std::uint8_t bottom() const { return get_value(BOTTOM_OFFSET); }
    [[nodiscard]] constexpr std::uint8_t left() const { return get_value(LEFT_OFFSET); }

    [[nodiscard]] constexpr std::uint8_t max_value() const {
        return std::max(std::max(top(), bottom()), std::max(left(), right()));
    }

    [[nodiscard]] constexpr TileEncoding combine(TileEncoding tile) const {

        // special case for max_value=3 (0b11)
        constexpr auto lookup = std::to_array<std::uint8_t>({ 0, 1, 0, 3 });

        std::uint8_t max_top    = std::max(tile.top(), top());
        std::uint8_t max_bottom = std::max(tile.bottom(), bottom());
        std::uint8_t max_left   = std::max(tile.left(), left());
        std::uint8_t max_right  = std::max(tile.right(), right());

        const std::uint8_t max_value = std::max(std::max(max_top, max_bottom), std::max(max_left, max_right));

        if (max_value != 0b11) {
            return { max_top, max_right, max_bottom, max_left };
        } else {
            return { lookup[max_top], lookup[max_right], lookup[max_bottom], lookup[max_left] };
        }
    }

    auto operator<=>(const TileEncoding&) const = default;
    constexpr TileEncoding operator|(TileEncoding tile) const { return combine(tile); }

    std::uint8_t encoding;
};

template <std::size_t Size> using tilemap_t = util::ConstMap<TileEncoding, std::string_view, Size>;
using tilemap_pair_t                        = std::pair<TileEncoding, std::string_view>;

constexpr auto TILES_MAP = util::to_map(std::to_array<tilemap_pair_t>({
    {{ 0, 0, 0, 0 },   " "},
 // Light
    {{ 0, 1, 0, 1 }, "─"},
    {{ 1, 0, 1, 0 }, "│"},
    {{ 0, 1, 1, 0 }, "┌"},
    {{ 0, 0, 1, 1 }, "┐"},
    {{ 1, 1, 0, 0 }, "└"},
    {{ 1, 0, 0, 1 }, "┘"},
    {{ 1, 1, 1, 0 }, "├"},
    {{ 1, 0, 1, 1 }, "┤"},
    {{ 0, 1, 1, 1 }, "┬"},
    {{ 1, 1, 0, 1 }, "┴"},
    {{ 1, 1, 1, 1 }, "┼"},
    {{ 1, 0, 0, 0 }, "╵"},
    {{ 0, 1, 0, 0 }, "╶"},
    {{ 0, 0, 1, 0 }, "╷"},
    {{ 0, 0, 0, 1 }, "╴"},
 // Bold
    {{ 0, 2, 0, 2 }, "━"},
    {{ 2, 0, 2, 0 }, "┃"},
    {{ 0, 2, 2, 0 }, "┏"},
    {{ 0, 0, 2, 2 }, "┓"},
    {{ 2, 2, 0, 0 }, "┗"},
    {{ 2, 0, 0, 2 }, "┛"},
    {{ 2, 2, 2, 0 }, "┣"},
    {{ 2, 0, 2, 2 }, "┫"},
    {{ 0, 2, 2, 2 }, "┳"},
    {{ 2, 2, 0, 2 }, "┻"},
    {{ 2, 2, 2, 2 }, "╋"},
    {{ 2, 0, 0, 0 }, "╹"},
    {{ 0, 2, 0, 0 }, "╺"},
    {{ 0, 0, 2, 0 }, "╻"},
    {{ 0, 0, 0, 2 }, "╸"},
 // Double
    {{ 0, 3, 0, 3 }, "═"},
    {{ 3, 0, 3, 0 }, "║"},
    {{ 0, 3, 3, 0 }, "╔"},
    {{ 0, 0, 3, 3 }, "╗"},
    {{ 3, 3, 0, 0 }, "╚"},
    {{ 3, 0, 0, 3 }, "╝"},
    {{ 3, 3, 3, 0 }, "╠"},
    {{ 3, 0, 3, 3 }, "╣"},
    {{ 0, 3, 3, 3 }, "╦"},
    {{ 3, 3, 0, 3 }, "╩"},
    {{ 3, 3, 3, 3 }, "╬"},
    {{ 3, 0, 0, 0 }, "║"},
    {{ 0, 3, 0, 0 }, "═"},
    {{ 0, 0, 3, 0 }, "║"},
    {{ 0, 0, 0, 3 }, "═"},
 // Light-Bold
    {{ 0, 1, 0, 2 }, "╼"},
    {{ 0, 2, 0, 1 }, "╾"},
    {{ 2, 0, 1, 0 }, "╿"},
    {{ 1, 0, 2, 0 }, "╽"},

    {{ 0, 2, 1, 0 }, "┍"},
    {{ 0, 1, 2, 0 }, "┎"},

    {{ 0, 0, 1, 2 }, "┑"},
    {{ 0, 0, 2, 1 }, "┒"},

    {{ 1, 2, 0, 0 }, "┕"},
    {{ 2, 1, 0, 0 }, "┖"},

    {{ 1, 0, 0, 2 }, "┙"},
    {{ 2, 0, 0, 1 }, "┚"},

    {{ 2, 1, 1, 0 }, "┞"},
    {{ 1, 2, 1, 0 }, "┝"},
    {{ 1, 1, 2, 0 }, "┟"},
    {{ 2, 1, 2, 0 }, "┠"},
    {{ 2, 2, 1, 0 }, "┡"},
    {{ 1, 2, 2, 0 }, "┢"},

    {{ 2, 0, 1, 1 }, "┦"},
    {{ 1, 0, 2, 1 }, "┧"},
    {{ 1, 0, 1, 2 }, "┥"},
    {{ 2, 0, 2, 1 }, "┨"},
    {{ 2, 0, 1, 2 }, "┩"},
    {{ 1, 0, 2, 2 }, "┪"},

    {{ 0, 2, 1, 1 }, "┮"},
    {{ 0, 1, 2, 1 }, "┰"},
    {{ 0, 1, 1, 2 }, "┭"},
    {{ 0, 2, 1, 2 }, "┯"},
    {{ 0, 1, 2, 2 }, "┱"},
    {{ 0, 2, 2, 1 }, "┲"},

    {{ 2, 1, 0, 1 }, "┸"},
    {{ 1, 2, 0, 1 }, "┶"},
    {{ 1, 1, 0, 2 }, "┵"},
    {{ 1, 2, 0, 2 }, "┷"},
    {{ 2, 1, 0, 2 }, "┹"},
    {{ 2, 2, 0, 1 }, "┺"},

    {{ 2, 1, 1, 1 }, "╀"},
    {{ 1, 2, 1, 1 }, "┾"},
    {{ 1, 1, 2, 1 }, "╁"},
    {{ 1, 1, 1, 2 }, "┽"},
    {{ 1, 2, 1, 2 }, "┿"},
    {{ 2, 1, 2, 1 }, "╂"},
    {{ 2, 1, 1, 2 }, "╃"},
    {{ 2, 2, 1, 1 }, "╄"},
    {{ 1, 1, 2, 2 }, "╅"},
    {{ 1, 2, 2, 1 }, "╆"},
    {{ 2, 2, 1, 2 }, "╇"},
    {{ 1, 2, 2, 2 }, "╈"},
    {{ 2, 1, 2, 2 }, "╉"},
    {{ 2, 2, 2, 1 }, "╊"},
 // Light-Double
    {{ 0, 3, 0, 1 }, "═"},
    {{ 0, 1, 0, 3 }, "═"},
    {{ 3, 0, 1, 0 }, "║"},
    {{ 1, 0, 3, 0 }, "║"},
    {{ 0, 3, 1, 0 }, "╒"},
    {{ 0, 1, 3, 0 }, "╓"},

    {{ 0, 0, 1, 3 }, "╕"},
    {{ 0, 0, 3, 1 }, "╖"},

    {{ 1, 3, 0, 0 }, "╘"},
    {{ 3, 1, 0, 0 }, "╙"},

    {{ 1, 0, 0, 3 }, "╛"},
    {{ 3, 0, 0, 1 }, "╜"},

    {{ 1, 3, 1, 0 }, "╞"},
    {{ 3, 1, 3, 0 }, "╟"},
    {{ 3, 3, 1, 0 }, "╚"},
    {{ 3, 1, 1, 0 }, "║"},
    {{ 1, 1, 3, 0 }, "║"},
    {{ 1, 3, 3, 0 }, "╔"},

    {{ 1, 0, 1, 3 }, "╡"},
    {{ 1, 0, 3, 3 }, "╗"},
    {{ 3, 0, 3, 1 }, "╢"},
    {{ 3, 0, 1, 1 }, "║"},
    {{ 3, 0, 1, 3 }, "╝"},
    {{ 1, 0, 3, 1 }, "║"},

    {{ 0, 3, 1, 3 }, "╤"},
    {{ 0, 1, 3, 1 }, "╥"},
    {{ 0, 1, 3, 3 }, "╗"},
    {{ 0, 1, 1, 3 }, "═"},
    {{ 0, 3, 3, 1 }, "╔"},
    {{ 0, 3, 1, 1 }, "═"},

    {{ 1, 3, 0, 3 }, "╧"},
    {{ 3, 1, 0, 1 }, "╨"},
    {{ 1, 3, 0, 1 }, "═"},
    {{ 3, 3, 0, 1 }, "╚"},
    {{ 1, 1, 0, 3 }, "═"},
    {{ 3, 1, 0, 3 }, "╝"},

    {{ 3, 1, 3, 1 }, "╫"},
    {{ 1, 3, 1, 3 }, "╪"},
    {{ 1, 1, 3, 3 }, "╗"},
    {{ 3, 1, 1, 3 }, "╬"},

    {{ 3, 1, 1, 1 }, "╫"},
    {{ 1, 3, 1, 1 }, "╪"},
    {{ 1, 1, 3, 1 }, "╫"},
    {{ 1, 1, 1, 3 }, "╪"},

    {{ 1, 3, 3, 3 }, "╦"},
    {{ 3, 1, 3, 3 }, "╣"},
    {{ 3, 3, 1, 3 }, "╩"},
    {{ 3, 3, 3, 1 }, "╠"},
    {{ 3, 3, 1, 1 }, "╚"},
    {{ 1, 3, 3, 1 }, "╔"},
}));

static_assert(!TILES_MAP.contains_duplicate_key());

}
#endif
