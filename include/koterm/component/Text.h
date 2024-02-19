#ifndef KOTERM_COMPONENT_TEXT
#define KOTERM_COMPONENT_TEXT

#include "koterm/component/Alignment.h"
#include "koterm/component/TextDecoration.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/terminal/Color.h"
#include "koterm/terminal/colors.h"
#include <cstddef>
#include <string>
#include <utility>

namespace koterm::component {

class Text {
public:
    [[nodiscard]] Alignment alignment() const { return m_align; }
    Alignment& modify_alignment() { return m_align; }

    [[nodiscard]] const std::string& text() const { return m_text; }
    void set_text(std::string&& text) { m_text = std::move(text); }
    void set_text(const std::string& text) { m_text = text; }
    [[nodiscard]] std::size_t size() const { return m_text.size(); }

    [[nodiscard]] TextDecoration decoration() const { return m_decoration; }
    TextDecoration& modify_decoration() { return m_decoration; }

    void render(
        terminal::BufferSpan& buffer,
        terminal::Color foreground = terminal::colors::SYS_DEFAULT,
        terminal::Color background = terminal::colors::SYS_DEFAULT
    );

private:
    std::string m_text;
    Alignment m_align;
    TextDecoration m_decoration;
};

}

#endif
