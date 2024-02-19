#ifndef KOTERM_SCREEN_PALLETE_PALLETES_H
#define KOTERM_SCREEN_PALLETE_PALLETES_H

#include "koterm/screen/pallete/Pallete.h"
#include "koterm/terminal/colors.h"
#include <string_view>

namespace koterm::screen::pallete {

using namespace koterm::terminal::colors;
using std::string_view_literals::operator""sv;

constexpr auto DEFAULT = Pallete(
    SYS_BLACK,
    SYS_RED,
    SYS_GREEN,
    SYS_YELLOW,
    SYS_BLUE,
    SYS_MAGENTA,
    SYS_CYAN,
    SYS_WHITE,
    SYS_BRIGHT_BLACK,
    SYS_BRIGHT_RED,
    SYS_BRIGHT_GREEN,
    SYS_BRIGHT_YELLOW,
    SYS_BRIGHT_BLUE,
    SYS_BRIGHT_MAGENTA,
    SYS_BRIGHT_CYAN,
    SYS_BRIGHT_WHITE,
    SYS_DEFAULT, // foreground
    SYS_DEFAULT // background
);

constexpr auto DRACULA = Pallete(
    "#44475A"sv,
    "#FF5555"sv,
    "#50FA7B"sv,
    "#FFB86C"sv,
    "#8BE9FD"sv,
    "#BD93F9"sv,
    "#FF79C6"sv,
    "#f8f8f2"sv,
    "#000000"sv,
    "#FF5555"sv,
    "#50FA7B"sv,
    "#FFB86C"sv,
    "#8BE9FD"sv,
    "#BD93F9"sv,
    "#FF79C6"sv,
    "#FFFFFF"sv,
    "#f8f8f2"sv,
    "#282A36"sv
);

constexpr auto UBUNTU = Pallete(
    "#2E3436"sv,
    "#CC0000"sv,
    "#4E9A06"sv,
    "#C4A000"sv,
    "#3465A4"sv,
    "#75507B"sv,
    "#06989A"sv,
    "#D3D7CF"sv,
    "#555753"sv,
    "#EF2929"sv,
    "#8AE234"sv,
    "#FCE94F"sv,
    "#729FCF"sv,
    "#AD7FA8"sv,
    "#34E2E2"sv,
    "#EEEEEC"sv,
    "#FFFFFF"sv,
    "#300A24"sv
);

constexpr auto ARGONAUT = Pallete(
    "#232323"sv,
    "#FF000F"sv,
    "#8CE10B"sv,
    "#FFB900"sv,
    "#008DF8"sv,
    "#6D43A6"sv,
    "#00D8EB"sv,
    "#FFFFFF"sv,
    "#444444"sv,
    "#FF2740"sv,
    "#ABE15B"sv,
    "#FFD242"sv,
    "#0092FF"sv,
    "#9A5FEB"sv,
    "#67FFF0"sv,
    "#FFFFFF"sv,
    "#FFFAF4"sv,
    "#0E1019"sv
);

constexpr auto DARKSIDE = Pallete(
    "#000000"sv,
    "#FF5555"sv,
    "#55FF55"sv,
    "#FFFF55"sv,
    "#5555FF"sv,
    "#FF55FF"sv,
    "#55FFFF"sv,
    "#BBBBBB"sv,
    "#555555"sv,
    "#FF5555"sv,
    "#55FF55"sv,
    "#FFFF55"sv,
    "#5555FF"sv,
    "#FF55FF"sv,
    "#55FFFF"sv,
    "#FFFFFF"sv,
    "#FFFFFF"sv,
    "#000000"sv
);

constexpr auto ELEMENTARY = Pallete(
    "#303030"sv,
    "#E1321A"sv,
    "#6AB017"sv,
    "#FFC005"sv,
    "#004F9E"sv,
    "#EC0048"sv,
    "#2AA7E7"sv,
    "#F2F2F2"sv,
    "#5D5D5D"sv,
    "#FF361E"sv,
    "#7BC91F"sv,
    "#FFD00A"sv,
    "#0071FF"sv,
    "#FF1D62"sv,
    "#4BB8FD"sv,
    "#A020F0"sv,
    "#F2F2F2"sv,
    "#101010"sv
);

}

#endif
