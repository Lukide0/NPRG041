#include <algorithm>
#include <cstdint>
#include <memory>

#include "koterm/dom/Element.h"
#include "koterm/dom/layout/Box.h"
#include "koterm/unit.h"

namespace koterm::dom::layout {

template class Box<BoxDirection::HORIZONTAL>;
template class Box<BoxDirection::VERTICAL>;

template <BoxDirection Direction>
Box<Direction>::box_t Box<Direction>::create(const Element::BufferSpan& buffer, DomManager* manager) {
    return std::make_shared<Box<Direction>>(buffer, manager);
}

template <BoxDirection Direction>
Box<Direction>::box_t
Box<Direction>::create(const Element::BufferSpan& buffer, DomManager* manager, const element_container_t& elements) {
    auto box = std::make_shared<Box<Direction>>(buffer, manager);
    box->add_elements(elements);
    return box;
}

template <BoxDirection Direction> void Box<Direction>::calculate_requirements() {
    unit_t min_height = 0;
    unit_t min_width  = 0;
    unit_t gaps_size  = 0;

    m_info.reset_children();
    m_shrink_min_size = 0;

    for (auto&& el : m_elements) {
        if (el->is_visible()) {
            el->update_requirements();

            const auto& info = el->requirements();

            if constexpr (Direction == BoxDirection::HORIZONTAL) {
                min_width += info.min_width;
                min_height = std::max(min_height, info.min_height);

                if (m_info.shrink_x == 0) {
                    m_shrink_min_size += info.min_width + m_gap;
                }

            } else {
                min_height += info.min_height;
                min_width = std::max(min_width, info.min_width);

                if (m_info.shrink_y == 0) {
                    m_shrink_min_size += info.min_height + m_gap;
                }
            }

            gaps_size += m_gap;

            m_info.grow_x_children += info.grow_x;
            m_info.grow_y_children += info.grow_y;

            m_info.shrink_y_children += info.shrink_y;
            m_info.shrink_x_children += info.shrink_x;
        }
    }

    // remove last gap
    if (gaps_size > 0) {
        gaps_size -= m_gap;
        if (m_shrink_min_size > 0) {
            m_shrink_min_size -= m_gap;
        }
    }

    m_info.min_height = min_height + gaps_size * static_cast<unit_t>(Direction == BoxDirection::VERTICAL);
    m_info.min_width  = min_width + gaps_size * static_cast<unit_t>(Direction == BoxDirection::HORIZONTAL);
}

template <BoxDirection Direction> void Box<Direction>::update_layout() {

    const unit_t min_size    = (Direction == BoxDirection::HORIZONTAL) ? m_info.min_width : m_info.min_height;
    const unit_t target_size = (Direction == BoxDirection::HORIZONTAL) ? width() : height();
    const unit_t grow   = (Direction == BoxDirection::HORIZONTAL) ? m_info.grow_x_children : m_info.grow_y_children;
    const unit_t shrink = (Direction == BoxDirection::HORIZONTAL) ? m_info.shrink_x_children : m_info.shrink_y_children;

    if (target_size >= min_size) {
        update_layout_grow(min_size, target_size, grow);
    } else if (target_size >= m_shrink_min_size) {
        update_layout_shrink(min_size, target_size, shrink);
    } else {
        update_layout_shrink_force(target_size);
    }
}

template <BoxDirection Direction>
void Box<Direction>::update_layout_grow(unit_t min_size, unit_t target_size, unit_t grow) {

    const unit_t extra = target_size - min_size;
    if (grow == 0) {
        grow = 1;
    }

    const auto& box = m_buffer.box();

    unit_t offset = (Direction == BoxDirection::HORIZONTAL) ? box.left : box.top;
    for (auto&& el : m_elements) {
        if (!el->is_visible()) {
            continue;
        }

        const auto& req = el->requirements();

        const unit_t size = (Direction == BoxDirection::HORIZONTAL) ? req.min_width + (req.grow_x * extra) / grow
                                                                    : req.min_height + (req.grow_y * extra) / grow;

        if constexpr (Direction == BoxDirection::HORIZONTAL) {
            el->handle_change_box({ box.top, box.bottom, offset, offset + size });
        } else {
            el->handle_change_box({ offset, offset + size, box.left, box.right });
        }

        offset += size + m_gap;
    }
}

template <BoxDirection Direction>
void Box<Direction>::update_layout_shrink(unit_t min_size, unit_t target_size, std::uint8_t shrink) {
    const unit_t overflow = min_size - target_size;

    if (shrink == 0) {
        shrink = 1;
    }

    const auto& box = m_buffer.box();
    unit_t offset   = (Direction == BoxDirection::HORIZONTAL) ? box.left : box.top;

    for (auto&& el : m_elements) {
        if (!el->is_visible()) {
            continue;
        }

        const auto& req        = el->requirements();
        const unit_t size      = (Direction == BoxDirection::HORIZONTAL) ? req.min_width : req.min_height;
        const unit_t el_shrink = (Direction == BoxDirection::HORIZONTAL) ? req.shrink_x : req.shrink_y;

        const unit_t new_size = size - (overflow * el_shrink) / shrink;
        // int overflow
        if (new_size > size) {
            update_layout_shrink_force(target_size);
            break;
        }

        if constexpr (Direction == BoxDirection::HORIZONTAL) {
            el->handle_change_box({ box.top, box.bottom, offset, offset + new_size });
        } else {
            el->handle_change_box({ offset, offset + new_size, box.left, box.right });
        }

        offset += size + m_gap;
    }
}

template <BoxDirection Direction> void Box<Direction>::update_layout_shrink_force(unit_t target_size) {
    const auto& box = m_buffer.box();

    const unit_t start = (Direction == BoxDirection::HORIZONTAL) ? box.left : box.top;
    unit_t offset      = start;
    for (auto&& el : m_elements) {
        if (!el->is_visible()) {
            continue;
        }

        const auto& req        = el->requirements();
        const unit_t size      = (Direction == BoxDirection::HORIZONTAL) ? req.min_width : req.min_height;
        const unit_t el_shrink = (Direction == BoxDirection::HORIZONTAL) ? req.shrink_x : req.shrink_y;

        if (el_shrink != 0 || offset + size > target_size + start) {
            el->handle_change_box({ 0, 0 });
            continue;
        }

        if constexpr (Direction == BoxDirection::HORIZONTAL) {
            el->handle_change_box({ box.top, box.bottom, offset, offset + size });
        } else {
            el->handle_change_box({ offset, offset + size, box.left, box.right });
        }

        offset += size + m_gap;
    }
}

}
