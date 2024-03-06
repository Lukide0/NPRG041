#ifndef KOTERM_DOM_INPUT_RADIOBOX_H
#define KOTERM_DOM_INPUT_RADIOBOX_H

#include "koterm/BoundingBox.h"
#include "koterm/component/Signal.h"
#include "koterm/component/Text.h"
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
#include <utility>
#include <vector>

namespace koterm::dom::input {

/**
 * @brief Represents a group of radio buttons for selecting a single option from a set of choices.
 *
 * This class extends the functionality of the Element class to provide a group of radio buttons,
 * where only one option can be selected at a time.
 *
 * @tparam T The type of value associated with each option.
 */
template <typename T> class Radiobox : public Element {
public:
    using signal_t = component::Signal<T>;

    struct Option {
        component::Text label;
        T value;
    };

    /**
     * @brief Creates a new Radiobox instance with the specified options.
     *
     * @param buffer The buffer to render the radiobox onto.
     * @param manager The DOM manager.
     * @param options The vector of options for the radiobox.
     * @return A shared pointer to the newly created Radiobox instance.
     */
    static std::shared_ptr<Radiobox<T>>
    create(const BufferSpan& buffer, DomManager* manager, std::vector<Option>&& options) {
        auto el         = std::make_shared<Radiobox<T>>(buffer, manager, true, false, true);
        el->m_options   = std::move(options);
        el->m_max_width = max_width(options);
        return el;
    }

    /**
     * @brief Creates a new Radiobox instance with the specified options.
     *
     * @param buffer The buffer to render the radiobox onto.
     * @param manager The DOM manager.
     * @param options The vector of options for the radiobox.
     * @return A shared pointer to the newly created Radiobox instance.
     */
    static std::shared_ptr<Radiobox<T>>
    create(const BufferSpan& buffer, DomManager* manager, const std::vector<Option>& options) {
        auto el         = std::make_shared<Radiobox<T>>(buffer, manager, true, false, true);
        el->m_options   = options;
        el->m_max_width = max_width(options);
        return el;
    }

    /**
     * @brief Creates a new Radiobox instance with the specified options.
     *
     * @param screen The BaseScreen to create the radiobox on.
     * @param options The vector of options for the radiobox.
     * @return A shared pointer to the newly created Radiobox instance.
     */
    static std::shared_ptr<Radiobox<T>> create(screen::BaseScreen* screen, std::vector<Option>&& options) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager(), std::move(options));
    }

    /**
     * @brief Creates a new Radiobox instance with the specified options.
     *
     * @param screen The BaseScreen to create the radiobox on.
     * @param options The vector of options for the radiobox.
     * @return A shared pointer to the newly created Radiobox instance.
     */
    static std::shared_ptr<Radiobox<T>> create(screen::BaseScreen* screen, const std::vector<Option>& options) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager(), options);
    }

    /**
     * @brief Handles key events for the radiobox.
     *
     * @param key The key event that occurred.
     * @return True if the key event was handled, false otherwise.
     */
    bool handle_key(event::KeyCode key) override;

    /**
     * @brief Handles mouse click events for the radiobox.
     *
     * @param event The mouse click event that occurred.
     * @return True if the mouse click event was handled, false otherwise.
     */
    bool handle_mouse_click(const event::MouseEvent& event) override;

    /**
     * @brief Prepares the radiobox for rendering.
     *
     * This function prepares the radiobox for rendering by updating its buffer contents based on
     * its current state and properties.
     */
    void prepare_buffer() override;
    void calculate_requirements() override;

    /**
     * @brief Accessor for the onChange signal of the radiobox.
     *
     * This signal is triggered when the selection in the radiobox changes.
     * @return Reference to the onChange signal of the radiobox.
     */
    signal_t& on_change() { return m_onchange; }

    /**
     * @brief Accessor for the options of the radiobox.
     *
     * @return Const reference to the vector of options in the radiobox.
     */
    const std::vector<Option>& options() const { return m_options; }

    /**
     * @brief Checks if the radiobox has a selected value.
     *
     * @return True if the radiobox has a selected value, false otherwise.
     */
    [[nodiscard]] bool has_value() const { return !m_options.empty(); }

    /**
     * @brief Retrieves the currently selected value of the radiobox.
     *
     * @return Const reference to the currently selected value of the radiobox.
     */
    [[nodiscard]] const T& value() const { return m_options[m_selected].value; }

    /**
     * @brief Selects the option at the specified index.
     *
     * @param index The index of the option to select.
     */
    void select(std::size_t index = 0) {
        std::size_t tmp = std::min<std::size_t>(index, std::max<std::size_t>(m_options.size(), 1) - 1);
        if (tmp != m_selected) {
            m_selected    = tmp;
            m_highlighted = tmp;
            m_onchange.emit(value());
            request_render();
        }
    }

    /**
     * @brief Highlights the option at the specified index.
     *
     * @param index The index of the option to highlight.
     */
    void highlight(std::size_t index = 0) {
        std::size_t tmp = std::min<std::size_t>(index, std::max<std::size_t>(m_options.size(), 1) - 1);
        if (tmp != m_highlighted) {
            m_highlighted = tmp;
            request_render();
        }
    }

