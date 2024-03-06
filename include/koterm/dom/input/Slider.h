#ifndef KOTERM_DOM_INPUT_SLIDER_H
#define KOTERM_DOM_INPUT_SLIDER_H

#include "koterm/component/Signal.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/exceptions.h"
#include "koterm/screen/BaseScreen.h"
#include "koterm/util/debug.h"
#include <algorithm>
#include <cstddef>
#include <limits>
#include <memory>

namespace koterm::dom::input {

class Slider : public Element {
public:
    using value_t  = std::size_t;
    using signal_t = component::Signal<value_t>;

    static constexpr value_t MAX = std::numeric_limits<value_t>::max();
    static constexpr value_t MIN = std::numeric_limits<value_t>::min();

    /**
     * @brief Creates a new Slider instance with the specified buffer and DOM manager.
     *
     * @param buffer The buffer to render the slider onto.
     * @param manager The DOM manager.
     * @return A shared pointer to the newly created Slider instance.
     */
    static std::shared_ptr<Slider> create(const BufferSpan& buffer, DomManager* manager);

    /**
     * @brief Creates a new Slider instance with the buffer from the specified screen and its DOM manager.
     *
     * @param screen The BaseScreen to create the slider on.
     * @return A shared pointer to the newly created Slider instance.
     */
    static std::shared_ptr<Slider> create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager());
    }

    /**
     * @brief Handles key events for the slider.
     *
     * @param key The key event that occurred.
     * @return True if the key event was handled, false otherwise.
     */
    bool handle_key(event::KeyCode key) override;

    /**
     * @brief Handles mouse move events for the slider.
     *
     * @param event The mouse move event that occurred.
     * @return True if the mouse move event was handled, false otherwise.
     */
    bool handle_mouse_move(const event::MouseEvent& event) override;

    /**
     * @brief Handles mouse click events for the slider.
     *
     * @param event The mouse click event that occurred.
     * @return True if the mouse click event was handled, false otherwise.
     */
    bool handle_mouse_click(const event::MouseEvent& event) override;

    /**
     * @brief Prepares the slider for rendering.
     *
     * This function prepares the slider for rendering by updating its buffer contents based on
     * its current state and properties.
     */
    void prepare_buffer() override;
    void calculate_requirements() override;

    /**
     * @brief Retrieves the current value of the slider.
     *
     * @return The current value of the slider.
     */
    value_t value() const { return m_value; }

    /**
     * @brief Sets the value of the slider.
     *
     * @param value The new value to set for the slider.
     */
    void set_value(value_t value) {
        m_value = std::max(std::min(value, m_value_max), m_value_min);
        m_onchange.emit(m_value);
        request_render();
    }

    /**
     * @brief Sets the range of values for the slider.
     *
     * @param min The minimum value of the slider range.
     * @param max The maximum value of the slider range.
     */
    void set_range(value_t min, value_t max) {
        koterm_assert(
            min < max,
            exception::InvalidRangeException { "invalid range: 'min' >= 'max'" },
            "invalid range: 'min' >= 'max'"
        );

        m_value_min = min;
        m_value_max = max;

        set_value(m_value);
    }

    /**
     * @brief Accessor for the onChange signal of the slider.
     *
     * This signal is triggered when the value of the slider changes.
     * @return Reference to the onChange signal of the slider.
     */
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
