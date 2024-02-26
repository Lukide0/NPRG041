#ifndef KOTERM_DOM_TEXT_H
#define KOTERM_DOM_TEXT_H

#include "koterm/component/Alignment.h"
#include "koterm/component/Text.h"
#include "koterm/component/TextDecoration.h"
#include "koterm/dom/Element.h"
#include "koterm/screen/BaseScreen.h"
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

    static std::shared_ptr<Text> create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer(), screen->viewport() }, &screen->dom_manager());
    }
    static std::shared_ptr<Text> create(const BufferSpan& buffer, DomManager* manager);

    ~Text() override = default;

    void prepare_buffer() override;

    [[nodiscard]] Alignment aligment() const { return m_text.alignment(); }
    Alignment& modify_alignment() {
        request_update();
        return m_text.modify_alignment();
    }

    [[nodiscard]] TextDecoration decoration() const { return m_text.decoration(); }
    TextDecoration& modify_decoration() {
        request_render();
        return m_text.modify_decoration();
    }

    [[nodiscard]] const std::string& text() const { return m_text.text(); }

    void set_text(std::string&& text) {
        m_text.set_text(std::move(text));
        request_update();
    }
    void set_text(const std::string& text) {
        m_text.set_text(text);
        request_update();
    }

    void set_foreground(terminal::Color color) {
        request_render();
        m_foreground = color;
    }
    void set_background(terminal::Color color) {
        request_render();
        m_background = color;
    }

    void calculate_requirements() override {
        m_info.min_width  = static_cast<unit_t>(m_text.size());
        m_info.min_height = 1;
    }

private:
    using Element::Element;

    terminal::Color m_foreground = terminal::colors::SYS_DEFAULT;
    terminal::Color m_background = terminal::colors::SYS_DEFAULT;
    component::Text m_text;
};

}

#endif
