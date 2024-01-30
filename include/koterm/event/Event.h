#ifndef KOTERM_EVENT_EVENT_H
#define KOTERM_EVENT_EVENT_H

#include "koterm/event/KeyCodes.h"
#include "koterm/terminal/Parser.h"
#include "koterm/terminal/unit.h"
#include <string>

namespace koterm::event {

struct CharacterEvent {
    std::string character;
};

struct MouseEvent {

    enum class Btn {
        LEFT,
        RIGHT,
        MIDDLE,
    };

    template <Btn BTN> [[nodiscard]] bool btn() const {
        switch (BTN) {
        case Btn::LEFT:
            return btn1() && !data.is_scroll();
        case Btn::RIGHT:
            return btn3() && !data.is_scroll();
        case Btn::MIDDLE:
            return btn2() && !data.is_scroll();
        }
    }

    template <Btn BTN> [[nodiscard]] bool btn_pressed() const { return btn<BTN>() && data.btn_down(); }
    template <Btn BTN> [[nodiscard]] bool btn_released() const { return btn<BTN>() && data.btn_up(); }

    [[nodiscard]] bool scroll_down() const { return data.is_scroll() && data.scroll_forward(); }
    [[nodiscard]] bool scroll_up() const { return data.is_scroll() && data.scroll_back(); }

    [[nodiscard]] bool control() const { return data.control(); }
    [[nodiscard]] bool shift() const { return data.shift(); }
    [[nodiscard]] bool meta() const { return data.meta(); }

    [[nodiscard]] terminal::point_t pos() const { return data.mouse; }

private:
    terminal::Parser::MouseEvent data;

    [[nodiscard]] bool btn1() const { return data.btn1(); }
    [[nodiscard]] bool btn2() const { return data.btn2(); }
    [[nodiscard]] bool btn3() const { return data.btn3(); }
};

class Event {
public:
    enum class EventType {
        RESIZE,
        MOUSE_BTN,
        MOUSE_SCROLL,
        KEY,
        SPECIAL_KEY,
    };

    [[nodiscard]] EventType type() const { return m_type; }

private:
    EventType m_type;

    union { };
};

}

#endif
