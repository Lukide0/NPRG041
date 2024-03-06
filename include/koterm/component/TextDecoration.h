#ifndef KOTERM_COMPONENT_TEXTDECORATION_H
#define KOTERM_COMPONENT_TEXTDECORATION_H

#include "koterm/terminal/Pixel.h"
namespace koterm::component {

class TextDecoration {
public:
    using Decoration = terminal::PixelStyle::StyleFlags;

    /**
     * @brief Sets a text decoration style.
     *
     * @param decoration The decoration style to set.
     * @return Reference to the current TextDecoration object.
     */
    TextDecoration& set(Decoration decoration) {
        switch (decoration) {
        case Decoration::BOLD:
            m_style.set<Decoration::BOLD>();
            break;
        case Decoration::DIM:
            m_style.set<Decoration::DIM>();
            break;
        case Decoration::ITALIC:
            m_style.set<Decoration::ITALIC>();
            break;
        case Decoration::UNDERLINE:
            m_style.set<Decoration::UNDERLINE>();
            break;
        case Decoration::BLINK:
            m_style.set<Decoration::BLINK>();
            break;
        case Decoration::INVERSE:
            m_style.set<Decoration::INVERSE>();
            break;
        case Decoration::HIDDEN:
            m_style.set<Decoration::HIDDEN>();
            break;
        case Decoration::STRIKETHROUGH:
            m_style.set<Decoration::STRIKETHROUGH>();
            break;
        }

        return *this;
    }

    /**
     * @brief Sets a specific text decoration style.
     *
     * @tparam D The decoration style to set.
     * @return Reference to the current TextDecoration object.
     */
    template <Decoration D> TextDecoration& set() {
        m_style.set<D>();
        return *this;
    }

    /**
     * @brief Unsets a text decoration style.
     *
     * @param decoration The decoration style to unset.
     * @return Reference to the current TextDecoration object.
     */
    TextDecoration& unset(Decoration decoration) {
        switch (decoration) {
        case Decoration::BOLD:
            m_style.unset<Decoration::BOLD>();
            break;
        case Decoration::DIM:
            m_style.unset<Decoration::DIM>();
            break;
        case Decoration::ITALIC:
            m_style.unset<Decoration::ITALIC>();
            break;
        case Decoration::UNDERLINE:
            m_style.unset<Decoration::UNDERLINE>();
            break;
        case Decoration::BLINK:
            m_style.unset<Decoration::BLINK>();
            break;
        case Decoration::INVERSE:
            m_style.unset<Decoration::INVERSE>();
            break;
        case Decoration::HIDDEN:
            m_style.unset<Decoration::HIDDEN>();
            break;
        case Decoration::STRIKETHROUGH:
            m_style.unset<Decoration::STRIKETHROUGH>();
            break;
        }

        return *this;
    }

    /**
     * @brief Unsets a specific text decoration style.
     *
     * @tparam D The decoration style to unset.
     * @return Reference to the current TextDecoration object.
     */
    template <Decoration D> TextDecoration& unset() {
        m_style.unset<D>();
        return *this;
    }

    /**
     * @brief Retrieves the current text decoration style.
     * @return The current text decoration style.
     */
    [[nodiscard]] terminal::PixelStyle style() const { return m_style; }

private:
    terminal::PixelStyle m_style;
};

}

#endif
