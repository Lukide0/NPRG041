#include "koterm/dom/input/Button.h"
#include "koterm/dom/DomManager.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include <memory>

namespace koterm::dom::input {

std::shared_ptr<Button> Button::create(const BufferSpan& buffer, DomManager* manager) {
    auto btn = std::make_shared<Button>(buffer, manager);
    btn->set_focusable(true);
    return btn;
}

void Button::prepare_buffer() {
    auto text_box = m_buffer.box();

    if (m_border.has()) {
        m_border.render(m_buffer);
        if (!text_box.can_shrink()) {
            return;
        }
        text_box.shrink();
    }

    auto text_buffer = BufferSpan { m_buffer.buffer(), text_box };
    m_text.render(text_buffer);
}

void Button::calculate_requirements() {
    m_info.min_width  = m_text.size();
    m_info.min_height = 1;

    if (m_border.has()) {
        m_info.min_width += 2;
        m_info.min_height += 2;
    }
}

bool Button::handle_key(event::KeyCode key) {
    if (!has_focus()) {
        return false;
    }

    if (key == event::KeyCode::ENTER) {
        m_onpress.emit();
        blur();
        return true;
    }

    return false;
}

bool Button::handle_mouse_click(const event::MouseEvent& event) {
    if (!m_buffer.box().contains(event.pos().x - 1, event.pos().y - 1)) {
        return false;
    }

    if (event.btn_pressed<event::MouseEvent::Btn::LEFT>()) {
        focus();
        return true;
    } else if (event.btn_released<event::MouseEvent::Btn::LEFT>()) {
        m_onpress.emit();
        blur();
        return true;
    }

    return false;
}

}
