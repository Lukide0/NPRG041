#include "koterm/terminal/Parser.h"
#include "koterm/terminal/unit.h"
#include "koterm/util/ConstMap.h"
#include "koterm/util/ascii.h"
#include <charconv>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <span>
#include <string_view>

/**
 * Mouse tracking modes in xterm.
 *
 * - Normal tracking mode (parameter 1000 to DECSET):
 *   - CSI M CbCxCy on both button press and release.
 *   - Cb: 0=MB1 pressed, 1=MB2 pressed, 2=MB3 pressed, 3=release.
 *   - Additional bits in Cb encode modifiers (4=Shift, 8=Meta, 16=Control).
 *
 * - Wheel mice (64 added to event code):
 *   - CSI M CbCxCy for buttons 1 and 2.
 *
 * - Button-event tracking (parameter 1002 to DECSET):
 *   - CSI M CbCxCy on both button press and release.
 *   - CSI M@ CxCy for motion into cell x,y with button 1 down.
 *
 * - Any-event tracking (parameter 1003 to DECSET):
 *   - Same as button-event mode, but all motion events are reported, even if no mouse button is down.
 *
 * - Extended coordinates (UTF-8, 1005):
 *   - Enables UTF-8 encoding for Cx, Cy, and Cb.
 *   - Expands maximum encodable position to 2015.
 */

