#ifndef KOTERM_DOM_TEXT_H
#define KOTERM_DOM_TEXT_H

#include "koterm/component/Alignment.h"
#include "koterm/component/Text.h"
#include "koterm/component/TextDecoration.h"
#include "koterm/dom/Element.h"
#include "koterm/screen/BaseScreen.h"
#include "koterm/terminal/Color.h"
#include "koterm/terminal/colors.h"
#include "koterm/unit.h"
#include <memory>
#include <string>
#include <utility>

namespace koterm::dom {

class Text : public Element {
public:
    using Alignment      = component::Alignment;
    using TextDecoration = component::TextDecoration;

    /**
     * @brief Creates a Text element.
     * @param screen Pointer to the screen where the text will be rendered.
     * @return A shared pointer to the created Text element.
     */
    static std::shared_ptr<Text> create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer(), screen->viewport() }, &screen->dom_manager());
    }

    /**
     * @brief Creates a Text element.
     * @param buffer The buffer where the text will be rendered.
     * @param manager Pointer to the DOM manager.
     * @return A shared pointer to the created Text element.
     */
    static std::shared_ptr<Text> create(const BufferSpan& buffer, DomManager* manager);

    ~Text() override = default;

    void prepare_buffer() override;

    /**
     * @brief Gets the alignment of the text.
     * @return The alignment of the text.
     */
    [[nodiscard]] Alignment aligment() const { return m_text.alignment(); }

    /**
     * @brief Modifies the alignment of the text.
     * @return A reference to the alignment to be modified.
     */
    Alignment& modify_alignment() {
        request_update();
        return m_text.modify_alignment();
    }

    /**
     * @brief Gets the decoration of the text.
     * @return The decoration of the text.
     */
    [[nodiscard]] TextDecoration decoration() const { return m_text.decoration(); }

    /**
     * @brief Modifies the decoration of the text.
     * @return A reference to the decoration to be modified.
     */
    TextDecoration& modify_decoration() {
        request_render();
        return m_text.modify_decoration();
    }

    /**
     * @brief Gets the text content.
     * @return The text content.
     */
    [[nodiscard]] const std::string& text() const { return m_text.text(); }

    /**
     * @brief Sets the text content.
     * @param text The new text content.
     */
    void set_text(std::string&& text) {
        m_text.set_text(std::move(text));
        request_update();
    }

    /**
     * @brief Sets the text content.
     * @param text The new text content.
     */
    void set_text(const std::string& text) {
        m_text.set_text(text);
        request_update();
    }

    /**
     * @brief Sets the foreground color of the text.
     * @param color The foreground color.
     */
    void set_foreground(terminal::Color color) {
        request_render();
        m_foreground = color;
    }

    /**
     * @brief Sets the background color of the text.
     * @param color The background color.
     */
    void set_background(terminal::Color color) {
        request_render();
        m_background = color;
    }

    void calculate_requirements() override {
        m_info.min_width  = static_cast<unit_t>(m_text.size());
        m_info.min_height = 1;
    }

private:
    using Element::Element;

    terminal::Color m_foreground = terminal::colors::SYS_DEFAULT;
    terminal::Color m_background = terminal::colors::SYS_DEFAULT;
    component::Text m_text;
};

}

#endif
