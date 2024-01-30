#ifndef KOTERM_EVENT_EVENTLISTENER_H
#define KOTERM_EVENT_EVENTLISTENER_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace koterm::event {

template <typename T> class EventSender;

/**
 * @brief Generic template class for event listeners.
 *
 * @tparam T Type of the events to be handled.
 */
template <typename T> class EventListener {
public:
    using sender_t = EventSender<T>;
    using event_t  = T;

    /**
     * @brief Creates a new sender for this listener.
     *
     * @return Sender instance.
     */
    sender_t make_sender() {
        std::unique_lock lock(m_mutex);

        m_senders_count += 1;

        return sender_t { this };
    }

    /**
     * @brief Removes a sender from this listener.
     */
    void remove_sender() {
        std::unique_lock lock(m_mutex);
        m_senders_count -= 1;

        lock.unlock();
        m_cv.notify_one();
    }

    /**
     * @brief Sends an event to this listener.
     *
     * @param event The event to be sent.
     */
    void send(const event_t& event) {
        std::unique_lock lock(m_mutex);
        m_queue.push(event);
        lock.unlock();

        m_cv.notify_one();
    }

    /**
     * @brief Sends an event to this listener.
     *
     * @param event The event to be sent.
     */
    void send(event_t&& event) {
        std::unique_lock lock(m_mutex);
        m_queue.push(std::move(event));

        lock.unlock();
        m_cv.notify_one();
    }

    /**
     * @brief Checks if the listener has any pending events.
     *
     * @return True if there aren't pending events, false otherwise.
     */
    [[nodiscard]] bool empty() {
        std::unique_lock lock(m_mutex);
        return m_queue.empty();
    }

    /**
     * @brief Checks if the listener has any pending events.
     *
     * @return True if there are pending events, false otherwise.
     */
    [[nodiscard]] bool has_pending() { return !empty(); }

    /**
     * @brief Waits for a pending event and retrieves it.
     *
     * @param out The variable to store the retrieved event.
     * @return True if an event was retrieved, false otherwise.
     */
    bool get_blocking(event_t& out) {
        event_t* out_ptr = std::addressof(out);

        while (m_senders_count > 0 || !m_queue.empty()) {
            std::unique_lock lock(m_mutex);
            // no pending events
            if (m_queue.empty()) {
                // try to wait
                m_cv.wait(lock);
            }

            // still no pending events
            if (m_queue.empty()) {
                continue;
            }

            *out_ptr = std::move(m_queue.front());
            m_queue.pop();

            return true;
        }

        return false;
    }

    /**
     * @brief Attempts to retrieve a pending event without waiting.
     *
     * @param out The variable to store the retrieved event.
     * @return True if an event was retrieved, false if there are no pending events.
     */
    bool get_non_blocking(event_t& out) {
        event_t* out_ptr = std::addressof(out);

        std::unique_lock lock(m_mutex);

        if (m_queue.empty()) {
            return false;
        }

        *out_ptr = std::move(m_queue.front());
        m_queue.pop();

        return true;
    }

private:
    std::queue<event_t> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::atomic<int> m_senders_count = 0;
};

}

#endif
