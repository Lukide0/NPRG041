#ifndef KOTERM_DOM_ELEMENT_H
#define KOTERM_DOM_ELEMENT_H

#include "koterm/BoundingBox.h"
#include "koterm/Dimensions.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/screen/pallete/Pallete.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/unit.h"
#include <concepts>
#include <cstdint>
#include <memory>
#include <set>
#include <vector>

namespace koterm::screen {
class BaseScreen;
}

namespace koterm::dom {

class DomManager;
class Element;

using element_t           = std::shared_ptr<Element>;
using element_ref         = Element*;
using element_weak_ref    = std::weak_ptr<Element>;
using element_const_ref   = const Element*;
using element_container_t = std::vector<element_t>;
using element_map_t       = std::set<element_t>;

struct ElementInfo {
    unit_t min_width;
    unit_t min_height;

    std::uint8_t grow_x = 0;
    std::uint8_t grow_y = 0;

    std::uint8_t shrink_x = 0;
    std::uint8_t shrink_y = 0;

    std::uint8_t grow_y_children = 0;
    std::uint8_t grow_x_children = 0;

    std::uint8_t shrink_y_children = 0;
    std::uint8_t shrink_x_children = 0;

    void reset_children() {
        grow_x_children   = 0;
        grow_y_children   = 0;
        shrink_x_children = 0;
        shrink_y_children = 0;
    }
};

class Element {
public:
    using BufferSpan = terminal::BufferSpan;
    using Ascii      = util::ascii::codes;
    /**
     * @brief Constructs an element with the given buffer span and DOM manager.
     *
     * @param buffer The buffer span for the element.
     * @param manager The DOM manager for the element.
     * @param focusable Flag indicating if the element is focusable.
     * @param shrinkable Flag indicating if the element is shrinkable.
     * @param growable Flag indicating if the element is growable.
     */
    Element(
        const BufferSpan& buffer,
        DomManager* manager,
        bool focusable  = false,
        bool shrinkable = false,
        bool growable   = false
    );
    /**
     * @brief Constructs an element with the given screen and optional flags.
     *
     * @param screen The base screen for the element.
     * @param focusable Flag indicating if the element is focusable.
     * @param shrinkable Flag indicating if the element is shrinkable.
     * @param growable Flag indicating if the element is growable.
     */
    Element(screen::BaseScreen* screen, bool focusable = false, bool shrinkable = false, bool growable = false);
    virtual ~Element();

    /**
     * @brief Shows the element.
     */
    void show();

    /**
     * @brief Hides the element.
     */
    void hide();

    /**
     * @brief Checks if the element is visible.
     *
     * @return True if the element is visible, false otherwise.
     */
    [[nodiscard]] bool is_visible() const { return !m_hidden; }

    /**
     * @brief Focuses on the element.
     */
    void focus();

    /**
     * @brief Removes focus from the element.
     */
    void blur();

    /**
     * @brief Checks if the element is focusable.
     *
     * @return True if the element is focusable, false otherwise.
     */
    [[nodiscard]] bool focusable() const { return m_focusable; }

    /**
     * @brief Checks if the element has focus.
     *
     * @return True if the element has focus, false otherwise.
     */
    [[nodiscard]] bool has_focus() const { return m_focused; }

    /**
     * @brief Renders the element.
     */
    void render();

    /**
     * @brief Retrieves the width of the element.
     *
     * @return The width of the element.
     */
    [[nodiscard]] unit_t width() const { return m_buffer.width(); }

    /**
     * @brief Retrieves the height of the element.
     *
     * @return The height of the element.
     */
    [[nodiscard]] unit_t height() const { return m_buffer.height(); }

    /**
     * @brief Retrieves the parent element of the current element.
     *
     * @return A constant pointer to the parent element or nullptr.
     */
    [[nodiscard]] element_const_ref parent() const { return m_parent; }

    /**
     * @brief Retrieves the parent element of the current element.
     *
     * @return A pointer to the parent element or nullptr.
     */
    [[nodiscard]] element_ref parent() { return m_parent; }

    /**
     * @brief Sets the parent element of the current element.
     *
     * @param parent The parent element to set.
     */
    void set_parent(element_ref parent) { m_parent = parent; }

    /**
     * @brief Handles an event for the element.
     *
     * @param event The event to handle.
     * @return True if the event was handled, false otherwise.
     */
    virtual bool handle_event(const event::Event& event);

    /**
     * @brief Handles a terminal resize event.
     *
     * @param dimensions The new dimensions after resize.
     * @return True if the resize was handled, false otherwise.
     */
    virtual bool handle_resize(const Dimensions& dimensions);

    /**
     * @brief Handles the change of bounding box for the element.
     *
     * @param box The new bounding box for the element.
     * @return True if the change was handled, false otherwise.
     */
    virtual bool handle_change_box(const BoundingBox& box);

