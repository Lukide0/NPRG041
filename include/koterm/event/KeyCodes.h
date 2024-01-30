#ifndef KOTERM_EVENT_KEYCODES_H
#define KOTERM_EVENT_KEYCODES_H

#include <cstdint>
namespace koterm::event {

enum class KeyCode : std::uint8_t {
    ARROW_UP,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,

    INSERT,
    DELETE,
    HOME,
    END,
    PAGE_UP,
    PAGE_DOWN,
};

}

#endif