private:
    using Element::Element;

    /**
     * @brief Calculates the maximum width among the labels of the radiobox options.
     *
     * @param options The vector of options for the radiobox.
     * @return The maximum width among the option labels.
     */
    static std::size_t max_width(const std::vector<Option>& options) {
        return std::max_element(
                   options.begin(),
                   options.end(),
                   [](const Option& a, const Option& b) -> bool { return a.label.size() < b.label.size(); }
        )->label.size();
    }

    std::vector<Option> m_options;
    std::size_t m_max_width   = 0;
    std::size_t m_selected    = 0;
    std::size_t m_highlighted = 0;
    signal_t m_onchange;
};

template <typename T> bool Radiobox<T>::handle_key(event::KeyCode key) {
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

template <typename T> bool Radiobox<T>::handle_mouse_click(const event::MouseEvent& event) {

    auto pos = event.pos();
    if (is_mouse_inside(pos) && event.btn_pressed<event::MouseEvent::Btn::LEFT>()) {
        focus();

        std::size_t selected = pos.y - m_buffer.box().top - 1;
        if (selected != m_selected && m_options.size() > selected) {
            highlight(selected);
        }
        return true;
    } else if (has_focus() && event.btn_released<event::MouseEvent::Btn::LEFT>()) {
        select(m_highlighted);
        blur();
        return true;
    }

    return false;
}

template <typename T> void Radiobox<T>::prepare_buffer() {
    using namespace std::string_view_literals;

    m_buffer.clear();

    BoundingBox box = m_buffer.box();
    box.left += 2;

    terminal::BufferSpan text_span { m_buffer.buffer() };

    for (std::size_t i = 0; i < m_options.size(); i++) {

        if (i == m_selected) {
            m_buffer.set_pixel_content("◉"sv, 0, i);
            m_buffer.set_pixel_content(' ', 1, i);
        } else {
            m_buffer.set_pixel_content("○"sv, 0, i);
            m_buffer.set_pixel_content(' ', 1, i);
        }

        BoundingBox text_box = box;
        text_box.top         = i;
        text_box.bottom      = i;

        text_span.resize(text_box);
        m_options[i].label.render(text_span);

        if (has_focus() && i == m_highlighted) {
            for (unit_t x = 2; x < m_buffer.width(); x++) {
                terminal::PixelStyle style;
                style.set<terminal::PixelStyle::StyleFlags::INVERSE>();
                m_buffer.set_pixel_style(style, x, m_highlighted);
            }
        }
    }

    for (unit_t i = 0; i < m_buffer.width(); i++) {
        terminal::PixelStyle style = m_buffer.get_pixel_style(i, m_selected);
        style.set<terminal::PixelStyle::StyleFlags::BOLD>();
        m_buffer.set_pixel_style(style, i, m_selected);
    }
}

template <typename T> void Radiobox<T>::calculate_requirements() {

    m_info.min_width  = m_max_width + 2;
    m_info.min_height = m_options.size();
}

}

#endif
