#ifndef KOTERM_TERMINAL_BOUNDINGBOX_H
#define KOTERM_TERMINAL_BOUNDINGBOX_H

#include "koterm/terminal/unit.h"
#include "koterm/util/range.h"
namespace koterm::terminal {

/**
 * @brief BoundingBox structure representing a rectangular region.
 * @details The coordinate system is defined with its origin (0, 0) at the top-left corner.
 */
struct BoundingBox {

    constexpr BoundingBox(unit_t start_y, unit_t end_y, unit_t start_x, unit_t end_x)
        : top(start_y)
        , bottom(end_y)
        , left(start_x)
        , right(end_x) { }

    constexpr BoundingBox(unit_t width, unit_t height)
        : top(0)
        , bottom(height)
        , left(0)
        , right(width) { }

    unit_t top;
    unit_t bottom;
    unit_t left;
    unit_t right;

    /**
     * @brief Checks if two BoundingBoxes intersect.
     *
     * @param a The first BoundingBox.
     * @param b The second BoundingBox.
     * @return True if there is an intersection, false otherwise.
     */
    static constexpr bool intersection(BoundingBox a, BoundingBox b) {
        // check for overlap along the x-axis
        if (a.right < b.left || b.right < a.left) {
            return false;
        }

        // check for overlap along the y-axis
        if (a.bottom < b.top || b.bottom < a.top) {
            return false;
        }

        return true;
    }

    /**
     * @brief Checks if the specified coordinates are within the BoundingBox.
     *
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     * @return True if (x, y) is inside the BoundingBox, false otherwise.
     */
    [[nodiscard]] constexpr bool contains(unit_t x, unit_t y) const {
        return util::range::inside(x, left, right) && util::range::inside(y, top, bottom);
    }
};

}

#endif
