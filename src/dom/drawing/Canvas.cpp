#include "koterm/dom/drawing/Canvas.h"
#include "koterm/BoundingBox.h"
#include "koterm/dom/DomManager.h"
#include "koterm/terminal/Buffer.h"
#include "koterm/unit.h"
#include <memory>

namespace koterm::dom::drawing {

std::shared_ptr<Canvas> Canvas::create(const BufferSpan& buffer, DomManager* manager) {
    return std::make_shared<Canvas>(buffer, manager, false, true, true);
}

Canvas& Canvas::fill_rect(BoundingBox box, color_t color) {

    BoundingBox this_box = m_buffer.box();

    this_box.left /= 2;
    this_box.right /= 2;

    box = BoundingBox::overlap(box, this_box);

    for (unit_t y = box.top; y < box.bottom; y++) {
        for (unit_t x = box.left; x < box.right * 2; x++) {
            m_buffer.set_pixel_background(color, x, y);
        }
    }

    return *this;
}

Canvas& Canvas::stroke_rect(BoundingBox box, color_t color) {

    BoundingBox this_box = m_buffer.box();

    this_box.left /= 2;
    this_box.right /= 2;

    box = BoundingBox::overlap(box, this_box);

    if (box.width() == 0 || box.height() == 0) {
        return *this;
    }

    for (unit_t x = box.left; x < box.right * 2; x++) {
        m_buffer.set_pixel_background(color, x, box.top);
        m_buffer.set_pixel_background(color, x, box.bottom - 1);
    }

    for (unit_t y = box.top; y < box.bottom; y++) {

        m_buffer.set_pixel_background(color, box.left * 2, y);
        m_buffer.set_pixel_background(color, box.left * 2 + 1, y);
        m_buffer.set_pixel_background(color, box.right * 2 - 1, y);
        m_buffer.set_pixel_background(color, box.right * 2 - 2, y);
    }

    return *this;
}

Canvas& Canvas::dot(point_t p, color_t color) {
    const auto& box = m_buffer.box();

    if (box.contains(p.x * 2, p.y)) {
        m_buffer.set_pixel_background(color, p.x * 2, p.y);
        m_buffer.set_pixel_background(color, p.x * 2 + 1, p.y);
    }

    return *this;
}

Canvas& Canvas::line(point_t start, point_t end, color_t color) {

    int dx;
    int step_x;

    start.x *= 2;
    end.x *= 2;

    if (start.x >= end.x) {
        dx     = static_cast<int>(start.x - end.x);
        step_x = -1;
    } else {
        dx     = static_cast<int>(end.x - start.x);
        step_x = 1;
    }

    int dy;
    int step_y;
    if (start.y >= end.y) {
        dy     = -static_cast<int>(start.y - end.y);
        step_y = -1;
    } else {
        dy     = -static_cast<int>(end.y - start.y);
        step_y = 1;
    }

    int err       = dx + dy;
    point_t begin = start;

    BoundingBox box = m_buffer.box();
    while (begin != end) {

        if (box.contains(begin.x, begin.y)) {
            m_buffer.set_pixel_background(color, begin.x, begin.y);
        }

        const int err2 = 2 * err;

        if (err2 >= dy) {
            err += dy;
            begin.x += step_x;
        }

        if (err2 <= dx) {
            err += dx;
            begin.y += step_y;
        }
    }

    return *this;
}

}
