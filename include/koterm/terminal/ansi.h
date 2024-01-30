#ifndef KOTERM_TERMINAL_ANSI_H
#define KOTERM_TERMINAL_ANSI_H

#include <ostream>
#include <string>
#include <string_view>

#define ESC_CODE "\x1B"
#define CSI_CODE ESC_CODE "["

namespace koterm::terminal::ansi {

constexpr std::string_view ESC = ESC_CODE;
constexpr int PREFIX_CODE      = '\x1B';
constexpr std::string_view CSI = CSI_CODE;

// Common Private Modes
constexpr std::string_view CPM = CSI_CODE "?";

enum class Features {
    AUTO_WRAP      = 7,
    CURSOR_VISIBLE = 25,
    // mouse tracking
    VT200_MOUSE = 1000,
    // mouse highlight tracking
    VT200_HIGHLIGHT_MOUSE = 1001,
    // button-event tracking
    BTN_EVENT_MOUSE = 1002,
    ANY_EVENT_MOUSE = 1003,

    EXT_MODE_MOUSE     = 1005,
    SRG_EXT_MODE_MOUSE = 1006,

    ALTERNATIVE_SCREEN = 1049,
};

constexpr std::string_view CURSOR_QUERY     = CSI_CODE "6n";
constexpr std::string_view CURSOR_TO_ORIGIN = CSI_CODE "H";
constexpr std::string_view CURSOR_SAVE      = CSI_CODE "7";
constexpr std::string_view CURSOR_RESTORE   = CSI_CODE "8";

constexpr std::string_view ERASE_FROM_CURSOR_TO_SCREEN_END      = CSI_CODE "0J";
constexpr std::string_view ERASE_TO_CURSOR                      = CSI_CODE "1J";
constexpr std::string_view ERASE_SCREEN                         = CSI_CODE "2J";
constexpr std::string_view ERASE_FROM_CURSOR_TO_LINE_END        = CSI_CODE "0K";
constexpr std::string_view ERASE_FROM_START_LINE_TO_CURSOR_LINE = CSI_CODE "1K";
constexpr std::string_view ERASE_LINE                           = CSI_CODE "2K";

inline void move_cursor(unsigned int row, unsigned int column, std::ostream& stream) {
    stream << CSI_CODE << row << ';' << column << 'H';
}

inline void move_cursor_up(unsigned int n, std::ostream& stream) { stream << CSI_CODE << n << 'A'; }
inline void move_cursor_down(unsigned int n, std::ostream& stream) { stream << CSI_CODE << n << 'B'; }
inline void move_cursor_right(unsigned int n, std::ostream& stream) { stream << CSI_CODE << n << 'C'; }
inline void move_cursor_left(unsigned int n, std::ostream& stream) { stream << CSI_CODE << n << 'D'; }

constexpr std::string_view STYLE_RESET               = CSI_CODE "0m";
constexpr std::string_view STYLE_BOLD                = CSI_CODE "1m";
constexpr std::string_view STYLE_BOLD_RESET          = CSI_CODE "22m";
constexpr std::string_view STYLE_DIM                 = CSI_CODE "2m";
constexpr std::string_view STYLE_DIM_RESET           = CSI_CODE "22m";
constexpr std::string_view STYLE_ITALIC              = CSI_CODE "3m";
constexpr std::string_view STYLE_ITALIC_RESET        = CSI_CODE "23m";
constexpr std::string_view STYLE_UNDERLINE           = CSI_CODE "4m";
constexpr std::string_view STYLE_UNDERLINE_RESET     = CSI_CODE "24m";
constexpr std::string_view STYLE_BLINKING            = CSI_CODE "5m";
constexpr std::string_view STYLE_BLINKING_RESET      = CSI_CODE "25m";
constexpr std::string_view STYLE_INVERSE             = CSI_CODE "7m";
constexpr std::string_view STYLE_INVERSE_RESET       = CSI_CODE "27m";
constexpr std::string_view STYLE_HIDDEN              = CSI_CODE "8m";
constexpr std::string_view STYLE_HIDDEN_RESET        = CSI_CODE "28m";
constexpr std::string_view STYLE_STRIKETHROUGH       = CSI_CODE "9m";
constexpr std::string_view STYLE_STRIKETHROUGH_RESET = CSI_CODE "29m";

}

#undef ESC_CODE
#undef CSI_CODE

#endif
