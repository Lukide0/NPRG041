#ifndef KOTERM_DOM_LAYOUT_BOX_H
#define KOTERM_DOM_LAYOUT_BOX_H

#include "koterm/dom/Element.h"
#include "koterm/dom/layout/DynamicLayout.h"
#include "koterm/screen/BaseScreen.h"
#include "koterm/unit.h"
#include <cstdint>
#include <memory>

namespace koterm::dom::layout {

enum BoxDirection {
    HORIZONTAL,
    VERTICAL,
};

/**
 * @brief Represents a container that lays out its child elements either horizontally or vertically.
 *
 * @tparam Direction The direction in which the child elements are laid out (HORIZONTAL or VERTICAL).
 */
template <BoxDirection Direction> class Box : public DynamicLayout {
public:
    /**
     * @brief Creates a new Box instance with the specified buffer and DOM manager.
     *
     * @param buffer The buffer to render the box onto.
     * @param manager The DOM manager.
     * @return A shared pointer to the newly created Box instance.
     */
    static std::shared_ptr<Box<Direction>> create(const Element::BufferSpan& buffer, DomManager* manager);

    /**
     * @brief Creates a new Box instance with the specified buffer, DOM manager, and child elements.
     *
     * @param buffer The buffer to render the box onto.
     * @param manager The DOM manager.
     * @param elements The vector of child elements to be added to the box.
     * @return A shared pointer to the newly created Box instance.
     */
    static std::shared_ptr<Box<Direction>>
    create(const Element::BufferSpan& buffer, DomManager* manager, const element_container_t& elements);

    /**
     * @brief Creates a new Box instance with the buffer from the specified screen and its DOM manager.
     *
     * @param screen The BaseScreen to create the box on.
     * @return A shared pointer to the newly created Box instance.
     */
    static std::shared_ptr<Box<Direction>> create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager());
    }

    /**
     * @brief Creates a new Box instance with the buffer from the specified screen, its DOM manager,
     * and the provided child elements.
     *
     * @param screen The BaseScreen to create the box on.
     * @param elements The vector of child elements to be added to the box.
     * @return A shared pointer to the newly created Box instance.
     */
    static std::shared_ptr<Box<Direction>> create(screen::BaseScreen* screen, const element_container_t& elements) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager(), elements);
    }

    void calculate_requirements() override;
    void update_layout() override;

    /**
     * @brief Sets the gap between child elements in the box.
     *
     * @param gap The gap between child elements.
     */
    void set_gap(unit_t gap) {
        m_gap = gap;
        request_update();
    }

    /**
     * @brief Retrieves the current gap between child elements in the box.
     *
     * @return The gap between child elements.
     */
    [[nodiscard]] unit_t gap() const { return m_gap; }

private:
    using DynamicLayout::DynamicLayout;

    unit_t m_gap             = 0;
    unit_t m_shrink_min_size = 0;

    void update_layout_grow(unit_t min_size, unit_t target_size, std::uint32_t grow);
    void update_layout_shrink(unit_t min_size, unit_t target_size, std::uint32_t shrink);
    void update_layout_shrink_force(unit_t target_size);
};

/**
 * @brief Alias for a vertical box layout container.
 *
 * The VBox alias represents a Box container with vertical layout direction. It arranges its child elements
 * vertically, stacking them from top to bottom.
 */
using VBox = Box<BoxDirection::VERTICAL>;

/**
 * @brief Alias for a horizontal box layout container.
 *
 * The HBox alias represents a Box container with horizontal layout direction. It arranges its child elements
 * horizontally, laying them out from left to right.
 */
using HBox = Box<BoxDirection::HORIZONTAL>;

}

#endif
