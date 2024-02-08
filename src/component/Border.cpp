

#include "koterm/component/Border.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/terminal/Tile.h"
#include "koterm/unit.h"
#include <cstdint>
namespace koterm::component {

void Border::render(terminal::BufferSpan& buffer) {

    unit_t width  = buffer.width();
    unit_t height = buffer.height();

    auto style = static_cast<std::uint8_t>(m_style);

    terminal::TileEncoding horizontal { 0, style, 0, style };
    terminal::TileEncoding vertical { style, 0, style, 0 };

    for (unit_t x = 0 + 1; x < width; x++) {
        buffer.set_pixel_content(horizontal, x, 0);
        buffer.set_pixel_content(horizontal, x, height);

        buffer.set_pixel_foreground(m_color, x, 0);
        buffer.set_pixel_foreground(m_color, x, height);
    }

    for (unit_t y = 0 + 1; y < height; y++) {
        buffer.set_pixel_content(vertical, 0, y);
        buffer.set_pixel_content(vertical, width, y);

        buffer.set_pixel_foreground(m_color, 0, y);
        buffer.set_pixel_foreground(m_color, width, y);
    }

    buffer.set_pixel_content({ 0, style, style, 0 }, 0, 0);
    buffer.set_pixel_foreground(m_color, 0, 0);

    buffer.set_pixel_content({ 0, 0, style, style }, width, 0);
    buffer.set_pixel_foreground(m_color, width, 0);

    buffer.set_pixel_content({ style, style, 0, 0 }, 0, height);
    buffer.set_pixel_foreground(m_color, 0, height);
    buffer.set_pixel_content({ style, 0, 0, style }, width, height);
    buffer.set_pixel_foreground(m_color, width, height);
}

}
