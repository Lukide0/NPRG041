#ifndef KOTERM_TERMINAL_CURSOR_H
#define KOTERM_TERMINAL_CURSOR_H

#include "koterm/terminal/ansi.h"
#include "koterm/unit.h"
#include <iostream>
namespace koterm::terminal {

class Cursor {
public:
    enum class EraseMode {
        CURSOR_TO_SCREEN_END,
        SCREEN_TO_CURSOR,
        SCREEN,
        CURSOR_TO_LINE_END,
        START_LINE_TO_CURSOR_LINE,
        LINE
    };

    static void move(unit_t x, unit_t y) { ansi::move_cursor(y + 1, x + 1, std::cout); }
    static void move_up(unit_t n) { ansi::move_cursor_up(n, std::cout); }
    static void move_down(unit_t n) { ansi::move_cursor_down(n, std::cout); }
    static void move_right(unit_t n) { ansi::move_cursor_right(n, std::cout); }
    static void move_left(unit_t n) { ansi::move_cursor_left(n, std::cout); }

    static void save() { std::cout << ansi::CURSOR_SAVE; }
    static void restore() { std::cout << ansi::CURSOR_RESTORE; }
    static void query() { std::cout << ansi::CURSOR_QUERY; }

    static void flush() { std::cout.flush(); }

    template <EraseMode MODE> static void erase() {
        switch (MODE) {
        case EraseMode::CURSOR_TO_SCREEN_END:
            std::cout << ansi::ERASE_FROM_CURSOR_TO_SCREEN_END;
            break;
        case EraseMode::SCREEN_TO_CURSOR:
            std::cout << ansi::ERASE_TO_CURSOR;
            break;
        case EraseMode::SCREEN:
            std::cout << ansi::ERASE_SCREEN;
            break;
        case EraseMode::CURSOR_TO_LINE_END:
            std::cout << ansi::ERASE_FROM_CURSOR_TO_LINE_END;
            break;
        case EraseMode::START_LINE_TO_CURSOR_LINE:
            std::cout << ansi::ERASE_FROM_START_LINE_TO_CURSOR_LINE;
            break;
        case EraseMode::LINE:
            std::cout << ansi::ERASE_LINE;
            break;
        }
    }
};

}

#endif
