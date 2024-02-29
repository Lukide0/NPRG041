#include "koterm/dom/input/Slider.h"
#include "koterm/BoundingBox.h"
#include "koterm/dom/DomManager.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/terminal/Pixel.h"
#include "koterm/unit.h"
#include <algorithm>
#include <memory>

namespace koterm::dom::input {

std::shared_ptr<Slider> Slider::create(const BufferSpan& buffer, DomManager* manager) {
    auto input = std::make_shared<Slider>(buffer, manager);
    input->set_focusable(true);

    return input;
}

bool Slider::handle_key(event::KeyCode key) {
    if (!has_focus()) {
        return false;
    }

    switch (key) {
    case event::KeyCode::ARROW_LEFT:
        m_value = std::max(m_value_min + 1, m_value) - 1;
        m_onchange.emit(m_value);
        break;
    case event::KeyCode::ARROW_RIGHT:
        m_value = std::min(m_value_max - 1, m_value) + 1;
        m_onchange.emit(m_value);
        break;
    case event::KeyCode::F10:
        blur();
        break;
    default:
        return false;
    }

    request_update();
    return true;
}

bool Slider::handle_mouse_click(const event::MouseEvent& event) {
    using Btn = event::MouseEvent::Btn;

    if (event.btn_released<Btn::LEFT>()) {
        blur();
        m_focused_by_mouse = false;
        return true;
    } else if (!is_mouse_inside(event.pos())) {
        return false;
    }

    if (event.btn_pressed<Btn::LEFT>()) {
        focus();
        m_focused_by_mouse = true;
        return handle_mouse_move(event);
    }

    return false;
}

bool Slider::handle_mouse_move(const event::MouseEvent& event) {
    auto pos = event.pos();
    if (!is_mouse_inside(event.pos()) || !has_focus() || !m_focused_by_mouse) {
        return false;
    }

    const BoundingBox box = m_buffer.box();
    const unit_t pos_x    = pos.x - (1 + box.left);
    const value_t values  = m_value_max - m_value_min;
    const unit_t width    = box.width();

    const double percent = pos_x / static_cast<double>(width);
    const auto new_value = static_cast<value_t>(static_cast<double>(values) * percent);

    set_value(new_value);
    return true;
}

void Slider::prepare_buffer() {
    using terminal::PixelStyle;
    using namespace std::string_view_literals;

    constexpr auto bold = PixelStyle::from<PixelStyle::BOLD>();

    m_buffer.clear();

    const auto max_width = m_buffer.width();
    const auto& pallete  = get_pallete();

    PixelStyle style;
    terminal::Color color = pallete.bright_black;

    if (has_focus()) {
        style = bold;
        color = pallete.foreground;
    }

    m_buffer.set_pixel_content('[', 0, 0);
    m_buffer.set_pixel_style(style, 0, 0);
    m_buffer.set_pixel_foreground(color.id(), 0, 0);

    const unit_t width = (m_value - m_value_min) * (max_width - 2) / (m_value_max - m_value_min);

    for (unit_t i = 0; i <= width; i++) {
        m_buffer.set_pixel_content("█"sv, i + 1, 0);
        m_buffer.set_pixel_style(style, i + 1, 0);
        m_buffer.set_pixel_foreground(color.id(), i + 1, 0);
    }

    m_buffer.set_pixel_content(']', max_width, 0);
    m_buffer.set_pixel_style(style, max_width, 0);
    m_buffer.set_pixel_foreground(color.id(), max_width, 0);
}

void Slider::calculate_requirements() {
    m_info.min_width  = 12;
    m_info.min_height = 1;
}

}
