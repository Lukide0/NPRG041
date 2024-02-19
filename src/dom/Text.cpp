#include "koterm/dom/Text.h"
#include "koterm/dom/Element.h"
#include "koterm/terminal/Buffer.h"
#include <memory>

namespace koterm::dom {

std::shared_ptr<Text> Text::create(const terminal::BufferSpan& buffer, DomManager* manager) {
    return std::make_shared<Text>(buffer, manager);
}

void Text::prepare_buffer() {
    m_buffer.clear();
    m_text.render(m_buffer, m_foreground, m_background);
}
}
