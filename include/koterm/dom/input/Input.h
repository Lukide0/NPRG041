#ifndef KOTERM_DOM_INPUT_INPUT_H
#define KOTERM_DOM_INPUT_INPUT_H

#include "koterm/component/Signal.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/screen/BaseScreen.h"
#include <cstddef>
#include <memory>
#include <string>

namespace koterm::dom::input {

class Input : public Element {
public:
    using input_t  = std::shared_ptr<Input>;
    using signal_t = component::Signal<const std::string&>;

    static input_t create(const BufferSpan& buffer, DomManager* manager);
    static input_t create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager());
    }

    bool handle_input(const event::CharacterEvent& event) override;
    bool handle_key(event::KeyCode key) override;

    void prepare_buffer() override;
    void calculate_requirements() override;

    const std::string& value() const { return m_value; }
    signal_t& on_change() { return m_onchange; }

private:
    using Element::Element;

    void remove_at(std::size_t index);

    std::size_t m_char_index = 0;
    std::string m_value;

    signal_t m_onchange;
};

}

#endif