namespace koterm::terminal {
using util::ascii::codes;

Parser::ParserState Parser::parse(std::uint8_t byte) {
    using util::ascii::codes;

    m_buffer.push_back(byte);

    m_type = EventType::NONE;

    if (m_buffer.empty()) {
        return ParserState::UNCOMPLETE;
    }

    std::uint8_t code = m_buffer.front();

    if (code == codes::ESC) {
        return parse_esc(std::span { std::next(m_buffer.begin()), m_buffer.end() });
    } else if (code < util::ascii::END_SPECIAL || code == codes::DELETE) {
        return ParserState::SPECIAL;
    }

    std::size_t tmp;
    return parse_utf8(m_buffer, tmp);
}

Parser::ParserState Parser::parse_utf8(span_t content, std::size_t& out_bytes) {
    if (content.empty()) {
        return ParserState::UNCOMPLETE;
    }

    const auto first_byte = content.front();

    constexpr std::uint8_t ASCII_MASK  = 0b1000'0000;
    constexpr std::uint8_t ASCII_VALUE = 0;

    constexpr std::uint8_t UTF8_2BYTE_MASK  = 0b1110'0000;
    constexpr std::uint8_t UTF8_2BYTE_VALUE = 0b1100'0000;

    constexpr std::uint8_t UTF8_3BYTE_MASK  = 0b1111'0000;
    constexpr std::uint8_t UTF8_3BYTE_VALUE = 0b1110'0000;

    constexpr std::uint8_t UTF8_4BYTE_MASK  = 0b1111'1000;
    constexpr std::uint8_t UTF8_4BYTE_VALUE = 0b1111'0000;

    constexpr std::uint8_t UTF8_OTHER_BYTE_MASK  = 0b1100'0000;
    constexpr std::uint8_t UTF8_OTHER_BYTE_VALUE = 0b1000'0000;

    std::size_t check_bytes;
    out_bytes = 1;

    if ((first_byte & ASCII_MASK) == ASCII_VALUE) [[likely]] {
        return ParserState::CHARACTER;
    } else if ((first_byte & UTF8_2BYTE_MASK) == UTF8_2BYTE_VALUE) {
        check_bytes = 1;
    } else if ((first_byte & UTF8_3BYTE_MASK) == UTF8_3BYTE_VALUE) {
        check_bytes = 2;
    } else if ((first_byte & UTF8_4BYTE_MASK) == UTF8_4BYTE_VALUE) {
        check_bytes = 3;
    }
    // not valid utf-8
    else {
        return ParserState::UNKNOWN;
    }

    out_bytes += check_bytes;

    if (content.size() < check_bytes + 1) {
        return ParserState::UNCOMPLETE;
    }

    for (std::size_t i = 0; i < check_bytes; i++) {
        if ((content[i + 1] & UTF8_OTHER_BYTE_MASK) != UTF8_OTHER_BYTE_VALUE) {
            return ParserState::UNKNOWN;
        }
    }

    return ParserState::CHARACTER;
}

Parser::ParserState Parser::parse_esc(span_t content) {
    if (content.empty()) {
        return ParserState::UNCOMPLETE;
    }

    const std::uint8_t byte = content.front();
    ParserState state;

    switch (byte) {
    case '[':
        state = parse_csi(content.subspan(1));
        break;
    case 'O':
        state = parse_key_ss3(content.subspan(1));
        break;
    default:
        state = ParserState::UNKNOWN;
        break;
    }

    return state;
}

Parser::ParserState Parser::parse_csi(span_t content) {
    if (content.empty()) {
        return ParserState::UNCOMPLETE;
    }

    const auto byte = content.front();

    switch (byte) {
    // Tracking
    case 'M':
        return parse_tracking(content.subspan(1));
    default:
        return parse_key_csi(content);
    }
}

Parser::ParserState Parser::parse_value(span_t& content, span_t& out) {
    std::size_t size;
    ParserState state = parse_utf8(content, size);
    if (state != ParserState::CHARACTER) {
        return state;
    }

    out     = content.subspan(0, size);
    content = content.subspan(size);

    return ParserState::CHARACTER;
}

Parser::ParserState Parser::parse_tracking(span_t content) {

    if (content.empty()) {
        return ParserState::UNCOMPLETE;
    }

    std::size_t cb_size;
    std::size_t cx_size;
    std::size_t cy_size;

    ParserState state = parse_utf8(content, cb_size);
    if (state != ParserState::CHARACTER || cb_size != 1) {
        return state;
    }

    state = parse_utf8(content.subspan(cb_size), cx_size);
    if (state != ParserState::CHARACTER) {
        return state;
    }

    state = parse_utf8(content.subspan(cb_size + cx_size), cy_size);
    if (state != ParserState::CHARACTER) {
        return state;
    }

    m_type       = EventType::MOUSE;
    m_mouse.code = content.front();

    m_mouse.mouse.x = span_to_value(content.subspan(cb_size, cx_size));
    m_mouse.mouse.y = span_to_value(content.subspan(cb_size + cx_size, cy_size));

    return ParserState::EVENT;
}

Parser::ParserState Parser::parse_key_csi(span_t content) {
    using koterm::event::KeyCode;

    if (content.empty()) {
        return ParserState::UNCOMPLETE;
    }

    const std::uint8_t first_byte = content.front();
    switch (first_byte) {
    case 'A':
        m_type = EventType::KEY;
        m_key  = KeyCode::ARROW_UP;
        return ParserState::EVENT;
    case 'B':
        m_type = EventType::KEY;
        m_key  = KeyCode::ARROW_DOWN;
        return ParserState::EVENT;
    case 'C':
        m_type = EventType::KEY;
        m_key  = KeyCode::ARROW_RIGHT;
        return ParserState::EVENT;
    case 'D':
        m_type = EventType::KEY;
        m_key  = KeyCode::ARROW_LEFT;
        return ParserState::EVENT;
    case 'H':
        m_type = EventType::KEY;
        m_key  = KeyCode::HOME;
        return ParserState::EVENT;
    case 'F':
        m_type = EventType::KEY;
        m_key  = KeyCode::END;
        return ParserState::EVENT;

    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
        break;
    default:
        return ParserState::UNKNOWN;
    }

    span_t subcontent = content.subspan(1);
    if (subcontent.empty()) {
        return ParserState::UNCOMPLETE;
    }

    // NOTE: byte = '1' or '2' or '3' or '4' or '5' or '6'
    const std::uint8_t second_byte = subcontent.front();
    std::uint8_t third_byte
        = ((subcontent.size() == 2) ? subcontent[1] : static_cast<std::uint8_t>(util::ascii::INVALID));

    // ESC [ x ~
    if (second_byte == '~') {

        constexpr auto LOOKUP = util::to_map(std::to_array<std::pair<std::uint8_t, KeyCode>>({
            {'1',      KeyCode::HOME},
            {'2',    KeyCode::INSERT},
            {'3',    KeyCode::DELETE},
            {'4',       KeyCode::END},
            {'5',   KeyCode::PAGE_UP},
            {'6', KeyCode::PAGE_DOWN},
        }));

        m_type = EventType::KEY;
        m_key  = LOOKUP.at(first_byte);
        return ParserState::EVENT;
    }
    // ESC [ 1 x ...
    else if (first_byte == '1') {
        switch (second_byte) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '7':
        case '8':
        case '9':
            break;
        default:
            return parse_cursor(content);
        }

        if (third_byte == util::ascii::INVALID) {
            return ParserState::UNCOMPLETE;
        } else if (third_byte != '~') {
            return parse_cursor(content);
        }

        // ESC [ 1 x ~
        constexpr auto LOOKUP = util::to_map(std::to_array<std::pair<std::uint8_t, KeyCode>>({
            {'1', KeyCode::F1},
            {'2', KeyCode::F2},
            {'3', KeyCode::F3},
            {'4', KeyCode::F4},
            {'5', KeyCode::F5},
            {'7', KeyCode::F6},
            {'8', KeyCode::F7},
            {'9', KeyCode::F8},
        }));

        m_type = EventType::KEY;
        m_key  = LOOKUP.at(second_byte);
        return ParserState::EVENT;
    }
    // ESC [ 2 x ...
    else if (first_byte == '2') {
        switch (second_byte) {
        case '0':
        case '1':
        case '3':
        case '4':
            break;
        default:
            return parse_cursor(content);
        }

        if (third_byte == util::ascii::INVALID) {
            return ParserState::UNCOMPLETE;
        } else if (third_byte != '~') {
            return parse_cursor(content);
        }

        // ESC [ 2 x ~

        constexpr auto LOOKUP = util::to_map(std::to_array<std::pair<std::uint8_t, KeyCode>>({
            {'0',  KeyCode::F9},
            {'1', KeyCode::F10},
            {'3', KeyCode::F11},
            {'4', KeyCode::F12},
        }));

        m_type = EventType::KEY;
        m_key  = LOOKUP.at(second_byte);
        return ParserState::EVENT;
    }

