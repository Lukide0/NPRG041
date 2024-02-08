#ifndef KOTERM_DOM_LAYOUT_BOX_H
#define KOTERM_DOM_LAYOUT_BOX_H

#include "koterm/dom/Element.h"
#include "koterm/dom/layout/Layout.h"
#include "koterm/unit.h"
#include <cstdint>
#include <memory>

namespace koterm::dom::layout {

enum BoxDirection {
    HORIZONTAL,
    VERTICAL,
};

template <BoxDirection Direction> class Box : public Layout {
public:
    using box_t = std::shared_ptr<Box<Direction>>;

    static box_t create(const Element::BufferSpan& buffer, DomManager* manager);
    static box_t create(const Element::BufferSpan& buffer, DomManager* manager, const element_container_t& elements);

    void calculate_requirements() override;
    void update_layout() override;

    void set_gap(unit_t gap) {
        m_gap = gap;
        request_update();
    }
    [[nodiscard]] unit_t gap() const { return m_gap; }

private:
    using Layout::Layout;

    unit_t m_gap             = 0;
    unit_t m_shrink_min_size = 0;

    void update_layout_grow(unit_t min_size, unit_t target_size, unit_t grow);
    void update_layout_shrink(unit_t min_size, unit_t target_size, std::uint8_t shrink);
    void update_layout_shrink_force(unit_t target_size);
};

using VBox = Box<BoxDirection::VERTICAL>;
using HBox = Box<BoxDirection::HORIZONTAL>;

}

#endif
