#ifndef KOTERM_DOM_DRAWING_CANVAS_H
#define KOTERM_DOM_DRAWING_CANVAS_H

#include "koterm/BoundingBox.h"
#include "koterm/component/Border.h"
#include "koterm/component/Signal.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/screen/BaseScreen.h"
#include "koterm/terminal/colors.h"
#include "koterm/unit.h"
#include <memory>
namespace koterm::dom::drawing {

class Canvas : public Element {
public:
    using signal_key_t         = component::Signal<event::KeyCode>;
    using signal_mouse_t       = component::Signal<const event::MouseEvent&>;
    using signal_special_key_t = component::Signal<Ascii>;
    using color_t              = terminal::Color::color_id;

    struct Viewport {
        unit_t width;
        unit_t height;
    };

    static std::shared_ptr<Canvas> create(const BufferSpan& buffer, DomManager* manager);
    static std::shared_ptr<Canvas> create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager());
    }

    void set_viewport(unit_t width, unit_t height) {
        m_view.width  = width;
        m_view.height = height;
    }

    [[nodiscard]] Viewport get_viewport() const { return m_view; }

    void clear() { m_buffer.clear(); }
    void clear_rect(unit_t width, unit_t height) { m_buffer.clear_rect({ width, height }); }
    void clear_rect(unit_t left, unit_t right, unit_t top, unit_t bottom) {
        m_buffer.clear_rect({ top, bottom, left, right });
    }
    void clear_rect(BoundingBox box) { m_buffer.clear_rect(box); }
    void force_render() { request_render(); }

    Canvas& fill_rect(BoundingBox box, color_t color = terminal::colors::SYS_DEFAULT);
    Canvas& stroke_rect(BoundingBox, color_t color);
    Canvas& line(point_t start, point_t end, color_t color);

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
