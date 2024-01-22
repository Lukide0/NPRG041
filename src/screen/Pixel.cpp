#include "koterm/screen/Pixel.h"
#include "koterm/screen/ansi.h"

namespace koterm::screen {

void PixelStyle::write(std::ostream& stream, PixelStyle previous) const {
    if (previous == *this) {
        return;
    }

    const bool bold          = is_set<BOLD>();
    const bool dim           = is_set<DIM>();
    const bool italic        = is_set<ITALIC>();
    const bool underline     = is_set<UNDERLINE>();
    const bool blink         = is_set<BLINK>();
    const bool inverse       = is_set<INVERSE>();
    const bool hidden        = is_set<HIDDEN>();
    const bool strikethrough = is_set<STRIKETHROUGH>();

    if (bold != previous.is_set<BOLD>()) {
        stream << (bold ? ansi::STYLE_BOLD : ansi::STYLE_BOLD_RESET);
    }

    if (dim != previous.is_set<DIM>()) {
        stream << (dim ? ansi::STYLE_DIM : ansi::STYLE_DIM_RESET);
    }

    if (italic != previous.is_set<ITALIC>()) {
        stream << (italic ? ansi::STYLE_ITALIC : ansi::STYLE_ITALIC_RESET);
    }

    if (underline != previous.is_set<UNDERLINE>()) {
        stream << (underline ? ansi::STYLE_UNDERLINE : ansi::STYLE_UNDERLINE_RESET);
    }

    if (blink != previous.is_set<BLINK>()) {
        stream << (blink ? ansi::STYLE_BLINKING : ansi::STYLE_BLINKING_RESET);
    }

    if (inverse != previous.is_set<INVERSE>()) {
        stream << (inverse ? ansi::STYLE_INVERSE : ansi::STYLE_INVERSE_RESET);
    }

    if (hidden != previous.is_set<HIDDEN>()) {
        stream << (hidden ? ansi::STYLE_HIDDEN : ansi::STYLE_HIDDEN_RESET);
    }

    if (strikethrough != previous.is_set<STRIKETHROUGH>()) {
        stream << (strikethrough ? ansi::STYLE_STRIKETHROUGH : ansi::STYLE_STRIKETHROUGH_RESET);
    }
}

}
