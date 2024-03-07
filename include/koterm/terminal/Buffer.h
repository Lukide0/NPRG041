#ifndef KOTERM_TERMINAL_BUFFER_H
#define KOTERM_TERMINAL_BUFFER_H

#include "Pixel.h"
#include "koterm/BoundingBox.h"
#include "koterm/exceptions.h"
#include "koterm/terminal/Color.h"
#include "koterm/terminal/Tile.h"
#include "koterm/terminal/colors.h"
#include "koterm/unit.h"
#include "koterm/util/debug.h"
#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>
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
    void set_pixel_content(const PixelContent& content, unit_t x, unit_t y);

    /**
     * @brief Sets the content of a pixel in the buffer using a tile encoding.
     *
     * @param tile The tile encoding to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_content(TileEncoding tile, unit_t x, unit_t y);

    /**
     * @brief Sets the background color of the buffer.
     *
     * @param color The color to set as the background.
     */
    void set_background(Color color) { m_bg = color.id(); }

    /**
     * @brief Sets the foreground color of the buffer.
     *
     * @param color The color to set as the foreground.
     */
    void set_foreground(Color color) { m_fg = color.id(); }

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
     * @brief Retrieves the bounding box of the buffer, representing its dimensions.
     *
     * @return The bounding box of the buffer.
     */
    [[nodiscard]] const BoundingBox& box() const { return m_box; }

    /**
     * @brief Retrieves the bounding box of the buffer's render area, considering any viewport or clipping.
     *
     * @return The bounding box of the buffer's render area.
     */
    [[nodiscard]] const BoundingBox& render_box() const { return m_render_box; }

    /**
     * @brief Gets the style of a pixel in the buffer.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The style of the pixel.
     */
    [[nodiscard]] PixelStyle get_pixel_style(unit_t x, unit_t y) const { return m_pixels_style[to_index(x, y)]; }

    /**
     * @brief Gets the content of a pixel in the buffer.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The content of the pixel.
     */
    [[nodiscard]] PixelContent get_pixel_content(unit_t x, unit_t y) const { return m_pixels_content[to_index(x, y)]; }

    /**
     * @brief Gets the color of a pixel in the buffer.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The color of the pixel.
     */
    [[nodiscard]] PixelColor get_pixel_color(unit_t x, unit_t y) const { return m_pixels_color[to_index(x, y)]; }

private:
    unit_t m_width;
    unit_t m_height;
    BoundingBox m_box;
    BoundingBox m_render_box;
    Color::color_id m_fg = colors::SYS_DEFAULT.id();
    Color::color_id m_bg = colors::SYS_DEFAULT.id();

    std::vector<PixelStyle> m_pixels_style;
    std::vector<PixelContent> m_pixels_content;
    std::vector<PixelColor> m_pixels_color;
    std::vector<TileEncoding> m_tiles;

    [[nodiscard]] std::size_t to_index(unit_t x, unit_t y) const { return x + y * m_width; }
};