    /**
     * @brief Handles mouse movement events for the element.
     *
     * @param event The mouse event containing information about the mouse movement.
     * @return True if the mouse movement was handled, false otherwise.
     */
    virtual bool handle_mouse_move(const event::MouseEvent& event);

    /**
     * @brief Handles mouse click events for the element.
     *
     * @param event The mouse event containing information about the mouse click.
     * @return True if the mouse click was handled, false otherwise.
     */
    virtual bool handle_mouse_click(const event::MouseEvent& event);

    /**
     * @brief Handles mouse scroll events for the element.
     *
     * @param event The mouse event containing information about the mouse scroll.
     * @return True if the mouse scroll was handled, false otherwise.
     */
    virtual bool handle_mouse_scroll(const event::MouseEvent& event);

    /**
     * @brief Handles input events for the element.
     *
     * @param event The character event containing information about the input.
     * @return True if the input was handled, false otherwise.
     */
    virtual bool handle_input(const event::CharacterEvent& event);

    /**
     * @brief Handles response to the request for the cursor position.
     *
     * @param cursor The new cursor position.
     * @return True if the cursor movement is handled successfully, false otherwise.
     */
    virtual bool handle_cursor(const point_t& cursor);

    /**
     * @brief Handles key events for the element.
     *
     * @param key The key code representing the pressed key.
     * @return True if the key event was handled, false otherwise.
     */
    virtual bool handle_key(event::KeyCode key);

    /**
     * @brief Handles special input events for the element.
     *
     * @param code The ASCII code representing the special input.
     * @return True if the special input event is handled successfully, false otherwise.
     */
    virtual bool handle_special_input(Ascii code);

    /**
     * @brief Retrieves the requirements of the element.
     *
     * @return A const reference to the element's requirements.
     */
    [[nodiscard]] const ElementInfo& requirements() const { return m_info; }

    /**
     * @brief Updates the requirements of the element.
     *
     * This function is called to update the element's requirements.
     */
    void update_requirements() {
        m_need_update = false;
        calculate_requirements();
    }

    virtual void calculate_requirements() = 0;

    /**
     * @brief Sets the growth parameters for the element.
     *
     * @param x The horizontal growth factor.
     * @param y The vertical growth factor.
     */
    void set_grow(std::uint8_t x, std::uint8_t y) {
        if (m_growable) {
            m_info.grow_x = x;
            m_info.grow_y = y;
        }
    }

    /**
     * @brief Sets the shrink parameters for the element.
     *
     * @param x The horizontal shrink factor.
     * @param y The vertical shrink factor.
     */
    void set_shrink(std::uint8_t x, std::uint8_t y) {
        if (m_shrinkable) {
            m_info.shrink_x = x;
            m_info.shrink_y = y;
        }
    }

    /**
     * @brief Checks if the element has a child.
     *
     * @param element The child element to check.
     * @return True if the element has the specified child, false otherwise.
     */
    virtual bool has_child(element_ref element) const;

    /**
     * @brief Checks if a point is inside the element.
     *
     * @param point The point to check.
     * @return True if the point is inside the element, false otherwise.
     */
    [[nodiscard]] bool is_mouse_inside(point_t point) const {
        return m_buffer.box().contains(point.x - 1, point.y - 1);
    }

protected:
    /**
     * @brief Prepares the buffer for rendering.
     *
     * This function is called before rendering to prepare the buffer.
     * Subclasses can override this function to customize buffer preparation.
     */
    virtual void prepare_buffer();

    /**
     * @brief Removes a child element from the current element.
     *
     * @param child The child element to remove.
     */
    virtual void remove_child(element_ref child);

    /**
     * @brief Requests a render for the element.
     *
     * This function is called to request a render for the element.
     */
    void request_render();

    /**
     * @brief Checks if the element needs an update.
     *
     * @return True if the element needs an update, false otherwise.
     */
    [[nodiscard]] bool need_update() const { return m_need_update; }

    /**
     * @brief Retrieves the palette for the element.
     *
     * @return A const reference to the palette for the element.
     */
    [[nodiscard]] const screen::pallete::Pallete& get_pallete() const;

    /**
     * @brief Requests an update for the element.
     *
     * This function is called to request an update for the element.
     */
    void request_update() {
        m_need_update = true;
        request_render();
    }

    terminal::BufferSpan m_buffer;
    ElementInfo m_info;

private:
    element_ref m_parent = nullptr;
    bool m_hidden        = false;
    bool m_focused       = false;
    bool m_focusable     = false;
    bool m_need_update   = false;
    bool m_shrinkable    = false;
    bool m_growable      = false;
    DomManager* m_manager;

    friend class DomManager;
};

}

#endif
