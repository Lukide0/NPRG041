#include "koterm/dom/input/Button.h"
#include "koterm/dom/DomManager.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/terminal/Color.h"
#include "koterm/terminal/colors.h"
#include "koterm/unit.h"
#include <memory>

namespace koterm::dom::input {

std::shared_ptr<Button> Button::create(const BufferSpan& buffer, DomManager* manager) {
    return std::make_shared<Button>(buffer, manager, true, false, true);
}

void Button::prepare_buffer() {

    m_buffer.clear();

    BoundingBox text_box = m_buffer.box();

    unit_t width  = m_buffer.width();
    unit_t height = m_buffer.height();

    const auto& pallete = get_pallete();

    terminal::Color fg = (m_fg == terminal::colors::SYS_DEFAULT) ? pallete.foreground : m_fg;
    terminal::Color bg = (m_bg == terminal::colors::SYS_DEFAULT) ? pallete.background : m_bg;
    if (has_focus()) {
        for (unit_t y = 1; y < height; y++) {
            for (unit_t x = 1; x < width; x++) {
                m_buffer.set_pixel_color({ bg.id(), fg.id() }, x, y);
            }
        }
    }

    if (m_border.has_style()) {
        m_border.render(m_buffer);
        if (!text_box.can_shrink()) {
            return;
        }
        text_box.shrink();
    }

    auto background = bg;
    auto foreground = fg;
    if (!has_focus()) {
        background = pallete.background;
        foreground = pallete.foreground;
    }

    auto text_buffer = BufferSpan { m_buffer.buffer(), text_box };
    m_text.render(text_buffer, foreground, background);
}

void Button::calculate_requirements() {
    m_info.min_width  = static_cast<unit_t>(m_text.size());
    m_info.min_height = 2;

    if (m_border.has_style()) {
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
    if (event.btn_released<event::MouseEvent::Btn::LEFT>() && has_focus()) {
        m_onpress.emit();
        blur();
        return true;
    } else if (is_mouse_inside(event.pos()) && event.btn_pressed<event::MouseEvent::Btn::LEFT>()) {
        focus();
        return true;
    }

    return false;
}

}
