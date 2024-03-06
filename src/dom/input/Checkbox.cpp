#include "koterm/dom/input/Checkbox.h"
#include "koterm/dom/DomManager.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/terminal/Pixel.h"
#include "koterm/unit.h"
#include <algorithm>
#include <memory>
#include <string>

namespace koterm::dom::input {

std::shared_ptr<Checkbox> Checkbox::create(const BufferSpan& buffer, DomManager* manager) {
    return std::make_shared<Checkbox>(buffer, manager, true, false, true);
}

void Checkbox::prepare_buffer() {

    m_buffer.clear();

    std::string text;

    if (m_checked) {
        text += "▣ ";
    } else {
        text += "▢ ";
    }

    text += m_label;

    auto style = terminal::PixelStyle {};
    if (has_focus()) {
        style.set<terminal::PixelStyle::StyleFlags::BOLD>();
    }

    const auto width = std::min<unit_t>(m_buffer.width(), text.size());

    for (unit_t i = 0; i < width; i++) {
        m_buffer.set_pixel_content(text[i], i, 0);
        m_buffer.set_pixel_style(style, i, 0);
    }
}

void Checkbox::calculate_requirements() {
    m_info.min_width  = static_cast<unit_t>(m_label.size() + 2);
    m_info.min_height = 1;
}

bool Checkbox::handle_key(event::KeyCode key) {
    if (!has_focus()) {
        return false;
    }

    if (key == event::KeyCode::ENTER) {
        m_checked = !m_checked;
        m_onchange.emit(m_checked);
        return true;
    }

    return false;
}

bool Checkbox::handle_mouse_click(const event::MouseEvent& event) {
    if (is_mouse_inside(event.pos()) && event.btn_pressed<event::MouseEvent::Btn::LEFT>()) {
        focus();
        return true;
    } else if (has_focus() && event.btn_released<event::MouseEvent::Btn::LEFT>()) {
        m_checked = !m_checked;
        m_onchange.emit(m_checked);
        return true;
    }

    return false;
}

}
