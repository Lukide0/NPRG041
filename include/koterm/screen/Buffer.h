#ifndef KOTERM_SCREEN_BUFFER_H
#define KOTERM_SCREEN_BUFFER_H

#include "Pixel.h"
#include "koterm/screen/BoundingBox.h"
#include "koterm/screen/Tile.h"
#include "koterm/screen/colors.h"
#include "unit.h"
#include <vector>

namespace koterm::screen {

class Buffer {
public:
    Buffer(unit_t width, unit_t height);
    void resize(unit_t width, unit_t height);

    void flush(std::ostream& stream) const;

    void clear() { clear_rect({ m_width - 1, m_height - 1 }); }
    void clear_rect(BoundingBox rect);

    void set_pixel_style(PixelStyle style, unit_t x, unit_t y);
    void set_pixel_background(Color::color_id background, unit_t x, unit_t y);
    void set_pixel_foreground(Color::color_id foreground, unit_t x, unit_t y);
    void set_pixel_color(Color::color_id background, Color::color_id foreground, unit_t x, unit_t y);

    void set_pixel_content(PixelContent content, unit_t x, unit_t y);
    void set_pixel_content(TileEncoding tile, unit_t x, unit_t y);

    void combine_tile(TileEncoding tile, unit_t x, unit_t y);

    [[nodiscard]] PixelStyle get_pixel_style(unit_t x, unit_t y) const;
    [[nodiscard]] PixelContent get_pixel_content(unit_t x, unit_t y) const;
    [[nodiscard]] PixelColor get_pixel_color(unit_t x, unit_t y) const;

private:
    unit_t m_width;
    unit_t m_height;
    BoundingBox m_box;

    std::vector<PixelStyle> m_pixels_style;
    std::vector<PixelContent> m_pixels_content;
    std::vector<PixelColor> m_pixels_color;
    std::vector<TileEncoding> m_tiles;

    [[nodiscard]] std::size_t to_index(unit_t x, unit_t y) const { return x + y * m_width; }
};

}

#endif
