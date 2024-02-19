#ifndef KOTERM_TERMINAL_PARSER_H
#define KOTERM_TERMINAL_PARSER_H

#include "koterm/event/KeyCodes.h"
#include "koterm/unit.h"
#include "koterm/util/ascii.h"
#include "koterm/util/bits.h"
#include "koterm/util/utf8.h"
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

namespace koterm::terminal {

class Parser {
public:
    using span_t = std::span<std::uint8_t>;

    enum class ParserState { UNCOMPLETE, UNKNOWN, EVENT, CHARACTER, SPECIAL };

    enum class EventType { NONE, MOUSE, KEY, CURSOR };

    struct MouseEvent {
        using bits = util::bits<std::uint8_t>;

        static constexpr std::uint8_t MB1_BIT = 0;
        static constexpr std::uint8_t MB2_BIT = 0;
        static constexpr std::uint8_t MB3_BIT = 1;

        static constexpr std::uint8_t SHIFT_BIT   = 2;
        static constexpr std::uint8_t META_BIT    = 3;
        static constexpr std::uint8_t CONTROL_BIT = 4;
        static constexpr std::uint8_t BUTTON_BIT  = 5;
        static constexpr std::uint8_t WHEEL_BIT   = 6;

        [[nodiscard]] bool btn1_press() const { return !bits::is_set<MB1_BIT>(code) && !bits::is_set<MB3_BIT>(code); }
        [[nodiscard]] bool btn2_press() const { return bits::is_set<MB2_BIT>(code); }
        [[nodiscard]] bool btn3_press() const { return bits::is_set<MB3_BIT>(code); }
        [[nodiscard]] bool btn_release() const { return bits::is_set<0, 1>(code); }
        [[nodiscard]] bool btn_press() const { return !btn_release(); }
        [[nodiscard]] bool scroll_back() const { return btn1_press(); }
        [[nodiscard]] bool scroll_forward() const { return btn2_press(); }
        [[nodiscard]] bool shift() const { return bits::is_set<SHIFT_BIT>(code); }
        [[nodiscard]] bool meta() const { return bits::is_set<META_BIT>(code); }
        [[nodiscard]] bool control() const { return bits::is_set<CONTROL_BIT>(code); }

        [[nodiscard]] bool is_scroll() const { return bits::is_set<WHEEL_BIT, BUTTON_BIT>(code); }
        [[nodiscard]] bool is_button() const {
            return bits::is_set<BUTTON_BIT>(code) && !bits::is_set<WHEEL_BIT>(code);
        }

        std::uint8_t code;
        point_t mouse;
    };

    Parser()              = default;
    Parser(Parser&&)      = delete;
    Parser(const Parser&) = delete;

    ParserState parse(std::uint8_t byte);

    [[nodiscard]] EventType event_type() const { return m_type; }
    [[nodiscard]] const MouseEvent& mouse() const { return m_mouse; }
    [[nodiscard]] event::KeyCode key() const { return m_key; }
    [[nodiscard]] util::ascii::codes code() const { return static_cast<util::ascii::codes>(m_buffer[0]); }
    [[nodiscard]] point_t cursor() const { return m_cursor; }

    [[nodiscard]] std::string_view text() const {
        return { reinterpret_cast<const char*>(m_buffer.data()), m_buffer.size() };
    }

    void clear() { m_buffer.clear(); }

private:
    unit_t span_to_value(span_t span) { return util::utf8_codepoint(span) - 32; }

    ParserState parse_value(span_t& content, span_t& out);
    ParserState parse_utf8(span_t content, std::size_t& out_bytes);
    ParserState parse_esc(span_t content);
    ParserState parse_key_ss3(span_t content);
    ParserState parse_key_csi(span_t content);
    ParserState parse_csi(span_t content);
    ParserState parse_cursor(span_t content);

    ParserState parse_tracking(span_t content);

    std::vector<std::uint8_t> m_buffer;
    EventType m_type;

    MouseEvent m_mouse;
    event::KeyCode m_key;
    point_t m_cursor;
};
}

#endif
