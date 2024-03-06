#ifndef KOTERM_COMPONENT_BORDER_H
#define KOTERM_COMPONENT_BORDER_H

#include "koterm/terminal/Buffer.h"
#include "koterm/terminal/Color.h"
#include "koterm/terminal/colors.h"
namespace koterm::component {

class Border {
public:
    using color_t = terminal::Color;

    enum class Style {
        NONE   = 0,
        THIN   = 1,
        BOLD   = 2,
        DOUBLE = 3,
    };
    /**
     * @brief Constructs a Border object with the given style and color.
     * @param style The border style.
     * @param color The border color.
     */
    Border(Style style = Style::NONE, color_t color = terminal::colors::SYS_DEFAULT)
        : m_style(style)
        , m_color(color) { }

    /**
     * @brief Sets the border style.
     * @param style The border style to set.
     */
    void set_style(Style style) { m_style = style; }

    /**
     * @brief Returns the current border style.
     * @return The current border style.
     */
    [[nodiscard]] Style style() const { return m_style; }

    /**
     * @brief Checks if the border has a style.
     * @return True if the border has a style, false otherwise.
     */
    [[nodiscard]] bool has_style() const { return m_style != Style::NONE; }

    /**
     * @brief Sets the border color.
     * @param color The color to set.
     */
    void set_color(color_t color) { m_color = color; }

    /**
     * @brief Returns the current border color.
     * @return The current border color.
     */
    [[nodiscard]] color_t color() const { return m_color; }

    /**
     * @brief Renders the border onto the specified buffer.
     * @param buffer The buffer to render onto.
     * @details The border has the same size as the buffer.
     */
    void render(terminal::BufferSpan& buffer);

private:
    Style m_style;
    color_t m_color;
};

}

#endif
