#ifndef KOTERM_DOM_DOMMANAGER_H
#define KOTERM_DOM_DOMMANAGER_H

#include "koterm/Dimensions.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/unit.h"
namespace koterm::dom {

class DomManager {
public:
    void set_root(element_t element) { m_root_element = element; }

    void handle_event(const event::Event& event);

    void steal_focus(element_ref element);
    void lose_focus(element_ref element);

    void set_active(element_ref element) { m_active = element; }

    [[nodiscard]] element_ref get_active() const { return m_active; }
    [[nodiscard]] element_ref get_focused() const { return m_focused; }

    void update_size(unit_t width, unit_t height);
    void update_size(const Dimensions& dimensions);
    void render(double delta);

private:
    element_t m_root_element;
    element_ref m_focused;
    element_ref m_active;
};

}

#endif