template <bool IS_CONST> class BufferSpanBase {
private:
    static constexpr bool is_mutable = !IS_CONST;

public:
    using buffer_t = std::conditional_t<IS_CONST, const Buffer, Buffer>;

    BufferSpanBase(buffer_t& buffer)
        : m_buffer(buffer)
        , m_box(buffer.box()) { }

    BufferSpanBase(buffer_t& buffer, const BoundingBox& box)
        : m_buffer(buffer)
        , m_box(box) { }

    BufferSpanBase(const BufferSpanBase& span)
        : m_buffer(span.m_buffer)
        , m_box(span.m_box) { }

    BufferSpanBase(BufferSpanBase&& span)
        : m_buffer(std::move(span.m_buffer))
        , m_box(std::move(span.m_box)) { }

    BufferSpanBase& operator=(const BufferSpanBase& span) {
        m_box    = span.m_box;
        m_buffer = span.m_buffer;
        return *this;
    }

    template <bool CONSTANT>
    BufferSpanBase& operator=(BufferSpanBase<CONSTANT>&& span)
        requires(!is_mutable || BufferSpanBase<CONSTANT>::is_mutable)
    {
        m_box    = std::move(span.m_box);
        m_buffer = std::move(span.m_buffer);
        return *this;
    }

    operator BufferSpanBase<true>() { return BufferSpanBase<true> { m_buffer, m_box }; }

    /**
     * @brief Clears the entire buffer.
     */
    void clear()
        requires(is_mutable)
    {
        if (width() == 0 || height() == 0) {
            return;
        }

        m_buffer.clear_rect(m_box);
    }

    /**
     * @brief Clears a rectangular region of the buffer.
     *
     * @param rect The bounding box specifying the region to clear.
     */
    void clear_rect(BoundingBox rect)
        requires(is_mutable)
    {
        rect.left += m_box.left;
        rect.right += m_box.left;
        rect.top += m_box.top;
        rect.top += m_box.top;

        m_buffer.clear_rect(BoundingBox::overlap(m_box, rect));
    }

    /**
     * @brief Sets the style of a pixel in the buffer.
     *
     * @param style The style to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_style(PixelStyle style, unit_t x, unit_t y)
        requires(is_mutable)
    {
        x += m_box.left;
        y += m_box.top;

        koterm_assert(
            m_box.contains(x, y),
            exception::InvalidPositionException { "Position out of bounding box" },
            "Position out of bounding box"
        );
        m_buffer.set_pixel_style(style, x, y);
    }

    /**
     * @brief Sets the background color of a pixel in the buffer.
     *
     * @param background The background color to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_background(Color::color_id background, unit_t x, unit_t y)
        requires(is_mutable)
    {
        x += m_box.left;
        y += m_box.top;
        koterm_assert(
            m_box.contains(x, y),
            exception::InvalidPositionException { "Position out of bounding box" },
            "Position out of bounding box"
        );
        m_buffer.set_pixel_background(background, x, y);
    }

    /**
     * @brief Sets the foreground color of a pixel in the buffer.
     *
     * @param foreground The foreground color to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_foreground(Color::color_id foreground, unit_t x, unit_t y)
        requires(is_mutable)
    {
        x += m_box.left;
        y += m_box.top;
        koterm_assert(
            m_box.contains(x, y),
            exception::InvalidPositionException { "Position out of bounding box" },
            "Position out of bounding box"
        );
        m_buffer.set_pixel_foreground(foreground, x, y);
    }

    /**
     * @brief Sets both background and foreground colors of a pixel in the buffer.
     *
     * @param background The background color to set.
     * @param foreground The foreground color to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_color(Color::color_id background, Color::color_id foreground, unit_t x, unit_t y)
        requires(is_mutable)
    {
        x += m_box.left;
        y += m_box.top;
        koterm_assert(
            m_box.contains(x, y),
            exception::InvalidPositionException { "Position out of bounding box" },
            "Position out of bounding box"
        );
        m_buffer.set_pixel_color(background, foreground, x, y);
    }

    /**
     * @brief Sets the color of a pixel in the buffer.
     *
     * @param color The color to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_color(PixelColor color, unit_t x, unit_t y)
        requires(is_mutable)
    {
        set_pixel_color(color.bg, color.fg, x, y);
    }

    /**
     * @brief Sets the content of a pixel in the buffer.
     *
     * @param content The content to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_content(const PixelContent& content, unit_t x, unit_t y)
        requires(is_mutable)
    {
        x += m_box.left;
        y += m_box.top;
        koterm_assert(
            m_box.contains(x, y),
            exception::InvalidPositionException { "Position out of bounding box" },
            "Position out of bounding box"
        );
        m_buffer.set_pixel_content(content, x, y);
    }

    /**
     * @brief Sets the content of a pixel in the buffer using a tile encoding.
     *
     * @param tile The tile encoding to set.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void set_pixel_content(TileEncoding tile, unit_t x, unit_t y)
        requires(is_mutable)
    {
        x += m_box.left;
        y += m_box.top;
        koterm_assert(
            m_box.contains(x, y),
            exception::InvalidPositionException { "Position out of bounding box" },
            "Position out of bounding box"
        );
        m_buffer.set_pixel_content(tile, x, y);
    }

    /**
     * @brief Combines a tile with the content of a pixel in the buffer.
     *
     * @param tile The tile encoding to combine.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     */
    void combine_tile(TileEncoding tile, unit_t x, unit_t y)
        requires(is_mutable)
    {
        x += m_box.left;
        y += m_box.top;
        koterm_assert(
            m_box.contains(x, y),
            exception::InvalidPositionException { "Position out of bounding box" },
            "Position out of bounding box"
        );
        m_buffer.combine_tile(tile, x, y);
    }

    /**
     * @brief Gets the style of a pixel in the buffer.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The style of the pixel.
     */
    [[nodiscard]] PixelStyle get_pixel_style(unit_t x, unit_t y) const { return m_buffer.get_pixel_style(x, y); }

    /**
     * @brief Gets the content of a pixel in the buffer.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The content of the pixel.
     */
    [[nodiscard]] PixelContent get_pixel_content(unit_t x, unit_t y) const { return m_buffer.get_pixel_content(x, y); }

    /**
     * @brief Gets the color of a pixel in the buffer.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The color of the pixel.
     */
    [[nodiscard]] PixelColor get_pixel_color(unit_t x, unit_t y) const { return m_buffer.get_pixel_color(x, y); }

    void resize(const BoundingBox& box) { m_box = box; }

    [[nodiscard]] const buffer_t& buffer() const { return m_buffer; }
    buffer_t& buffer()
        requires(is_mutable)
    {
        return m_buffer;
    }
    [[nodiscard]] const BoundingBox& box() const { return m_box; }

    [[nodiscard]] unit_t width() const { return m_box.width(); }
    [[nodiscard]] unit_t height() const { return m_box.height(); }

private:
    buffer_t& m_buffer;
    BoundingBox m_box;
};

using ConstBufferView = BufferSpanBase<true>;
using BufferSpan      = BufferSpanBase<false>;

}

#endif
