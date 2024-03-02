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

class DomManager {
public:
    DomManager(const screen::pallete::Pallete& pallete)
        : m_pallete(pallete) { }
    void set_root(element_t element) { m_root_element = element; }

    void handle_event(const event::Event& event);

    void register_focusable(element_ref el) {
        if (el->focusable()) {
            m_focusable.push_back(el);
        }
    }

    void remove_focusable(element_ref el);

    bool steal_focus(element_ref element);
    void lose_focus(element_ref element);

    void focus_next();
    void focus_prev();

    [[nodiscard]] element_ref get_focused() const {
        if (m_focusable.empty()) {
            return nullptr;
        } else {
            return m_focusable[m_focused_index];
        }
    }

    void update_size(unit_t width, unit_t height);
    void update_size(const Dimensions& dimensions);
    void render();

    [[nodiscard]] const screen::pallete::Pallete& get_pallete() const { return m_pallete; }

    [[nodiscard]] bool need_render() const { return m_need_redraw; }
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
