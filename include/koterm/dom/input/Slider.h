#ifndef KOTERM_DOM_INPUT_SLIDER_H
#define KOTERM_DOM_INPUT_SLIDER_H

#include "koterm/component/Signal.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/screen/BaseScreen.h"
#include "koterm/util/debug.h"
#include <cstddef>
#include <limits>
#include <memory>

namespace koterm::dom::input {

class Slider : public Element {
public:
    using slider_t = std::shared_ptr<Slider>;
    using value_t  = std::size_t;
    using signal_t = component::Signal<value_t>;

    static constexpr value_t MAX = std::numeric_limits<value_t>::max();
    static constexpr value_t MIN = std::numeric_limits<value_t>::min();

    static slider_t create(const BufferSpan& buffer, DomManager* manager);
    static slider_t create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager());
    }

    bool handle_key(event::KeyCode key) override;
    bool handle_mouse_move(const event::MouseEvent& event) override;
    bool handle_mouse_click(const event::MouseEvent& event) override;

    void prepare_buffer() override;
    void calculate_requirements() override;

    value_t value() const { return m_value; }
    void set_value(value_t value) {
        m_value = std::max(std::min(value, m_value_max), m_value_min);
        m_onchange.emit(m_value);
        request_render();
    }

    void set_range(value_t min, value_t max) {
        koterm_assert(
            min < max, exception::KotermException { "invalid range: 'min' >= 'max'" }, "invalid range: 'min' >= 'max'"
        );

        m_value_min = min;
        m_value_max = max;

        set_value(m_value);
    }

    signal_t& on_change() { return m_onchange; }

private:
    using Element::Element;

    value_t m_value     = MIN;
    value_t m_value_min = MIN;
    value_t m_value_max = MAX;

    signal_t m_onchange;
    bool m_focused_by_mouse = false;
};
}

#endif
