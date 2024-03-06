#ifndef KOTERM_BOUNDINGBOX_H
#define KOTERM_BOUNDINGBOX_H

#include "koterm/Dimensions.h"
#include "koterm/exceptions.h"
#include "koterm/unit.h"
#include "koterm/util/debug.h"
#include "koterm/util/range.h"
#include <algorithm>
namespace koterm {

/**
 * @brief BoundingBox structure representing a rectangular region.
 * @details The coordinate system is defined with its origin (0, 0) at the top-left corner.
 */
struct BoundingBox {

    /**
     * @brief Constructs a BoundingBox with specified coordinates.
     * @param start_y The y-coordinate of the top edge.
     * @param end_y The y-coordinate of the bottom edge.
     * @param start_x The x-coordinate of the left edge.
     * @param end_x The x-coordinate of the right edge.
     */
    constexpr BoundingBox(unit_t start_y, unit_t end_y, unit_t start_x, unit_t end_x)
        : top(start_y)
        , bottom(end_y)
        , left(start_x)
        , right(end_x) { }
    /**
     * @brief Constructs a BoundingBox with specified width and height.
     * @param width The width of the BoundingBox.
     * @param height The height of the BoundingBox.
     */
    constexpr BoundingBox(unit_t width, unit_t height)
        : top(0)
        , bottom(height)
        , left(0)
        , right(width) { }

    /**
     * @brief Constructs a BoundingBox from given dimensions.
     * @param dims The dimensions to construct the BoundingBox from.
     */
    constexpr BoundingBox(const Dimensions& dims)
        : top(0)
        , bottom(dims.height)
        , left(0)
        , right(dims.width) { }

    constexpr BoundingBox(BoundingBox&&)      = default;
    constexpr BoundingBox(const BoundingBox&) = default;

    unit_t top;
    unit_t bottom;
    unit_t left;
    unit_t right;

    /**
     * @brief Width of the BoundingBox.
     * @return The width calculated as (right - left).
     */
    [[nodiscard]] constexpr unit_t width() const { return right - left; }
    /**
     * @brief Height of the BoundingBox.
     * @return The height calculated as (bottom - top).
     */
    [[nodiscard]] constexpr unit_t height() const { return bottom - top; }

    /**
     * @brief Checks if the BoundingBox can be shrunk by the specified amount.
     * @param n The amount to shrink.
     * @return True if the BoundingBox can be shrunk, false otherwise.
     */
    [[nodiscard]] bool can_shrink(unit_t n = 1) const { return width() >= n && height() >= n; }

    /**
     * @brief Shrinks the BoundingBox by the specified amount.
     * @param n The amount to shrink.
     */
    void shrink(unit_t n = 1) {
        koterm_assert(
            can_shrink(n), exception::KotermException { "Cannot shrink bounding box" }, "Cannot shrink bounding box"
        );

        top += n;
        bottom -= n;
        left += n;
        right -= n;
    }

    /**
     * @brief Checks if two BoundingBoxes intersect.
     *
     * @param a The first BoundingBox.
     * @param b The second BoundingBox.
     * @return True if there is an intersection, false otherwise.
     */
    static constexpr bool intersection(const BoundingBox& a, const BoundingBox& b) {
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

    /**
     * @brief Calculates the overlapping BoundingBox between two BoundingBoxes.
     * @param a The first BoundingBox.
     * @param b The second BoundingBox.
     * @return The overlapping BoundingBox if it exists, an empty BoundingBox otherwise.
     */
    [[nodiscard]] static constexpr BoundingBox overlap(const BoundingBox& a, const BoundingBox& b) {

        if (!intersection(a, b)) {
            return { 0, 0, 0, 0 };
        }

        const unit_t left   = std::max(a.left, b.left);
        const unit_t right  = std::min(a.right, b.right);
        const unit_t top    = std::max(a.top, b.top);
        const unit_t bottom = std::min(a.bottom, b.bottom);

        return { top, bottom, left, right };
    }

    constexpr BoundingBox& operator=(const BoundingBox& box) = default;
    constexpr bool operator==(const BoundingBox& box) const {
        return box.left == left && box.right == right && box.top == top && box.bottom == bottom;
    }
};

}

#endif
