#include "koterm/screen/Buffer.h"
#include "koterm/screen/ansi.h"
#include "koterm/screen/colors.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>

namespace koterm::screen {

template <bool IS_BACKGROUND> void write_color(const Color& color, std::ostream& stream);

Buffer::Buffer(unit_t width, unit_t height)
    : m_width(width)
    , m_height(height)
    , m_box(width - 1, height - 1) {
    assert(width != 0 && height != 0);

    const std::size_t size = static_cast<std::size_t>(width) * height;

    m_pixels_style.reserve(size);
    m_pixels_content.reserve(size);
    m_pixels_color.reserve(size);
    m_tiles.reserve(size);

    for (std::size_t i = 0; i < size; i++) {
        m_pixels_style.emplace_back();
        m_pixels_content.emplace_back();
        m_pixels_color.emplace_back(colors::SYS_DEFAULT.id(), colors::SYS_DEFAULT.id());
        m_tiles.emplace_back(0, 0, 0, 0);
    }
}

void Buffer::resize(unit_t width, unit_t height) {
    assert(width != 0 && height != 0);

    const std::size_t new_size = static_cast<std::size_t>(width) * height;

    clear();

    m_pixels_style.resize(new_size);
    m_pixels_content.resize(new_size);
    m_pixels_color.resize(new_size);
    m_tiles.resize(new_size);

    m_width  = width;
    m_height = height;
    m_box    = { width - 1, height - 1 };
}

void Buffer::clear_rect(BoundingBox rect) {

    const unit_t start_y = rect.top;
    const unit_t end_y   = std::min(rect.bottom, m_height - 1);
    const unit_t start_x = rect.left;
    const unit_t end_x   = std::min(rect.right, m_width - 1);

    for (unit_t y = start_y; y <= end_y; y++) {
        const std::size_t row_index = static_cast<std::size_t>(y) * m_width;
        for (unit_t x = start_x; x <= end_x; x++) {
            const std::size_t index = row_index + x;

            m_pixels_style[index]           = {};
            m_pixels_color[index]           = { colors::SYS_DEFAULT.id(), colors::SYS_DEFAULT.id() };
            m_pixels_content[index].content = ' ';
            m_tiles[index].encoding         = TileEncoding::NONE;
        }
    }
}

void Buffer::flush(std::ostream& stream) const {

    PixelStyle prev_style;
    PixelColor prev_color = { colors::SYS_DEFAULT.id(), colors::SYS_DEFAULT.id() };

    for (unit_t y = 0; y < m_height; y++) {
        const std::size_t row_index = static_cast<std::size_t>(y) * m_width;
        for (unit_t x = 0; x < m_width; x++) {

            const std::size_t index = row_index + x;
            const auto pixel_style  = m_pixels_style.at(index);
            const auto pixel_color  = m_pixels_color.at(index);

            if (pixel_style != prev_style) {
                pixel_style.write(stream, prev_style);
            }

            if (pixel_color.bg != prev_color.bg) {
                write_color<true>(Color::from_id(pixel_color.bg));
            }

            if (pixel_color.fg != prev_color.fg) {
                write_color<false>(Color::from_id(pixel_color.fg));
            }

            stream << m_pixels_content.at(index).content;
            prev_style = pixel_style;
            prev_color = pixel_color;
        }

        stream << "\r\n";
    }

    stream.flush();
}

void Buffer::set_pixel_style(PixelStyle style, unit_t x, unit_t y) {
    assert(m_box.contains(x, y));

    std::size_t index     = to_index(x, y);
    m_pixels_style[index] = style;
}

void Buffer::set_pixel_background(Color::color_id background, unit_t x, unit_t y) {
    assert(m_box.contains(x, y));

    std::size_t index        = to_index(x, y);
    m_pixels_color[index].bg = background;
}

void Buffer::set_pixel_foreground(Color::color_id foreground, unit_t x, unit_t y) {
    assert(m_box.contains(x, y));

    std::size_t index        = to_index(x, y);
    m_pixels_color[index].fg = foreground;
}

void Buffer::set_pixel_color(Color::color_id background, Color::color_id foreground, unit_t x, unit_t y) {
    assert(m_box.contains(x, y));

    std::size_t index     = to_index(x, y);
    m_pixels_color[index] = { .bg = background, .fg = foreground };
}

void Buffer::set_pixel_content(PixelContent content, unit_t x, unit_t y) {
    assert(m_box.contains(x, y));

    std::size_t index       = to_index(x, y);
    m_pixels_content[index] = content;
    m_tiles[index].encoding = TileEncoding::NONE;
}

void Buffer::set_pixel_content(TileEncoding tile, unit_t x, unit_t y) {
    assert(m_box.contains(x, y));

    std::size_t index       = to_index(x, y);
    m_tiles[index]          = tile;
    m_pixels_content[index] = TILES_MAP.at(tile);
}

void Buffer::combine_tile(TileEncoding tile, unit_t x, unit_t y) {
    assert(m_box.contains(x, y));

    std::size_t index       = to_index(x, y);
    auto& buffer_tile       = m_tiles[index];
    buffer_tile             = buffer_tile.combine(tile);
    m_pixels_content[index] = TILES_MAP.at(buffer_tile);
}

template <bool IS_BACKGROUND> void write_color(const Color& color, std::ostream& stream) {
    switch (color.type()) {
    case Color::Type::RGB:
        stream << ansi::CSI << ((IS_BACKGROUND) ? "48;2;" : "38;2;") << color.red() << ';' << color.green() << ';'
               << color.blue() << 'm';
        break;
    case Color::Type::SYSTEM:
        stream << ansi::CSI << ((IS_BACKGROUND) ? color.code() + 10 : color.code()) << 'm';
        break;
    }
}
}
