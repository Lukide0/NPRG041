#include "koterm/dom/input/Button.h"
#include "koterm/dom/DomManager.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/unit.h"
#include <algorithm>
#include <memory>

namespace koterm::dom::input {

std::shared_ptr<Button> Button::create(const BufferSpan& buffer, DomManager* manager) {
    auto btn = std::make_shared<Button>(buffer, manager);
    btn->set_focusable(true);
    return btn;
}

void Button::prepare_buffer() {

    m_buffer.clear();

    BoundingBox text_box = m_buffer.box();

    unit_t width  = m_buffer.width();
    unit_t height = m_buffer.height();

    if (has_focus()) {
        for (unit_t y = 1; y < height; y++) {
            for (unit_t x = 1; x < width; x++) {
                m_buffer.set_pixel_color({ m_bg.id(), m_fg.id() }, x, y);
            }
        }
    }

    if (m_border.has()) {
        m_border.render(m_buffer);
        if (!text_box.can_shrink()) {
            return;
        }
        text_box.shrink();
    }

    auto background = m_bg;
    auto foreground = m_fg;
    if (!has_focus()) {
        background = get_pallete().background;
        foreground = get_pallete().foreground;
    }

    auto text_buffer = BufferSpan { m_buffer.buffer(), text_box };
    m_text.render(text_buffer, foreground, background);
}

void Button::calculate_requirements() {
    m_info.min_width  = static_cast<unit_t>(m_text.size());
    m_info.min_height = 2;

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
    if (!is_mouse_inside(event.pos())) {
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
