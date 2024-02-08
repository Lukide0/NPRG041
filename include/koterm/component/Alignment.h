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

    Alignment(Horizontal horizontal = Horizontal::LEFT, Vertical vertical = Vertical::TOP)
        : m_hor(horizontal)
        , m_vert(vertical) { }

    [[nodiscard]] Horizontal horizontal() const { return m_hor; }
    [[nodiscard]] Vertical vertical() const { return m_vert; }

    void set_horizontal(Horizontal horizontal) { m_hor = horizontal; }
    void set_vertical(Vertical vertical) { m_vert = vertical; }

private:
    Horizontal m_hor;
    Vertical m_vert;
};

}

#endif