    return parse_cursor(content);
}

Parser::ParserState Parser::parse_key_ss3(span_t content) {
    using koterm::event::KeyCode;

    if (content.empty()) {
        return ParserState::UNCOMPLETE;
    }

    const std::uint8_t byte = content.front();
    switch (byte) {
    case 'P':
        m_type = EventType::KEY;
        m_key  = KeyCode::F1;
        return ParserState::EVENT;
    case 'Q':
        m_type = EventType::KEY;
        m_key  = KeyCode::F2;
        return ParserState::EVENT;
    case 'R':
        m_type = EventType::KEY;
        m_key  = KeyCode::F3;
        return ParserState::EVENT;
    case 'S':
        m_type = EventType::KEY;
        m_key  = KeyCode::F4;
        return ParserState::EVENT;

    default:
        return ParserState::UNKNOWN;
    }
}

Parser::ParserState find_until(std::uint8_t search, Parser::span_t& content, Parser::span_t& out) {

    if (content.empty()) {
        return Parser::ParserState::UNCOMPLETE;
    }

    std::size_t offset = 0;

    while (content[offset] != search) {
        const std::uint8_t byte = content[offset];
        if ('0' > byte || byte > '9') {
            return Parser::ParserState::UNKNOWN;
        }

        offset += 1;
        if (offset >= content.size()) {
            return Parser::ParserState::UNCOMPLETE;
        }
    }

    out     = content.subspan(0, offset);
    content = content.subspan(offset);

    return Parser::ParserState::CHARACTER;
}

Parser::ParserState Parser::parse_cursor(span_t content) {
    span_t row;
    span_t column;

    ParserState state = find_until(';', content, row);
    if (state != ParserState::CHARACTER) {
        return state;
    }

    if (content.empty()) {
        return ParserState::UNCOMPLETE;
    }

    content = content.subspan(1);

    state = find_until('R', content, column);
    if (state != ParserState::CHARACTER) {
        return state;
    }

    std::string_view row_ref { reinterpret_cast<char*>(row.data()), row.size() };
    std::string_view column_ref { reinterpret_cast<char*>(column.data()), column.size() };

    int row_value;
    int column_value;

    auto [row_end, row_err]       = std::from_chars(row_ref.begin(), row_ref.end(), row_value);
    auto [column_end, column_err] = std::from_chars(column_ref.begin(), column_ref.end(), column_value);

    if (row_err != std::errc() || row_end != row_ref.end() || column_err != std::errc()
        || column_end != column_ref.end()) {

        return ParserState::UNKNOWN;
    }

    m_type     = EventType::CURSOR;
    m_cursor.x = column_value;
    m_cursor.y = row_value;

    return ParserState::EVENT;
}

}
