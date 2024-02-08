#include "koterm/dom/Element.h"
#include "koterm/BoundingBox.h"
#include "koterm/Dimensions.h"
#include "koterm/dom/DomManager.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/unit.h"

namespace koterm::dom {

// NOLINTNEXTLINE(modernize-pass-by-value)
Element::Element(const terminal::BufferSpan& buffer, DomManager* manager)
    : m_buffer(buffer)
    , m_manager(manager) { }

Element::~Element() {
    if (m_manager != nullptr) {
        m_manager->lose_focus(this);
    }
    if (m_parent != nullptr) {
        m_parent->remove_child(this);
    }
}

void Element::show() {
    if (!m_hidden) {
        return;
    }

    m_hidden = false;
    request_update();
}

void Element::hide() {
    if (m_hidden) {
        return;
    }

    m_hidden = true;
    request_update();
}

void Element::focus() {
    if (m_focused) {
        return;
    }

    m_manager->steal_focus(this);
    m_focused = true;
    request_render();
}

void Element::blur() {
    if (!m_focused) {
        return;
    }
    m_manager->lose_focus(this);
    m_focused = false;
    request_render();
}

void Element::render() {
    if (m_need_update) {
        update_requirements();
    }

    if (m_hidden || width() == 0 || height() == 0) {
        return;
    }

    prepare_buffer();
}

void Element::handle_event(const event::Event& event) {
    using event::Event;

    switch (event.type()) {
    case Event::EventType::RESIZE:
        handle_resize(event.get<Event::EventType::RESIZE>());
        break;
    case Event::EventType::MOUSE_BTN:
        handle_mouse_click(event.get<Event::EventType::MOUSE_BTN>());
        break;
    case Event::EventType::MOUSE_SCROLL:
        handle_mouse_scroll(event.get<Event::EventType::MOUSE_SCROLL>());
        break;
    case event::Event::EventType::MOUSE_MOVE:
        handle_mouse_move(event.get<Event::EventType::MOUSE_MOVE>());
        break;
    case Event::EventType::CHARACTER:
        handle_input(event.get<Event::EventType::CHARACTER>());
        break;
    case Event::EventType::SPECIAL_KEY:
        handle_key(event.get<Event::EventType::SPECIAL_KEY>());
        break;
    case Event::EventType::CURSOR:
        handle_cursor(event.get<Event::EventType::CURSOR>());
        break;
    }
}

void Element::handle_change_box(const BoundingBox& box) {
    m_buffer.resize(box);
    request_update();
}
void Element::handle_resize([[maybe_unused]] const Dimensions& dimensions) { }
void Element::prepare_buffer() { }
void Element::handle_mouse_move([[maybe_unused]] const event::MouseEvent& event) { }
void Element::handle_mouse_click([[maybe_unused]] const event::MouseEvent& event) { }
void Element::handle_mouse_scroll([[maybe_unused]] const event::MouseEvent& event) { }
void Element::handle_input([[maybe_unused]] const event::CharacterEvent& event) { }
void Element::handle_cursor([[maybe_unused]] const point_t& cursor) { }
void Element::handle_key([[maybe_unused]] event::KeyCode key) { }
void Element::remove_child([[maybe_unused]] element_ref child) { }
void Element::request_render() { m_manager->request_render(); }

}
