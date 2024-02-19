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

class InteractScreen : public Screen {
public:
    ~InteractScreen() override;

    static std::unique_ptr<InteractScreen> create(unit_t width, unit_t height);

    void main_loop();

    void start_event_listener();

    void handle_events();
    void run() { m_quit = false; }
    void exit() { m_quit = true; }
    [[nodiscard]] bool running() const { return !m_quit; }

protected:
    InteractScreen(unit_t width, unit_t height)
        : Screen(width, height) { }

private:
    std::atomic<bool> m_quit = true;
    std::thread m_parser_thread;
    bool m_parser_thread_running = false;
    event::EventListener<event::Event> m_event_listener;
};

}

#endif
