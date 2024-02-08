#ifndef KOTERM_DOM_TEXT_H
#define KOTERM_DOM_TEXT_H

#include "koterm/component/Alignment.h"
#include "koterm/component/TextDecoration.h"
#include "koterm/dom/Element.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/terminal/Color.h"
#include "koterm/terminal/colors.h"
#include "koterm/unit.h"
#include <memory>
#include <string>
#include <utility>

namespace koterm::dom {

class Text : public Element {
public:
    using Alignment      = component::Alignment;
    using TextDecoration = component::TextDecoration;

    static std::shared_ptr<Text> create(const terminal::BufferSpan& buffer, DomManager* manager);

    ~Text() override = default;

    void prepare_buffer() override;

    [[nodiscard]] Alignment aligment() const { return m_aligment; }
    Alignment& modify_alignment() { return m_aligment; }

    [[nodiscard]] TextDecoration decoration() const { return m_decoration; }
    TextDecoration& modify_decoration() { return m_decoration; }

    [[nodiscard]] const std::string& text() const { return m_text; }

    void set_text(std::string&& text) {
        m_text = std::move(text);
        request_update();
    }
    void set_text(const std::string& text) {
        m_text = text;
        request_update();
    }

    void set_foreground(terminal::Color color) { m_foreground = color; }
    void set_background(terminal::Color color) { m_background = color; }

    void calculate_requirements() override {
        m_info.min_width  = static_cast<unit_t>(m_text.size());
        m_info.min_height = 1;

        m_buffer.clear();
    }

private:
    using Element::Element;

    Alignment m_aligment;
    TextDecoration m_decoration;

    terminal::Color m_foreground = terminal::colors::SYS_DEFAULT;
    terminal::Color m_background = terminal::colors::SYS_DEFAULT;
    std::string m_text;
};

}

#endif
