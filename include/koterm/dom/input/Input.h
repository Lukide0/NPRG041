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
    using signal_t = component::Signal<const std::string&>;

    /**
     * @brief Creates a new Input instance from a given buffer and DOM manager.
     *
     * @param buffer The buffer to render the input field onto.
     * @param manager The DOM manager.
     * @return A shared pointer to the newly created Input instance.
     */
    static std::shared_ptr<Input> create(const BufferSpan& buffer, DomManager* manager);

    /**
     * @brief Creates a new Input instance from a given BaseScreen.
     *
     * @param screen The BaseScreen to create the input field on.
     * @return A shared pointer to the newly created Input instance.
     */
    static std::shared_ptr<Input> create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager());
    }

    /**
     * @brief Handles character input events for the input field.
     *
     * @param event The character input event that occurred.
     * @return True if the character input event was handled, false otherwise.
     */
    bool handle_input(const event::CharacterEvent& event) override;

    /**
     * @brief Handles key events for the input field.
     *
     * @param key The key event that occurred.
     * @return True if the key event was handled, false otherwise.
     */
    bool handle_key(event::KeyCode key) override;

    /**
     * @brief Prepares the input field's buffer for rendering.
     *
     * This function prepares the input field's buffer by updating its contents based on
     * its current value and properties.
     */
    void prepare_buffer() override;

    void calculate_requirements() override;

    /**
     * @brief Accessor for the current value of the input field.
     *
     * @return Const reference to the current value of the input field.
     */
    const std::string& value() const { return m_value; }

    /**
     * @brief Accessor for the onChange signal of the input field.
     *
     * This signal is triggered when the value of the input field changes.
     * @return Reference to the onChange signal of the input field.
     */
    signal_t& on_change() { return m_onchange; }

private:
    using Element::Element;

    /**
     * @brief Removes a character at the specified index from the input field's value.
     *
     * @param index The index of the character to remove.
     */
    void remove_at(std::size_t index);

    std::size_t m_char_index = 0;
    std::string m_value;

    signal_t m_onchange;
};

}

#endif
