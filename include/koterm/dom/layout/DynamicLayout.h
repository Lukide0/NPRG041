#ifndef KOTERM_DOM_LAYOUT_DYNAMICLAYOUT_H
#define KOTERM_DOM_LAYOUT_DYNAMICLAYOUT_H

#include "koterm/dom/Element.h"
#include "koterm/event/Event.h"
#include <cstddef>

namespace koterm::dom::layout {

class DynamicLayout : public Element {
public:
    /**
     * @brief Adds a single element to the layout container.
     *
     * @param element Pointer to the element to be added.
     */
    void add_element(element_t element);

    /**
     * @brief Adds multiple elements to the layout container.
     *
     * @param elements Container holding pointers to the elements to be added.
     */
    void add_elements(const element_container_t& elements);

    /**
     * @brief Removes an element from the layout container.
     *
     * @param element Pointer to the element to be removed.
     */
    void remove_element(element_ref element);

    bool handle_event(const event::Event& event) override;
    bool has_child(element_ref element) const override;

    /**
     * @brief Updates the layout of the container.
     *
     * This method is pure virtual and must be implemented by subclasses to define the specific layout algorithm.
     */
    virtual void update_layout() = 0;

protected:
    using Element::Element;

    element_container_t m_elements;
    std::size_t m_focus = 0;

private:
    void prepare_buffer() override;
    void remove_child(element_ref child) override;
};

}

#endif
