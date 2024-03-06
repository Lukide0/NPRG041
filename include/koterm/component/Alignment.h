#ifndef KOTERM_COMPONENT_ALIGNMENT_H
#define KOTERM_COMPONENT_ALIGNMENT_H

#include <cstdint>
namespace koterm::component {

class Alignment {
public:
    enum class Horizontal : std::uint8_t {
        LEFT   = 0,
        CENTER = 1,
        RIGHT  = 2,
    };

    enum class Vertical : std::uint8_t {
        TOP    = 0,
        CENTER = 1,
        BOTTOM = 2,
    };

    /**
     * @brief Constructs an Alignment object with the given horizontal and vertical alignment.
     * @param horizontal The horizontal alignment.
     * @param vertical The vertical alignment.
     */
    Alignment(Horizontal horizontal = Horizontal::LEFT, Vertical vertical = Vertical::TOP)
        : m_hor(horizontal)
        , m_vert(vertical) { }

    /**
     * @brief Returns the horizontal alignment.
     * @return The horizontal alignment.
     */
    [[nodiscard]] Horizontal horizontal() const { return m_hor; }
    /**
     * @brief Returns the vertical alignment.
     * @return The vertical alignment.
     */
    [[nodiscard]] Vertical vertical() const { return m_vert; }

    /**
     * @brief Sets the horizontal alignment.
     * @param horizontal The horizontal alignment to set.
     */
    void set_horizontal(Horizontal horizontal) { m_hor = horizontal; }

    /**
     * @brief Sets the vertical alignment.
     * @param vertical The vertical alignment to set.
     */
    void set_vertical(Vertical vertical) { m_vert = vertical; }

private:
    Horizontal m_hor;
    Vertical m_vert;
};

}

#endif
