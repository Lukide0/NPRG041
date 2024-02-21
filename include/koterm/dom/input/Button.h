#ifndef KOTERM_DOM_INPUT_BUTTON_H
#define KOTERM_DOM_INPUT_BUTTON_H

#include "koterm/component/Alignment.h"
#include "koterm/component/Border.h"
#include "koterm/component/Signal.h"
#include "koterm/component/Text.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/screen/BaseScreen.h"
#include "koterm/terminal/Color.h"
#include "koterm/terminal/colors.h"
#include <memory>

namespace koterm::dom::input {

class Button : public Element {

public:
    using button_t  = std::shared_ptr<Button>;
    using signal_t  = component::Signal<>;
    using border_t  = component::Border;
    using text_t    = component::Text;
    using Alignment = component::Alignment;
    using Color     = koterm::terminal::Color;

    static button_t create(const BufferSpan& buffer, DomManager* manager);
    static button_t create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager());
    }

    signal_t& on_press() { return m_onpress; }

    void prepare_buffer() override;
    void calculate_requirements() override;
    bool handle_key(event::KeyCode key) override;
    bool handle_mouse_click(const event::MouseEvent& event) override;

    [[nodiscard]] const text_t& text() const { return m_text; }
    text_t& modify_text() {
        request_update();
        return m_text;
    }

    [[nodiscard]] const border_t& border() const { return m_border; }
    border_t& modify_border() {
        request_update();
        return m_border;
    }

    [[nodiscard]] Color background() const { return m_bg; }
    [[nodiscard]] Color foreground() const { return m_fg; }

    void set_background(Color color) { m_bg = color; }
    void set_foreground(Color color) { m_fg = color; }

private:
    using Element::Element;

    Color m_bg = koterm::terminal::colors::SYS_DEFAULT;
    Color m_fg = koterm::terminal::colors::SYS_DEFAULT;

    signal_t m_onpress;
    text_t m_text;
    border_t m_border;
};

}

#endif
