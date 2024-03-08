#ifndef KOTERM_EVENT_EVENT_H
#define KOTERM_EVENT_EVENT_H

#include "koterm/Dimensions.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/terminal/Parser.h"
#include "koterm/unit.h"
#include "koterm/util/ascii.h"
#include <string>
#include <string_view>
#include <variant>

namespace koterm::event {

/**
 * @brief Represents a character event containing a string character.
 */
struct CharacterEvent {
    std::string character;
};

/**
 * @brief Represents a mouse event.
 */
struct MouseEvent {
    enum class Btn {
        NONE,
        LEFT,
        RIGHT,
        MIDDLE,
    };

    MouseEvent() = default;

    /**
     * @brief Constructs a MouseEvent with provided data and button state.
     *
     * @param data The mouse event data.
     * @param btn The mouse button state.
     */
    MouseEvent(const terminal::Parser::MouseEvent& data)
        : m_data(data) {
        if (data.btn1()) {
            m_btn = Btn::LEFT;
        } else if (data.btn2()) {
            m_btn = Btn::MIDDLE;
        } else if (data.btn3()) {
            m_btn = Btn::RIGHT;
        }
    }

    /**
     * @brief Checks if the specified button is pressed.
     *
     * @tparam BTN The button state to check.
     * @return True if the specified button is pressed, otherwise false.
     */
    template <Btn BTN> [[nodiscard]] bool btn_pressed() const {
        return m_btn == BTN && m_data.is_button() && m_data.btn_press();
    }

    /**
     * @brief Checks if the specified button is released.
     *
     * @tparam BTN The button state to check.
     * @return True if the specified button is released, otherwise false.
     */
    template <Btn BTN> [[nodiscard]] bool btn_released() const {
        return m_btn == BTN && m_data.is_button() && m_data.btn_release();
    }

    /**
     * @brief Checks if the mouse is scrolled down.
     *
     * @return True if the mouse is scrolled down, otherwise false.
     */
    [[nodiscard]] bool scroll_down() const { return m_data.is_scroll() && m_data.scroll_forward(); }

    /**
     * @brief Checks if the mouse is scrolled up.
     *
     * @return True if the mouse is scrolled up, otherwise false.
     */
    [[nodiscard]] bool scroll_up() const { return m_data.is_scroll() && m_data.scroll_back(); }

    /**
     * @brief Checks if the control key is pressed.
     *
     * @return True if the control key is pressed, otherwise false.
     */
    [[nodiscard]] bool control() const { return m_data.control(); }

    /**
     * @brief Checks if the shift key is pressed.
     *
     * @return True if the shift key is pressed, otherwise false.
     */
    [[nodiscard]] bool shift() const { return m_data.shift(); }

    /**
     * @brief Checks if the meta key is pressed.
     *
     * @return True if the meta key is pressed, otherwise false.
     */
    [[nodiscard]] bool meta() const { return m_data.meta(); }

    /**
     * @brief Gets the position of the mouse.
     *
     * @return The position of the mouse.
     */
    [[nodiscard]] point_t pos() const { return m_data.mouse; }

private:
    terminal::Parser::MouseEvent m_data;
    Btn m_btn = Btn::NONE;
};

/**
 * @brief Represents an event containing various types of data such as resize, mouse movement, key press, etc.
 */
class Event {
public:
    enum class EventType {
        NONE,
        RESIZE,
        MOUSE_MOVE,
        MOUSE_BTN,
        MOUSE_SCROLL,
        CHARACTER,
        SPECIAL_KEY,
        CURSOR,
        SPECIAL_CHARACTER,
    };

    Event() = default;

    /**
     * @brief Gets the type of the event.
     *
     * @return The type of the event.
     */
    [[nodiscard]] EventType type() const { return m_type; }

    /**
     * @brief Gets the data associated with the specified event type.
     *
     * @tparam TYPE The type of the event data to retrieve.
     * @return The data associated with the specified event type.
     */
    template <EventType TYPE> [[nodiscard]] const auto& get() const {

        if constexpr (TYPE == EventType::RESIZE) {
            return std::get<Dimensions>(m_data);
        } else if constexpr (TYPE == EventType::MOUSE_BTN || TYPE == EventType::MOUSE_SCROLL || TYPE == EventType::MOUSE_MOVE) {
            return std::get<MouseEvent>(m_data);
        } else if constexpr (TYPE == EventType::CHARACTER) {
            return std::get<CharacterEvent>(m_data);
        } else if constexpr (TYPE == EventType::SPECIAL_KEY) {
            return std::get<KeyCode>(m_data);
        } else if constexpr (TYPE == EventType::CURSOR) {
            return std::get<point_t>(m_data);
        } else if constexpr (TYPE == EventType::SPECIAL_CHARACTER) {
            return std::get<util::ascii::codes>(m_data);
        }
    }

    static Event create_resize(const Dimensions& dims) {
        Event e;
        e.m_data = dims;
        e.m_type = EventType::RESIZE;

        return e;
    }

    static Event create_mouse_move(const terminal::Parser::MouseEvent& mouse_info) {
        Event e;
        e.m_data = MouseEvent {
            mouse_info,
        };
        e.m_type = EventType::MOUSE_MOVE;

        return e;
    }

    static Event create_mouse_scroll(const terminal::Parser::MouseEvent& mouse_info) {
        Event e;
        e.m_data = MouseEvent { mouse_info };
        e.m_type = EventType::MOUSE_SCROLL;

        return e;
    }

    static Event create_mouse_btn(const terminal::Parser::MouseEvent& mouse_info) {
        Event e;
        e.m_data = MouseEvent { mouse_info };
        e.m_type = EventType::MOUSE_BTN;

        return e;
    }

    static Event create_character(std::string_view character) {
        Event e;
        e.m_data = CharacterEvent { std::string { character } };
        e.m_type = EventType::CHARACTER;

        return e;
    }

    static Event create_special_character(util::ascii::codes character) {
        Event e;
        e.m_data = character;
        e.m_type = EventType::SPECIAL_CHARACTER;

        return e;
    }

    static Event create_key(KeyCode key) {
        Event e;
        e.m_data = key;
        e.m_type = EventType::SPECIAL_KEY;

        return e;
    }

    static Event create_cursor(point_t cursor) {
        Event e;
        e.m_data = cursor;
        e.m_type = EventType::CURSOR;

        return e;
    }

    static Event create_from_mouse(const terminal::Parser::MouseEvent& mouse_info) {
        if (mouse_info.is_button()) {
            return create_mouse_btn(mouse_info);
        } else if (mouse_info.is_scroll()) {
            return create_mouse_scroll(mouse_info);
        } else {
            return create_mouse_move(mouse_info);
        }
    }

private:
    EventType m_type;

    std::variant<MouseEvent, KeyCode, CharacterEvent, point_t, Dimensions, util::ascii::codes> m_data;
};

}

#endif
