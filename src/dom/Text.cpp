#include "koterm/dom/Text.h"
#include "koterm/dom/Element.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/terminal/Color.h"
#include "koterm/terminal/colors.h"
#include <memory>

namespace koterm::dom {

std::shared_ptr<Text> Text::create(const terminal::BufferSpan& buffer, DomManager* manager) {
    return std::make_shared<Text>(buffer, manager, false, true, true);
}

void Text::prepare_buffer() {
    m_buffer.clear();

    const auto& pallete = get_pallete();

    terminal::Color fg = (m_foreground == terminal::colors::SYS_DEFAULT) ? pallete.foreground : m_foreground;
    terminal::Color bg = (m_background == terminal::colors::SYS_DEFAULT) ? pallete.background : m_background;

    m_text.render(m_buffer, fg, bg);
}
}
