#ifndef KOTERM_DOM_INPUT_TOGGLE_H
#define KOTERM_DOM_INPUT_TOGGLE_H

#include "koterm/BoundingBox.h"
#include "koterm/component/Signal.h"
#include "koterm/component/Text.h"
#include "koterm/component/TextDecoration.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/screen/BaseScreen.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/terminal/Pixel.h"
#include "koterm/unit.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <numeric>
#include <utility>
#include <vector>

namespace koterm::dom::input {

template <typename T> class Toggle : public Element {
public:
    using signal_t = component::Signal<T>;

    struct Option {
        component::Text label;
        T value;
    };

    static std::shared_ptr<Toggle<T>>
    create(const BufferSpan& buffer, DomManager* manager, std::vector<Option>&& options) {
        auto el         = std::make_shared<Toggle<T>>(buffer, manager, true);
        el->m_options   = std::move(options);
        el->m_sum_width = max_width(options);
        return el;
    }
    static std::shared_ptr<Toggle<T>>
    create(const BufferSpan& buffer, DomManager* manager, const std::vector<Option>& options) {
        auto el         = std::make_shared<Toggle<T>>(buffer, manager, true);
        el->m_options   = options;
        el->m_sum_width = sum_width(options);
        return el;
    }

    static std::shared_ptr<Toggle<T>> create(screen::BaseScreen* screen, std::vector<Option>&& options) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager(), std::forward(options));
    }

    static std::shared_ptr<Toggle<T>> create(screen::BaseScreen* screen, const std::vector<Option>& options) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager(), options);
    }

    bool handle_key(event::KeyCode key) override;
    bool handle_mouse_click(const event::MouseEvent& event) override;

    void prepare_buffer() override;
    void calculate_requirements() override;

    signal_t& on_change() { return m_onchange; }

    const std::vector<Option>& options() const { return m_options; }

    [[nodiscard]] bool has_value() const { return !m_options.empty(); }
    [[nodiscard]] const T& value() const { return m_options[m_selected].value; }
    void select(std::size_t index = 0) {
        std::size_t tmp = std::min<std::size_t>(index, std::max<std::size_t>(m_options.size(), 1) - 1);
        if (tmp != m_selected) {
            m_selected    = tmp;
            m_highlighted = tmp;
            m_onchange.emit(value());
            request_render();
        }
    }

    void highlight(std::size_t index = 0) {
        std::size_t tmp = std::min<std::size_t>(index, std::max<std::size_t>(m_options.size(), 1) - 1);
        if (tmp != m_highlighted) {
            m_highlighted = tmp;
            request_render();
        }
    }

private:
    using Element::Element;

    static std::size_t sum_width(const std::vector<Option>& options) {
        return std::accumulate(options.begin(), options.end(), 0, [](int sum, const Option& item) -> bool {
            return sum + item.label.size();
        });
    }

    std::vector<Option> m_options;
    std::size_t m_sum_width   = 0;
    std::size_t m_selected    = 0;
    std::size_t m_highlighted = 0;
    signal_t m_onchange;
};

template <typename T> bool Toggle<T>::handle_key(event::KeyCode key) {
    if (!has_focus()) {
        return false;
    }

    switch (key) {
    case event::KeyCode::ARROW_UP:
        highlight(std::max<std::size_t>(1, m_highlighted) - 1);
        return true;
    case event::KeyCode::ARROW_DOWN:
        highlight(m_highlighted + 1);
        return true;
    case event::KeyCode::ENTER:
        select(m_highlighted);
        return true;
    default:
        return false;
    }
}

template <typename T> bool Toggle<T>::handle_mouse_click(const event::MouseEvent& event) {

    auto pos = event.pos();
    if (is_mouse_inside(pos) && event.btn_pressed<event::MouseEvent::Btn::LEFT>()) {
        focus();

        std::size_t pos_x = pos.x - m_buffer.box().left - 1;
        std::size_t curr  = 0;
        for (std::size_t i = 0; i < m_options.size(); i++) {
            const auto& opt = m_options[i];

            curr += opt.label.size();
            if (curr >= pos_x) {
                highlight(i);
                break;
            }

            curr += 1;
        }

        return true;
    } else if (has_focus() && event.btn_released<event::MouseEvent::Btn::LEFT>()) {
        select(m_highlighted);
        blur();
        return true;
    }

    return false;
}

template <typename T> void Toggle<T>::prepare_buffer() {
    using namespace std::string_view_literals;

    m_buffer.clear();

    BoundingBox box = m_buffer.box();
    terminal::BufferSpan text_span { m_buffer.buffer() };

    for (std::size_t i = 0; i < m_options.size(); i++) {

        if (i != 0) {
            m_buffer.set_pixel_content('|', box.left, 0);
            m_buffer.set_pixel_style(terminal::PixelStyle::from<terminal::PixelStyle::PixelStyle::BOLD>(), box.left, 0);
            box.left += 1;
        }

        BoundingBox text_box = box;
        text_span.resize(text_box);

        Option& opt = m_options[i];

        component::TextDecoration decoration = opt.label.decoration();
        if (i == m_selected) {
            opt.label.modify_decoration().set(component::TextDecoration::Decoration::BOLD);
        }

        if (has_focus() && i == m_highlighted) {
            opt.label.modify_decoration().set(component::TextDecoration::Decoration::INVERSE);
            opt.label.render(text_span);
        }

        opt.label.render(text_span);
        opt.label.set_decoration(decoration);

        box.left += opt.label.size();
    }
}

template <typename T> void Toggle<T>::calculate_requirements() {
    m_info.min_width  = m_sum_width + std::max<std::size_t>(m_options.size(), 1) - 1;
    m_info.min_height = 1;
}

}

#endif
