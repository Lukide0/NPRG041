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
#include <memory>

namespace koterm::dom::input {

class Button : public Element {

public:
    using button_t  = std::shared_ptr<Button>;
    using signal_t  = component::Signal<>;
    using border_t  = component::Border;
    using text_t    = component::Text;
    using Alignment = component::Alignment;

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

private:
    using Element::Element;

    signal_t m_onpress;
    text_t m_text;
    border_t m_border;
};

}

#endif
