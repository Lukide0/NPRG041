#include "koterm/dom/Text.h"
#include "koterm/component/Alignment.h"
#include "koterm/dom/Element.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/unit.h"
#include <algorithm>
#include <cstddef>
#include <memory>

namespace koterm::dom {

std::shared_ptr<Text> Text::create(const terminal::BufferSpan& buffer, DomManager* manager) {
    return std::make_shared<Text>(buffer, manager);
}

void Text::prepare_buffer() {

    const auto width  = this->width();
    const auto height = this->height();

    const unit_t text_size = m_text.size();

    if (text_size == 0) {
        return;
    }

    unit_t y;
    switch (m_aligment.vertical()) {
    case Alignment::Vertical::TOP:
        y = 0;
        break;
    case Alignment::Vertical::CENTER:
        y = height / 2;
        break;
    case Alignment::Vertical::BOTTOM:
        y = height;
        break;
    }

    unit_t start_x;
    switch (m_aligment.horizontal()) {
    case Alignment::Horizontal::LEFT:
        start_x = 0;
        break;
    case Alignment::Horizontal::CENTER:
        if (width <= text_size) {
            start_x = 0;
        } else {
            start_x = (width - text_size) / 2;
        }
        break;
    case Alignment::Horizontal::RIGHT:
        if (width <= text_size) {
            start_x = 0;
        } else {
            start_x = width - text_size;
        }
        break;
    }
    unit_t end_x = std::min(width, text_size) + start_x;

    std::size_t index = 0;
    for (unit_t x = start_x; x < end_x; x++, index++) {
        m_buffer.set_pixel_content(m_text[index], x, y);
        m_buffer.set_pixel_color(m_background.id(), m_foreground.id(), x, y);
        m_buffer.set_pixel_style(m_decoration.style(), x, y);
    }
}
}
