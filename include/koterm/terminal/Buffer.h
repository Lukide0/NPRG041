#ifndef KOTERM_TERMINAL_BUFFER_H
#define KOTERM_TERMINAL_BUFFER_H

#include "Pixel.h"
#include "koterm/terminal/BoundingBox.h"
#include "koterm/terminal/Tile.h"
#include "unit.h"
#include <vector>

namespace koterm::terminal {

/**
 * @brief Represents a buffer for storing terminal graphics.
 */
class Buffer {
public:
    /**
     * @brief Constructs a buffer with the specified width and height.
     *
     * @param width The width of the buffer.
     * @param height The height of the buffer.
     */
    Buffer(unit_t width, unit_t height);

    /**
     * @brief Resizes the buffer to the specified width and height.
     *
     * @param width The new width of the buffer.
     * @param height The new height of the buffer.
     */
    void resize(unit_t width, unit_t height);

    /**
     * @brief Sets the rendering view of the buffer.
     *
     * @param offset_x The x-coordinate offset of the rendering view.
     * @param offset_y The y-coordinate offset of the rendering view.
     * @param width The width of the rendering view.
     * @param height The height of the rendering view.
     */
    void set_render_view(unit_t offset_x, unit_t offset_y, unit_t width, unit_t height);

    /**
     * @brief Sets the rendering view of the buffer using a bounding box.
     *
     * @param box The bounding box specifying the rendering view.
     */
    void set_render_view(BoundingBox box);

    /**
     * @brief Flushes the buffer, rendering its content.
     */
    void flush() const;

    /**
     * @brief Clears the entire buffer.
     */
    void clear() { clear_rect({ m_width - 1, m_height - 1 }); }

    /**
     * @brief Clears a rectangular region of the buffer.
     *
     * @param rect The bounding box specifying the region to clear.
     */
    void clear_rect(BoundingBox rect);

    /**
     * @brief Sets the style of a pixel in the buffer.
     *
     * @param style The style to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_style(PixelStyle style, unit_t x, unit_t y);

    /**
     * @brief Sets the background color of a pixel in the buffer.
     *
     * @param background The background color to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_background(Color::color_id background, unit_t x, unit_t y);

    /**
     * @brief Sets the foreground color of a pixel in the buffer.
     *
     * @param foreground The foreground color to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_foreground(Color::color_id foreground, unit_t x, unit_t y);

    /**
     * @brief Sets both background and foreground colors of a pixel in the buffer.
     *
     * @param background The background color to set.
     * @param foreground The foreground color to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_color(Color::color_id background, Color::color_id foreground, unit_t x, unit_t y);

    /**
     * @brief Sets the color of a pixel in the buffer.
     *
     * @param color The color to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_color(PixelColor color, unit_t x, unit_t y) { set_pixel_color(color.bg, color.fg, x, y); }

    /**
     * @brief Sets the content of a pixel in the buffer.
     *
     * @param content The content to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_content(PixelContent content, unit_t x, unit_t y);

    /**
     * @brief Sets the content of a pixel in the buffer using a tile encoding.
     *
     * @param tile The tile encoding to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_content(TileEncoding tile, unit_t x, unit_t y);

    /**
     * @brief Combines a tile with the content of a pixel in the buffer.
     *
     * @param tile The tile encoding to combine.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void combine_tile(TileEncoding tile, unit_t x, unit_t y);

    /**
     * @brief Gets the width of the buffer.
     *
     * @return The width of the buffer.
     */
    [[nodiscard]] unit_t width() const { return m_width; }

    /**
     * @brief Gets the height of the buffer.
     *
     * @return The height of the buffer.
     */
    [[nodiscard]] unit_t height() const { return m_height; }

    /**
     * @brief Gets the style of a pixel in the buffer.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The style of the pixel.
     */
    [[nodiscard]] PixelStyle get_pixel_style(unit_t x, unit_t y) const;

    /**
     * @brief Gets the content of a pixel in the buffer.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The content of the pixel.
     */
    [[nodiscard]] PixelContent get_pixel_content(unit_t x, unit_t y) const;

    /**
     * @brief Gets the color of a pixel in the buffer.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The color of the pixel.
     */
    [[nodiscard]] PixelColor get_pixel_color(unit_t x, unit_t y) const;

private:
    unit_t m_width;
    unit_t m_height;
    BoundingBox m_box;
    BoundingBox m_render_box;

    std::vector<PixelStyle> m_pixels_style;
    std::vector<PixelContent> m_pixels_content;
    std::vector<PixelColor> m_pixels_color;
    std::vector<TileEncoding> m_tiles;

    [[nodiscard]] std::size_t to_index(unit_t x, unit_t y) const { return x + y * m_width; }
};

}

#endif
