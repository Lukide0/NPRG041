#ifndef KOTERM_SCREEN_PALLETE_PALLETE_H
#define KOTERM_SCREEN_PALLETE_PALLETE_H

#include "koterm/terminal/Color.h"
#include <string_view>
namespace koterm::screen::pallete {

using koterm::terminal::Color;

struct Pallete {

    consteval Pallete(
        const Color& black_color,
        const Color& red_color,
        const Color& green_color,
        const Color& yellow_color,
        const Color& blue_color,
        const Color& magenta_color,
        const Color& cyan_color,
        const Color& white_color,

        const Color& bright_black_color,
        const Color& bright_red_color,
        const Color& bright_green_color,
        const Color& bright_yellow_color,
        const Color& bright_blue_color,
        const Color& bright_magenta_color,
        const Color& bright_cyan_color,
        const Color& bright_white_color,
        const Color& foreground_color,
        const Color& background_color
    )
        : black(black_color)
        , red(red_color)
        , green(green_color)
        , yellow(yellow_color)
        , blue(blue_color)
        , magenta(magenta_color)
        , cyan(cyan_color)
        , white(white_color)
        , bright_black(bright_black_color)
        , bright_red(bright_red_color)
        , bright_green(bright_green_color)
        , bright_yellow(bright_yellow_color)
        , bright_blue(bright_blue_color)
        , bright_magenta(bright_magenta_color)
        , bright_cyan(bright_cyan_color)
        , bright_white(bright_white_color)
        , foreground(foreground_color)
        , background(background_color) { }

    Color black;
    Color red;
    Color green;
    Color yellow;
    Color blue;
    Color magenta;
    Color cyan;
    Color white;

    Color bright_black;
    Color bright_red;
    Color bright_green;
    Color bright_yellow;
    Color bright_blue;
    Color bright_magenta;
    Color bright_cyan;
    Color bright_white;
    Color foreground;
    Color background;
};

}

#endif
