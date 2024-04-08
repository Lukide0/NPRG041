#include "koterm/dom/input/Input.h"
#include "koterm/component/Border.h"
#include "koterm/dom/DomManager.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/terminal/Pixel.h"
#include "koterm/unit.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <string_view>

namespace koterm::dom::input {

std::shared_ptr<Input> Input::create(const BufferSpan& buffer, DomManager* manager) {
    return std::make_shared<Input>(buffer, manager, true, true, true);
}

bool Input::handle_input(const event::CharacterEvent& event) {

    if (!has_focus()) {
        return false;
    }

    if (m_char_index != m_value.size()) {
        m_value = m_value.substr(0, m_char_index) + event.character + m_value.substr(m_char_index);
    } else {
        m_value += event.character;
    }

    m_char_index += 1;
    m_onchange.emit(m_value);
    request_update();

    return true;
}

bool Input::handle_key(event::KeyCode key) {
    if (!has_focus()) {
        return false;
    }

    switch (key) {
    case event::KeyCode::ARROW_LEFT:
        m_char_index = std::max<std::size_t>(m_char_index, 1) - 1;
        break;
    case event::KeyCode::ARROW_RIGHT:
        m_char_index = std::min(m_char_index + 1, m_value.size());
        break;
    case event::KeyCode::DEL:
        remove_at(m_char_index);
        break;
    case event::KeyCode::BACKSPACE:
        if (m_char_index == 0) {
            break;
        }
        m_char_index = std::max<std::size_t>(m_char_index, 1) - 1;
        remove_at(m_char_index);
        break;
    case event::KeyCode::HOME:
        m_char_index = 0;
        break;
    case event::KeyCode::END:
        m_char_index = m_value.size();
        break;
    default:
        return false;
    }

    request_update();
    return true;
}

void Input::remove_at(std::size_t index) {
    if (index >= m_value.size()) {
        return;
    }

    m_value.erase(index, 1);

    if (m_char_index > m_value.size()) {
        m_char_index = m_value.size();
    }
    m_onchange.emit(m_value);
}

void Input::prepare_buffer() {
    using terminal::PixelStyle;
    using namespace std::string_view_literals;

    m_buffer.clear();

    auto max_width = std::min(m_buffer.width(), static_cast<unit_t>(m_value.size() + 1));

    constexpr auto bold = PixelStyle::from<PixelStyle::BOLD>();

    const auto& pallete = get_pallete();

    PixelStyle style;
    terminal::Color color = pallete.bright_black;

    if (has_focus()) {
        style = bold;
        color = pallete.foreground;
    }

    std::size_t offset = 0;
    if (m_char_index >= max_width) {
        offset = m_char_index - max_width;
    }

    for (unit_t i = 0; i <= max_width && i + offset <= m_value.size(); i++) {

        m_buffer.set_pixel_content(m_value[i + offset], i, 0);
        m_buffer.set_pixel_style(style, i, 0);
        m_buffer.set_pixel_foreground(color.id(), i, 0);
    }

    if (!has_focus()) {
        return;
    }

    if (m_char_index == m_value.size()) {
        m_buffer.set_pixel_content("█"sv, static_cast<unit_t>(m_char_index - offset), 0);
    } else {
        m_buffer.set_pixel_style(
            PixelStyle::from<PixelStyle::INVERSE>(), static_cast<unit_t>(m_char_index - offset), 0
        );
    }
}

void Input::calculate_requirements() {
    m_info.min_width  = 2;
    m_info.min_height = 1;
}

}
