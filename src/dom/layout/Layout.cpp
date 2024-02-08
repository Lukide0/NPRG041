#include "koterm/dom/layout/Layout.h"
#include "koterm/BoundingBox.h"
#include "koterm/Dimensions.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include <algorithm>

namespace koterm::dom::layout {

void Layout::add_element(element_t element) {
    element->set_parent(this);
    m_elements.insert(element);

    request_update();
}

void Layout::add_elements(const element_container_t& elements) {
    for (auto&& element : elements) {
        element->set_parent(this);
    }

    m_elements.insert(elements.begin(), elements.end());
    request_update();
}

void Layout::remove_element(element_ref element) {
    auto it = std::find_if(m_elements.begin(), m_elements.end(), [element](const element_t& el) -> bool {
        return el.get() == element;
    });

    if (it != m_elements.end()) {
        m_elements.erase(it);
        request_update();
    }
}

void Layout::handle_event(const event::Event& event) {
    for (auto&& element : m_elements) {
        element->handle_event(event);
    }
}

void Layout::remove_child(element_ref child) {
    remove_element(child);
    request_update();
}

void Layout::prepare_buffer() {

    update_layout();

    for (auto&& element : m_elements) {
        element->render();
    }
}

}
