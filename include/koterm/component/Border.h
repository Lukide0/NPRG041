#ifndef KOTERM_COMPONENT_BORDER_H
#define KOTERM_COMPONENT_BORDER_H

#include "koterm/terminal/Buffer.h"
#include "koterm/terminal/Color.h"
#include "koterm/terminal/colors.h"
namespace koterm::component {

class Border {
public:
    enum class Style {
        NONE   = 0,
        THIN   = 1,
        BOLD   = 2,
        DOUBLE = 3,
    };

    Border(Style style = Style::NONE, terminal::Color::color_id color = terminal::colors::SYS_DEFAULT.id())
        : m_style(style)
        , m_color(color) { }

    void set_style(Style style) { m_style = style; }
    [[nodiscard]] Style style() const { return m_style; }

    [[nodiscard]] bool has() const { return m_style != Style::NONE; }

    void render(terminal::BufferSpan& buffer);

private:
    Style m_style;
    terminal::Color::color_id m_color;
};

}

#endif
