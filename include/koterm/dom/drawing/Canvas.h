#ifndef KOTERM_DOM_DRAWING_CANVAS_H
#define KOTERM_DOM_DRAWING_CANVAS_H

#include "koterm/BoundingBox.h"
#include "koterm/dom/Element.h"
#include "koterm/screen/BaseScreen.h"
#include "koterm/terminal/Color.h"
#include "koterm/terminal/colors.h"
#include "koterm/unit.h"
#include <memory>
namespace koterm::dom::drawing {

class Canvas : public Element {
public:
    using color_t = terminal::Color;

    /**
     * @brief Struct representing the viewport of the canvas.
     */
    struct Viewport {
        unit_t width;
        unit_t height;
    };

    /**
     * @brief Creates a canvas with the given buffer and DOM manager.
     *
     * @param buffer The buffer span for the canvas.
     * @param manager The DOM manager for the canvas.
     * @return A shared pointer to the created canvas.
     */
    static std::shared_ptr<Canvas> create(const BufferSpan& buffer, DomManager* manager);

    /**
     * @brief Creates a canvas with the given screen.
     *
     * @param screen The base screen to create the canvas from.
     * @return A shared pointer to the created canvas.
     */
    static std::shared_ptr<Canvas> create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager());
    }

    /**
     * @brief Sets the viewport size of the canvas.
     *
     * @param width The width of the viewport.
     * @param height The height of the viewport.
     */
    void set_viewport(unit_t width, unit_t height) {
        m_view.width  = width;
        m_view.height = height;
    }

    /**
     * @brief Retrieves the current viewport size of the canvas.
     *
     * @return The viewport size of the canvas.
     */
    [[nodiscard]] Viewport get_viewport() const { return m_view; }

    /**
     * @brief Clears the entire canvas.
     */
    void clear() { m_buffer.clear(); }

    /**
     * @brief Clears a rectangle area on the canvas.
     *
     * @param width The width of the rectangle.
     * @param height The height of the rectangle.
     */
    void clear_rect(unit_t width, unit_t height) { m_buffer.clear_rect({ width, height }); }

    /**
     * @brief Clears a rectangle area on the canvas.
     *
     * @param left The left coordinate of the rectangle.
     * @param right The right coordinate of the rectangle.
     * @param top The top coordinate of the rectangle.
     * @param bottom The bottom coordinate of the rectangle.
     */
    void clear_rect(unit_t left, unit_t right, unit_t top, unit_t bottom) {
        m_buffer.clear_rect({ top, bottom, left, right });
    }

    /**
     * @brief Clears a rectangle area on the canvas.
     *
     * @param box The bounding box of the rectangle to clear.
     */
    void clear_rect(BoundingBox box) { m_buffer.clear_rect(box); }

    /**
     * @brief Forces rendering of the canvas.
     */
    void force_render() { request_render(); }

    /**
     * @brief Fills a rectangle area on the canvas with the specified color.
     *
     * @param box The bounding box of the rectangle to fill.
     * @param color The color to fill the rectangle with.
     * @return Reference to the canvas.
     */
    Canvas& fill_rect(BoundingBox box, color_t color = terminal::colors::SYS_DEFAULT);

    /**
     * @brief Strokes the border of a rectangle area on the canvas with the specified color.
     *
     * @param box The bounding box of the rectangle to stroke.
     * @param color The color to stroke the rectangle with.
     * @return Reference to the canvas.
     */
    Canvas& stroke_rect(BoundingBox, color_t color);

    /**
     * @brief Draws a line on the canvas between two points with the specified color.
     *
     * @param start The starting point of the line.
     * @param end The ending point of the line.
     * @param color The color of the line.
     * @return Reference to the canvas.
     */
    Canvas& line(point_t start, point_t end, color_t color);

    /*
     * @bried Draws a point
     *
     * @param p The point coord
     * @param color The color of the point
     */
    Canvas& dot(point_t p, color_t color);

    void calculate_requirements() override {
        m_info.min_width  = m_view.width * 2;
        m_info.min_height = m_view.height;
    }

private:
    using Element::Element;

    Viewport m_view;
};
}

#endif
