#include "koterm/dom/DomManager.h"
#include "koterm/Dimensions.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/unit.h"

namespace koterm::dom {

void DomManager::handle_event(const event::Event& event) {
    if (m_root_element != nullptr) {
        m_root_element->handle_event(event);
    }
    if (event.type() == event::Event::EventType::RESIZE) {
        update_size(event.get<event::Event::EventType::RESIZE>());
        request_render();
    }
}

void DomManager::steal_focus(element_ref element) { m_focused = element; }
void DomManager::lose_focus(element_ref element) {
    if (m_focused == element) {
        m_focused = nullptr;

        auto* el = element->parent();
        while (el != nullptr && !el->focusable()) {
            el = el->parent();
        }

        if (m_focused != nullptr) {
            el->focus();
        }
    }
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

}
