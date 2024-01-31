#include "koterm/dom/Element.h"
#include "koterm/BoundingBox.h"
#include "koterm/Dimensions.h"
#include "koterm/dom/DomManager.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/unit.h"

namespace koterm::dom {

Element::Element(const terminal::BufferSpan& buffer, DomManager* manager)
    : m_manager(manager)
    , m_buffer(buffer) { }

void Element::focus() {
    m_manager->steal_focus(this);
    m_focused = true;
}

void Element::blur() {
    if (m_focused) {
        m_manager->lose_focus(this);
        m_focused = false;
    }
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

void Element::handle_resize(const Dimensions& dimensions) { m_buffer.resize({ dimensions.width, dimensions.height }); }
void Element::render([[maybe_unused]] double delta) { }
void Element::handle_mouse_move([[maybe_unused]] const event::MouseEvent& event) { }
void Element::handle_mouse_click([[maybe_unused]] const event::MouseEvent& event) { }
void Element::handle_mouse_scroll([[maybe_unused]] const event::MouseEvent& event) { }
void Element::handle_input([[maybe_unused]] const event::CharacterEvent& event) { }
void Element::handle_cursor([[maybe_unused]] const point_t& cursor) { }
void Element::handle_key([[maybe_unused]] event::KeyCode key) { }

}
