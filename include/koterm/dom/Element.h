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
using element_ref         = Element* /*std::weak_ptr<Element>*/;
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

    Element(const BufferSpan& buffer, DomManager* manager);
    Element(screen::BaseScreen* screen);
    virtual ~Element();

    void show();
    void hide();
    [[nodiscard]] bool is_visible() const { return !m_hidden; }

    void focus();
    void blur();
    [[nodiscard]] bool focusable() const { return m_focusable; }
    [[nodiscard]] bool has_focus() const { return m_focused; }

    void render();
    [[nodiscard]] unit_t width() const { return m_buffer.width(); }
    [[nodiscard]] unit_t height() const { return m_buffer.height(); }
    [[nodiscard]] element_const_ref parent() const { return m_parent; }
    [[nodiscard]] element_ref parent() { return m_parent; }

    void set_parent(element_ref parent) { m_parent = parent; }

    virtual bool handle_event(const event::Event& event);
    virtual bool handle_resize(const Dimensions& dimensions);
    virtual bool handle_change_box(const BoundingBox& box);
    virtual bool handle_mouse_move(const event::MouseEvent& event);
    virtual bool handle_mouse_click(const event::MouseEvent& event);
    virtual bool handle_mouse_scroll(const event::MouseEvent& event);
    virtual bool handle_input(const event::CharacterEvent& event);
    virtual bool handle_cursor(const point_t& cursor);
    virtual bool handle_key(event::KeyCode key);
    virtual bool handle_special_input(Ascii code);

    [[nodiscard]] const ElementInfo& requirements() const { return m_info; }
    void update_requirements() {
        m_need_update = false;
        calculate_requirements();
    }
    virtual void calculate_requirements() = 0;

    void set_grow(std::uint8_t x, std::uint8_t y) {
        m_info.grow_x = x;
        m_info.grow_y = y;
    }

    void set_shrink(std::uint8_t x, std::uint8_t y) {
        m_info.shrink_x = x;
        m_info.shrink_y = y;
    }

    virtual bool has_child(element_ref element) const;

    [[nodiscard]] bool is_mouse_inside(point_t point) const {
        return m_buffer.box().contains(point.x - 1, point.y - 1);
    }

protected:
    virtual void prepare_buffer();
    virtual void remove_child(element_ref child);

    void request_render();

    void set_focusable(bool focusable = false) { m_focusable = focusable; }

    [[nodiscard]] bool need_update() const { return m_need_update; }
    [[nodiscard]] const screen::pallete::Pallete& get_pallete() const;

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
    DomManager* m_manager;
};

}

#endif
