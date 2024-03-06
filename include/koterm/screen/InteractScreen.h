#ifndef KOTERM_SCREEN_INTERACTSCREEN_H
#define KOTERM_SCREEN_INTERACTSCREEN_H

#include "koterm/event/Event.h"
#include "koterm/event/EventListener.h"
#include "koterm/screen/Screen.h"
#include "koterm/unit.h"
#include <atomic>
#include <memory>
#include <thread>
namespace koterm::screen {

/**
 * @brief A screen class for interactive applications with event handling capabilities.
 */
class InteractScreen : public Screen {
public:
    ~InteractScreen() override;

    /**
     * @brief Creates a new InteractScreen instance with the specified width and height.
     *
     * @param width The width of the screen.
     * @param height The height of the screen.
     * @return A unique pointer to the created InteractScreen instance or nullptr.
     */
    static std::unique_ptr<InteractScreen> create(unit_t width, unit_t height);

    /**
     * @brief Starts the main event loop of the screen.
     */
    void main_loop();

    /**
     * @brief Starts the event listener for handling input events.
     */
    void start_event_listener();

    /**
     * @brief Sets whether the screen can grow in size.
     *
     * @param grow Boolean value indicating whether the screen can grow.
     */
    void set_grow(bool grow = false) { m_can_grow = grow; }

    /**
     * @brief Handles all pending events.
     */
    void handle_events();

    /**
     * @brief Performs preparations before executing the screen in a custom loop.
     */
    void before_run();

    /**
     * @brief Exits the screen.
     */
    void exit_loop();

    /**
     * @brief Checks if the screen is running.
     *
     * @return True if the screen is running, false otherwise.
     */
    [[nodiscard]] bool running() const { return !m_quit; }

protected:
    InteractScreen(unit_t width, unit_t height)
        : Screen(width, height) { }

private:
    std::atomic<bool> m_quit = true;
    std::thread m_parser_thread;
    bool m_can_grow              = false;
    bool m_parser_thread_running = false;
    event::EventListener<event::Event> m_event_listener;
};

}

#endif
