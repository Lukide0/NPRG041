#include "koterm/dom/DomManager.h"
#include "koterm/Dimensions.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/unit.h"
#include <algorithm>
#include <cstddef>
#include <iterator>

namespace koterm::dom {

void DomManager::handle_event(const event::Event& event) {
    using EventType = event::Event::EventType;

    if (m_root_element != nullptr) {
        if (event.type() == EventType::SPECIAL_KEY && handle_special_key(event.get<EventType::SPECIAL_KEY>())) {
            return;
        }

        m_root_element->handle_event(event);
    }

    if (event.type() == EventType::RESIZE) {
        update_size(event.get<EventType::RESIZE>());
        request_render();
    }
}

bool DomManager::steal_focus(element_ref element) {
    auto it = find_focusable(element);

    if (it == m_focusable.end()) {
        return false;
    }

    auto* focused = *it;

    if (focused->has_focus()) {
        focused->blur();
    }

    auto dist       = std::distance(m_focusable.begin(), it);
    m_focused_index = dist;

    return true;
}
void DomManager::lose_focus(element_ref element) {
    if (!element->has_focus()) {
        return;
    }

    m_focused_index = std::max<std::size_t>(m_focused_index, 1) - 1;
}

void DomManager::update_size(unit_t width, unit_t height) {
    if (m_root_element != nullptr) {
        m_root_element->handle_change_box({ width - 1, height - 1 });
    }
}

void DomManager::update_size(const Dimensions& dimensions) {
    if (m_root_element != nullptr) {
        m_root_element->handle_change_box({ dimensions.width - 1, dimensions.height - 1 });
    }
}

void DomManager::render() {
    if (m_root_element != nullptr) {
        m_root_element->render();
    }
    m_need_redraw = false;
}

bool DomManager::handle_special_key(event::KeyCode key) {
    using event::KeyCode;

    switch (key) {
    case FOCUS_NEXT_KEY:
        focus_next();
        request_render();
        return true;
    case LOOSE_FOCUS_KEY:
    case FOCUS_PREV_KEY:
        focus_prev();
        request_render();
        return true;
    default:
        return false;
    }
}

void DomManager::remove_focusable(element_ref el) {
    auto it = find_focusable(el);
    if (it == m_focusable.end()) {
        return;
    }

    if (el->has_focus()) {
        lose_focus(el);
    }

    m_focusable.erase(it);
}

void DomManager::focus_next() {

    const std::size_t size = m_focusable.size();

    if (m_focusable.empty()) {
        m_focused_index = 0;
        return;
    }

    element_ref focused = unsafe_get_focused();

    if (!focused->has_focus()) {
        focused->m_focused = true;
        request_render();
        return;
    }

    focused->m_focused = false;

    if (m_focused_index + 1 >= size) {
        if (m_focused_index == 0) {
            return;
        }
        m_focused_index = 0;

    } else {
        m_focused_index += 1;
    }

    focused            = unsafe_get_focused();
    focused->m_focused = true;
    request_render();
}

void DomManager::focus_prev() {

    const std::size_t size = m_focusable.size();

    if (m_focusable.empty()) {
        m_focused_index = 0;
        return;
    }

    element_ref focused = unsafe_get_focused();

    if (!focused->has_focus()) {
        focused->m_focused = true;
        request_render();
        return;
    }

    focused->m_focused = false;

    if (m_focused_index == 0) {
        if (size == 1) {
            return;
        }
        m_focused_index = size - 1;

    } else {
        m_focused_index -= 1;
    }

    focused            = unsafe_get_focused();
    focused->m_focused = true;
    request_render();
}

}
