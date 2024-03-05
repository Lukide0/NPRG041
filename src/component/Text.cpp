#include "koterm/component/Text.h"
#include "koterm/component/Alignment.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/terminal/Color.h"
#include "koterm/unit.h"
#include <algorithm>
#include <cstddef>

namespace koterm::component {

void Text::render(terminal::BufferSpan& buffer, terminal::Color foreground, terminal::Color background) {

    const auto width  = buffer.width();
    const auto height = buffer.height();

    const auto text_size = static_cast<unit_t>(m_text.size());

    if (text_size == 0) {
        return;
    }

    unit_t y = 0;
    switch (m_align.vertical()) {
    case Alignment::Vertical::TOP:
        break;
    case Alignment::Vertical::CENTER:
        y = height / 2;
        break;
    case Alignment::Vertical::BOTTOM:
        y = height;
        break;
    }

    unit_t start_x = 0;
    switch (m_align.horizontal()) {
    case Alignment::Horizontal::LEFT:
        break;
    case Alignment::Horizontal::CENTER:
        if (width > text_size) {
            start_x = (width - text_size) / 2;
        }
        break;
    case Alignment::Horizontal::RIGHT:
        if (width > text_size) {
            start_x = width - text_size;
        }
        break;
    }
    unit_t end_x = std::min(width, text_size) + start_x;

    std::size_t index = 0;
    for (unit_t x = start_x; x < end_x; x++, index++) {
        buffer.set_pixel_content(m_text[index], x, y);
        buffer.set_pixel_color(background.id(), foreground.id(), x, y);
        buffer.set_pixel_style(m_decoration.style(), x, y);
    }
}

}
