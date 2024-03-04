#ifndef KOTERM_DOM_INPUT_CHECKBOX_H
#define KOTERM_DOM_INPUT_CHECKBOX_H

#include "koterm/component/Signal.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/screen/BaseScreen.h"
#include <memory>
#include <string>
namespace koterm::dom::input {

class Checkbox : public Element {
public:
    using signal_t = component::Signal<bool>;

    static std::shared_ptr<Checkbox> create(const BufferSpan& buffer, DomManager* manager);
    static std::shared_ptr<Checkbox> create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager());
    }

    bool handle_key(event::KeyCode key) override;
    bool handle_mouse_click(const event::MouseEvent& event) override;

    void prepare_buffer() override;
    void calculate_requirements() override;

    signal_t& on_change() { return m_onchange; }
    [[nodiscard]] const std::string& label() const { return m_label; }
    void set_label(const std::string& label) { m_label = label; }

    void check() {
        if (m_checked) {
            return;
        }
        m_checked = true;
        m_onchange.emit(true);
        request_render();
    }
    void uncheck() {
        if (!m_checked) {
            return;
        }
        m_checked = false;
        m_onchange.emit(false);
        request_render();
    }

    void set_value(bool value) {
        if (m_checked == value) {
            return;
        }
        m_checked = value;
        m_onchange.emit(value);
        request_render();
    }

    [[nodiscard]] bool value() const { return m_checked; }

private:
    using Element::Element;

    std::string m_label;
    bool m_checked = false;
    signal_t m_onchange;
};

}

#endif
