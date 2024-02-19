#ifndef KOTERM_DOM_DOMMANAGER_H
#define KOTERM_DOM_DOMMANAGER_H

#include "koterm/Dimensions.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/screen/pallete/Pallete.h"
#include "koterm/unit.h"
namespace koterm::dom {

class DomManager {
public:
    DomManager(const screen::pallete::Pallete& pallete)
        : m_pallete(pallete) { }
    void set_root(element_t element) { m_root_element = element; }

    void handle_event(const event::Event& event);

    void steal_focus(element_ref element);
    void lose_focus(element_ref element);

    [[nodiscard]] element_ref get_focused() const { return m_focused; }

    void update_size(unit_t width, unit_t height);
    void update_size(const Dimensions& dimensions);
    void render();

    [[nodiscard]] const screen::pallete::Pallete& get_pallete() const { return m_pallete; }

    [[nodiscard]] bool need_render() const { return m_need_redraw; }
    void request_render() { m_need_redraw = true; }

private:
    element_t m_root_element;
    element_ref m_focused;
    const screen::pallete::Pallete& m_pallete;

    bool m_need_redraw = false;
};

}

#endif
