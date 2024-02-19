#ifndef KOTERM_DOM_LAYOUT_DYNAMICLAYOUT_H
#define KOTERM_DOM_LAYOUT_DYNAMICLAYOUT_H

#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"

namespace koterm::dom::layout {

class DynamicLayout : public Element {
public:
    void add_element(element_t element);
    void add_elements(const element_container_t& elements);
    void remove_element(element_ref element);

    bool handle_event(const event::Event& event) override;
    bool has_child(element_ref element) const override;

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
