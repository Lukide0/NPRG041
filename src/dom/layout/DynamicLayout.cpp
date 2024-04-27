#include "koterm/dom/layout/DynamicLayout.h"
#include "koterm/BoundingBox.h"
#include "koterm/Dimensions.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/exceptions.h"
#include "koterm/util/debug.h"
#include <algorithm>

namespace koterm::dom::layout {

void DynamicLayout::add_element(element_t element) {
    koterm_assert(
        element.get() != this, koterm::exception::InvalidElementException("Invalid element"), "Invalid element"
    )

        element->set_parent(this);
    m_elements.push_back(element);

    request_update();
    request_layout_update();
}

void DynamicLayout::add_elements(const element_container_t& elements) {
    for (auto&& element : elements) {
        koterm_assert(
            element.get() != this, koterm::exception::InvalidElementException("Invalid element"), "Invalid element"
        ) element->set_parent(this);
    }

    m_elements.insert(m_elements.end(), elements.begin(), elements.end());
    request_update();
    request_layout_update();
}

void DynamicLayout::remove_element(element_ref element) {
    auto it = std::find_if(m_elements.begin(), m_elements.end(), [element](const element_t& el) -> bool {
        return el.get() == element;
    });

    if (it != m_elements.end()) {
        m_elements.erase(it);
        request_update();
        request_layout_update();
    }
}

bool DynamicLayout::handle_event(const event::Event& event) {

    for (auto&& element : m_elements) {
        if (element->handle_event(event)) {
            return true;
        }
    }
    return false;
}

bool DynamicLayout::has_child(element_ref element) const {
    const auto it = std::find_if(m_elements.begin(), m_elements.end(), [element](const element_t& el) -> bool {
        return el.get() == element;
    });

    return it != m_elements.end();
}

void DynamicLayout::remove_child(element_ref child) {
    remove_element(child);
    request_update();
    request_layout_update();
}

void DynamicLayout::prepare_buffer() {

    if (m_need_layout_update) {
        m_need_layout_update = false;
        update_layout();
    }

    for (auto&& element : m_elements) {
        element->render();
    }
}

}
