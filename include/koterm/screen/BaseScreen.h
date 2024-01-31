#ifndef KOTERM_TERMINAL_BASESCREEN_H
#define KOTERM_TERMINAL_BASESCREEN_H

#include "koterm/BoundingBox.h"
#include "koterm/dom/DomManager.h"
#include "koterm/dom/Element.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/unit.h"

namespace koterm::screen {

/**
 * @brief Base class for a screen with a buffer and viewport.
 */
class BaseScreen {
public:
    /**
     * @brief Retrieves the buffer.
     *
     * @return The buffer associated with the screen.
     */
    [[nodiscard]] const terminal::Buffer& buffer() const { return m_buffer; }

    /**
     * @brief Retrieves the viewport bounding box.
     *
     * @return The bounding box representing the viewport.
     */
    [[nodiscard]] const BoundingBox& viewport() const { return m_viewport; }

    /**
     * @brief Retrieves the width of the screen.
     *
     * @return The width of the screen.
     */
    [[nodiscard]] unit_t width() const { return m_buffer.width(); }

    /**
     * @brief Retrieves the height of the screen.
     *
     * @return The height of the screen.
     */
    [[nodiscard]] unit_t height() const { return m_buffer.height(); }

    /**
     * @brief Clears the buffer of the screen.
     */
    void clear() { m_buffer.clear(); }

    void set_document(const dom::element_t& element) { m_dom.set_root(element); }

    virtual ~BaseScreen() = default;

protected:
    /**
     * @brief Constructor for the BaseScreen class.
     *
     * @param width The initial width of the screen.
     * @param height The initial height of the screen.
     */
    BaseScreen(unit_t width, unit_t height)
        : m_viewport(width, height)
        , m_buffer(width, height) { }

    /**
     * @brief Resizes the screen with a new width and height.
     *
     * @param width The new width of the screen (must be greater than 0).
     * @param height The new height of the screen (must be greater than 0).
     */
    void resize(unit_t width, unit_t height) {
        m_dom.update_size(width, height);
        m_buffer.resize(width, height);
    }

    BoundingBox m_viewport;
    terminal::Buffer m_buffer;
    dom::DomManager m_dom;
};
}

#endif
