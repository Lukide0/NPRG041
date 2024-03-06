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
    using color_t   = koterm::terminal::Color;

    /**
     * @brief Creates a new Button instance from a given buffer and DOM manager.
     *
     * @param buffer The buffer to render the button onto.
     * @param manager The DOM manager.
     * @return A shared pointer to the newly created Button instance.
     */
    static std::shared_ptr<Button> create(const BufferSpan& buffer, DomManager* manager);

    /**
     * @brief Creates a new Button instance from a given BaseScreen.
     *
     * @param screen The BaseScreen to create the button on.
     * @return A shared pointer to the newly created Button instance.
     */
    static std::shared_ptr<Button> create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager());
    }

    /**
     * @brief Accessor for the press signal of the button.
     *
     * This signal is triggered when the button is pressed.
     * @return Reference to the press signal of the button.
     */
    signal_t& on_press() { return m_onpress; }

    /**
     * @brief Prepares the button's buffer for rendering.
     *
     * This function prepares the button's buffer by updating its contents based on
     * its current state and properties.
     */
    void prepare_buffer() override;

    void calculate_requirements() override;

    /**
     * @brief Handles key events for the button.
     *
     * @param key The key event that occurred.
     * @return True if the key event was handled, false otherwise.
     */
    bool handle_key(event::KeyCode key) override;

    /**
     * @brief Handles mouse click events for the button.
     *
     * @param event The mouse click event that occurred.
     * @return True if the mouse click event was handled, false otherwise.
     */
    bool handle_mouse_click(const event::MouseEvent& event) override;

    /**
     * @brief Accessor for the text component of the button.
     *
     * @return Const reference to the text component of the button.
     */
    [[nodiscard]] const text_t& text() const { return m_text; }

    /**
     * @brief Modifier for the text component of the button.
     *
     * This function allows modification of the text component of the button,
     * triggering a render update.
     *
     * @return Reference to the text component of the button.
     */
    text_t& modify_text() {
        request_update();
        return m_text;
    }

    /**
     * @brief Accessor for the border component of the button.
     *
     * @return Const reference to the border component of the button.
     */
    [[nodiscard]] const border_t& border() const { return m_border; }

    /**
     * @brief Modifier for the border component of the button.
     *
     * This function allows modification of the border component of the button,
     * triggering a render update.
     *
     * @return Reference to the border component of the button.
     */
    border_t& modify_border() {
        request_update();
        return m_border;
    }

    /**
     * @brief Accessor for the background color of the button.
     *
     * @return The background color of the button.
     */
    [[nodiscard]] color_t background() const { return m_bg; }

    /**
     * @brief Accessor for the foreground color of the button.
     *
     * @return The foreground color of the button.
     */
    [[nodiscard]] color_t foreground() const { return m_fg; }

    /**
     * @brief Sets the background color of the button.
     *
     * @param color The background color to set.
     */
    void set_background(color_t color) { m_bg = color; }

    /**
     * @brief Sets the foreground color of the button.
     *
     * @param color The foreground color to set.
     */
    void set_foreground(color_t color) { m_fg = color; }

private:
    using Element::Element;

    color_t m_bg = koterm::terminal::colors::SYS_DEFAULT;
    color_t m_fg = koterm::terminal::colors::SYS_CYAN;

    signal_t m_onpress;
    text_t m_text;
    border_t m_border;
};

}

#endif
