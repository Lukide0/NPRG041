#ifndef KOTERM_EVENT_EVENTSENDER_H
#define KOTERM_EVENT_EVENTSENDER_H

#include "koterm/event/EventListener.h"

namespace koterm::event {

/**
 * @brief Generic template class for event senders.
 *
 * @tparam T Type of the events to be handled.
 */
template <typename T> class EventSender {
public:
    using listener_t = EventListener<T>;
    using event_t    = T;

    /**
     * @brief Sends an event to the associated listener.
     *
     * @param event The event to be sent.
     */
    void send(const event_t& event) { m_listener->send(event); }

    /**
     * @brief Sends an event to the associated listener.
     *
     * @param event The event to be sent.
     */
    void send(event_t&& event) { m_listener->send(std::move(event)); }

    /**
     * @brief Creates a duplicate sender associated with the same listener.
     *
     * @return A duplicate sender instance.
     */
    [[nodiscard]] EventSender<T> duplicate() { return m_listener->make_sender(); }

    ~EventSender() { m_listener->remove_sender(); }

private:
    friend listener_t;

    EventSender(listener_t* listener)
        : m_listener(listener) { }

    listener_t* m_listener;
};

}

#endif
