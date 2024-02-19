#include "koterm/screen/Screen.h"
#include "koterm/terminal/Cursor.h"
#include "koterm/terminal/terminal.h"
#include "koterm/unit.h"
#include <algorithm>
#include <chrono>
#include <memory>

namespace koterm::screen {

std::unique_ptr<Screen> Screen::create(unit_t width, unit_t height) {
    if (!terminal::has_initialized()) {
        return nullptr;
    }

    return std::unique_ptr<Screen>(new Screen(width, height));
}

void Screen::render() {
    terminal::Cursor::move(0, 0);
    m_dom.render();
    m_buffer.flush();
}

}
