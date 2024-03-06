#ifndef KOTERM_DOM_DOMMANAGER_H
#define KOTERM_DOM_DOMMANAGER_H

#include "koterm/Dimensions.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/screen/pallete/Pallete.h"
#include "koterm/unit.h"
#include <algorithm>
#include <cstddef>
#include <vector>
namespace koterm::dom {

constexpr event::KeyCode LOOSE_FOCUS_KEY = event::KeyCode::F10;
constexpr event::KeyCode FOCUS_NEXT_KEY  = event::KeyCode::PAGE_DOWN;
constexpr event::KeyCode FOCUS_PREV_KEY  = event::KeyCode::PAGE_UP;

/**
 * @brief Class responsible for managing the Document Object Model (DOM) of the user interface.
 */
class DomManager {
public:
    /**
     * @brief Constructs a DomManager object with the specified palette.
     *
     * @param pallete The palette used for rendering.
     */
    DomManager(const screen::pallete::Pallete& pallete)
        : m_pallete(pallete) { }

    /**
     * @brief Sets the root element of the DOM.
     *
     * @param element The root element.
     */
    void set_root(element_t element) { m_root_element = element; }

    /**
     * @brief Handles the given event.
     *
     * @param event The event to handle.
     */
    void handle_event(const event::Event& event);

    /**
     * @brief Registers a focusable element.
     *
     * @param el The focusable element to register.
     */
    void register_focusable(element_ref el) {
        if (el->focusable()) {
            m_focusable.push_back(el);
        }
    }

    /**
     * @brief Removes a focusable element.
     *
     * @param el The focusable element to remove.
     */
    void remove_focusable(element_ref el);

    /**
     * @brief Attempts to steal focus from the current focused element.
     *
     * @param element The element to steal focus to.
     * @return True if focus was successfully stolen, false otherwise.
     */
    bool steal_focus(element_ref element);

    /**
     * @brief Loses focus from the specified element.
     *
     * @param element The element to lose focus from.
     */
    void lose_focus(element_ref element);

    /**
     * @brief Moves focus to the next focusable element.
     */
    void focus_next();

    /**
     * @brief Moves focus to the previous focusable element.
     */
    void focus_prev();

    /**
     * @brief Retrieves the currently focused element.
     *
     * @return The currently focused element.
     */
    [[nodiscard]] element_ref get_focused() const {
        if (m_focusable.empty()) {
            return nullptr;
        } else {
            return m_focusable[m_focused_index];
        }
    }

    /**
     * @brief Updates the size of the DOM.
     *
     * @param width The new width of the DOM.
     * @param height The new height of the DOM.
     */
    void update_size(unit_t width, unit_t height);

    /**
     * @brief Updates the size of the DOM.
     *
     * @param dimensions The new dimensions of the DOM.
     */
    void update_size(const Dimensions& dimensions);

    /**
     * @brief Renders the DOM.
     */
    void render();

    /**
     * @brief Retrieves the palette used for rendering.
     *
     * @return The palette used for rendering.
     */
    [[nodiscard]] const screen::pallete::Pallete& get_pallete() const { return m_pallete; }

    /**
     * @brief Checks if a render is needed.
     *
     * @return True if a render is needed, false otherwise.
     */
    [[nodiscard]] bool need_render() const { return m_need_redraw; }

    /**
     * @brief Requests a render.
     */
    void request_render() { m_need_redraw = true; }

private:
    element_t m_root_element;
    const screen::pallete::Pallete& m_pallete;
    bool m_need_redraw = false;

    std::vector<element_ref> m_focusable;
    std::size_t m_focused_index = 0;

    bool handle_special_key(event::KeyCode key);
    [[nodiscard]] element_ref unsafe_get_focused() const { return m_focusable[m_focused_index]; }
    auto find_focusable(element_ref element) { return std::find(m_focusable.begin(), m_focusable.end(), element); }
};

}

#endif
