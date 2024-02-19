#include "koterm/dom/Element.h"
#include "koterm/BoundingBox.h"
#include "koterm/Dimensions.h"
#include "koterm/component/Border.h"
#include "koterm/dom/DomManager.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/screen/BaseScreen.h"
#include "koterm/screen/pallete/Pallete.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/unit.h"
#include "koterm/util/ascii.h"
#include "koterm/util/debug.h"

namespace koterm::dom {

// NOLINTNEXTLINE(modernize-pass-by-value)
Element::Element(const BufferSpan& buffer, DomManager* manager)
    : m_buffer(buffer)
    , m_manager(manager) { }

Element::Element(screen::BaseScreen* screen)
    : m_buffer(screen->buffer())
    , m_manager(&screen->dom_manager()) { }

Element::~Element() {
    if (m_manager != nullptr && m_focused) {
        m_manager->lose_focus(this);
    }
    if (m_parent != nullptr) {
        m_parent->remove_child(this);
    }
}

const screen::pallete::Pallete& Element::get_pallete() const { return m_manager->get_pallete(); }
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
    if (m_focused || !m_focusable) {
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

    if constexpr (util::debug_dom()) {
        component::Border border(component::Border::Style::THIN);
        border.render(m_buffer);
    }
}

bool Element::handle_event(const event::Event& event) {
    using event::Event;

    switch (event.type()) {
    case Event::EventType::RESIZE:
        return handle_resize(event.get<Event::EventType::RESIZE>());
    case Event::EventType::MOUSE_BTN:
        return handle_mouse_click(event.get<Event::EventType::MOUSE_BTN>());
    case Event::EventType::MOUSE_SCROLL:
        return handle_mouse_scroll(event.get<Event::EventType::MOUSE_SCROLL>());
    case event::Event::EventType::MOUSE_MOVE:
        return handle_mouse_move(event.get<Event::EventType::MOUSE_MOVE>());
    case Event::EventType::CHARACTER:
        return handle_input(event.get<Event::EventType::CHARACTER>());
    case Event::EventType::SPECIAL_KEY:
        return handle_key(event.get<Event::EventType::SPECIAL_KEY>());
    case Event::EventType::CURSOR:
        return handle_cursor(event.get<Event::EventType::CURSOR>());
    case Event::EventType::SPECIAL_CHARACTER:
        return handle_special_input(event.get<Event::EventType::SPECIAL_CHARACTER>());
    case Event::EventType::NONE:
        return false;
    }

    return false;
}

bool Element::handle_change_box(const BoundingBox& box) {
    m_buffer.resize(box);
    request_update();
    return true;
}

bool Element::has_child([[maybe_unused]] element_ref element) const { return false; }

bool Element::handle_resize([[maybe_unused]] const Dimensions& dimensions) { return false; }
void Element::prepare_buffer() { }
bool Element::handle_mouse_move([[maybe_unused]] const event::MouseEvent& event) { return false; }
bool Element::handle_mouse_click([[maybe_unused]] const event::MouseEvent& event) { return false; }
bool Element::handle_mouse_scroll([[maybe_unused]] const event::MouseEvent& event) { return false; }
bool Element::handle_input([[maybe_unused]] const event::CharacterEvent& event) { return false; }
bool Element::handle_cursor([[maybe_unused]] const point_t& cursor) { return false; }
bool Element::handle_key([[maybe_unused]] event::KeyCode key) { return false; }
bool Element::handle_special_input([[maybe_unused]] util::ascii::codes code) { return false; }
void Element::remove_child([[maybe_unused]] element_ref child) { }
void Element::request_render() { m_manager->request_render(); }

}
