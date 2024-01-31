#ifndef KOTERM_DOM_ELEMENT_H
#define KOTERM_DOM_ELEMENT_H

#include "koterm/BoundingBox.h"
#include "koterm/Dimensions.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/unit.h"
#include <memory>
#include <vector>
namespace koterm::dom {

class DomManager;

class Element {
public:
    Element(const terminal::BufferSpan& buffer, DomManager* manager);
    virtual ~Element();
    virtual Dimensions required_size(Dimensions available) = 0;

    void show() { m_hidden = false; }
    void hide() {
        m_hidden = true;
        blur();
    }
    [[nodiscard]] bool is_visible() const { return !m_hidden; }

    void focus();
    void blur();
    [[nodiscard]] bool has_focus() const { return m_focused; }

    virtual void handle_event(const event::Event& event);
    virtual void handle_resize(const Dimensions& dimensions);

    virtual void render(double delta);

protected:
    virtual void handle_mouse_move(const event::MouseEvent& event);
    virtual void handle_mouse_click(const event::MouseEvent& event);
    virtual void handle_mouse_scroll(const event::MouseEvent& event);
    virtual void handle_input(const event::CharacterEvent& event);
    virtual void handle_cursor(const point_t& cursor);
    virtual void handle_key(event::KeyCode key);

private:
    bool m_hidden  = false;
    bool m_focused = false;
    DomManager* m_manager;
    terminal::BufferSpan m_buffer;
};

using element_t      = std::shared_ptr<Element>;
using element_ref    = Element* /*std::weak_ptr<Element>*/;
using element_list_t = std::vector<element_t>;

}

#endif
