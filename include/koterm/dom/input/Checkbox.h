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

    /**
     * @brief Creates a new Checkbox instance from a given buffer and DOM manager.
     *
     * @param buffer The buffer to render the checkbox onto.
     * @param manager The DOM manager.
     * @return A shared pointer to the newly created Checkbox instance.
     */
    static std::shared_ptr<Checkbox> create(const BufferSpan& buffer, DomManager* manager);

    /**
     * @brief Creates a new Checkbox instance from a given BaseScreen.
     *
     * @param screen The BaseScreen to create the checkbox on.
     * @return A shared pointer to the newly created Checkbox instance.
     */
    static std::shared_ptr<Checkbox> create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager());
    }

    /**
     * @brief Handles key events for the checkbox.
     *
     * @param key The key event that occurred.
     * @return True if the key event was handled, false otherwise.
     */
    bool handle_key(event::KeyCode key) override;

    /**
     * @brief Handles mouse click events for the checkbox.
     *
     * @param event The mouse click event that occurred.
     * @return True if the mouse click event was handled, false otherwise.
     */
    bool handle_mouse_click(const event::MouseEvent& event) override;

    /**
     * @brief Prepares the checkbox's buffer for rendering.
     *
     * This function prepares the checkbox's buffer by updating its contents based on
     * its current state and properties.
     */
    void prepare_buffer() override;
    void calculate_requirements() override;

    /**
     * @brief Accessor for the onChange signal of the checkbox.
     *
     * This signal is triggered when the value of the checkbox changes.
     * @return Reference to the onChange signal of the checkbox.
     */
    signal_t& on_change() { return m_onchange; }

    /**
     * @brief Accessor for the label of the checkbox.
     *
     * @return Const reference to the label of the checkbox.
     */
    [[nodiscard]] const std::string& label() const { return m_label; }

    /**
     * @brief Sets the label of the checkbox.
     *
     * @param label The label text to set.
     */
    void set_label(const std::string& label) { m_label = label; }

    /**
     * @brief Checks the checkbox.
     *
     * Sets the checkbox to the checked state and emits the onChange signal.
     */
    void check() {
        if (m_checked) {
            return;
        }
        m_checked = true;
        m_onchange.emit(true);
        request_render();
    }

    /**
     * @brief Unchecks the checkbox.
     *
     * Sets the checkbox to the unchecked state and emits the onChange signal.
     */
    void uncheck() {
        if (!m_checked) {
            return;
        }
        m_checked = false;
        m_onchange.emit(false);
        request_render();
    }

    /**
     * @brief Sets the value of the checkbox.
     *
     * @param value The value to set (true for checked, false for unchecked).
     */
    void set_value(bool value) {
        if (m_checked == value) {
            return;
        }
        m_checked = value;
        m_onchange.emit(value);
        request_render();
    }

    /**
     * @brief Accessor for the value of the checkbox.
     *
     * @return The current value of the checkbox (true for checked, false for unchecked).
     */
    [[nodiscard]] bool value() const { return m_checked; }

private:
    using Element::Element;

    std::string m_label;
    bool m_checked = false;
    signal_t m_onchange;
};

}

#endif
