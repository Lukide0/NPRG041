#ifndef KOTERM_DOM_LAYOUT_LAYOUT_H
#define KOTERM_DOM_LAYOUT_LAYOUT_H

#include "koterm/BoundingBox.h"
#include "koterm/Dimensions.h"
#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include "koterm/unit.h"
#include <span>
namespace koterm::dom::layout {

class Layout : public Element {
public:
    void add_element(element_t element);
    void add_elements(const element_container_t& elements);
    void remove_element(element_ref element);

    void handle_event(const event::Event& event) override;

    virtual void update_layout() = 0;

protected:
    using Element::Element;

    element_map_t m_elements;

private:
    void prepare_buffer() override;
    void remove_child(element_ref child) override;
};

}

#endif
