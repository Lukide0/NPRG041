#include "koterm/screen/InteractScreen.h"
#include "koterm/Dimensions.h"
#include "koterm/event/Event.h"
#include "koterm/event/EventListener.h"
#include "koterm/event/EventSender.h"
#include "koterm/terminal/Parser.h"
#include "koterm/terminal/terminal.h"
#include "koterm/unit.h"
#include "koterm/util/os.h"
#include <array>
#include <atomic>
#include <cstdint>
#include <memory>
#include <thread>

namespace koterm::screen {

void event_emiter(event::EventListener<event::Event>* listener, std::atomic<bool>* quit);

InteractScreen::~InteractScreen() { exit(); }

std::unique_ptr<InteractScreen> InteractScreen::create(unit_t width, unit_t height) {
    using Features = terminal::FeatureFlags;

    if (!terminal::has_initialized()) {
        return nullptr;
    }

    if (!terminal::features().contains<Features::NO_ECHO_AND_WAITING>()) {
        return nullptr;
    }

    return std::unique_ptr<InteractScreen>(new InteractScreen(width, height));
}

void InteractScreen::handle_events() {
    event::Event event;
    while (m_event_listener.get_non_blocking(event)) {
        m_dom.handle_event(event);
    }
}

void InteractScreen::start_event_listener() {
    if (m_parser_thread_running) {
        return;
    }

    m_parser_thread         = std::thread { event_emiter, &m_event_listener, &m_quit };
    m_parser_thread_running = true;
}

void InteractScreen::main_loop() {
    m_quit = false;

    start_event_listener();

    while (!m_quit) {
        handle_events();
        try_render();
    }
}

#ifdef OS_LINUX
void event_emiter(event::EventListener<event::Event>* listener, std::atomic<bool>* quit) {
    using event::Event;
    using ParserState     = terminal::Parser::ParserState;
    using ParserEventType = terminal::Parser::EventType;

    auto sender = listener->make_sender();

    auto parser = terminal::Parser();
    std::array<std::uint8_t, 256> buffer;

    Dimensions dims                     = terminal::dimensions();
    event::MouseEvent::Btn last_pressed = event::MouseEvent::Btn::NONE;
    while (!(*quit)) {
        if (terminal::input_available()) {
            int bytes = read(STDIN_FILENO, buffer.data(), buffer.size());
            if (bytes == -1) {
                continue;
            }

            for (int i = 0; i < bytes; i++) {
                switch (parser.parse(buffer[i])) {
                case ParserState::UNCOMPLETE:
                    break;
                case ParserState::UNKNOWN:
                    parser.clear();
                    break;
                case ParserState::EVENT:
                    switch (parser.event_type()) {
                    case ParserEventType::MOUSE: {
                        const auto& mouse = parser.mouse();
                        if (!mouse.is_button()) {
                            sender.send(Event::create_from_mouse(mouse));
                            break;
                        }

                        if (mouse.btn_release()) {
                            sender.send(Event::create_from_mouse(mouse, last_pressed));
                            last_pressed = event::MouseEvent::Btn::NONE;
                            break;
                        }

                        if (mouse.btn3_press()) {
                            last_pressed = event::MouseEvent::Btn::RIGHT;
                        } else if (mouse.btn2_press()) {
                            last_pressed = event::MouseEvent::Btn::MIDDLE;
                        } else {
                            last_pressed = event::MouseEvent::Btn::LEFT;
                        }

                        sender.send(Event::create_from_mouse(mouse, last_pressed));
                        break;
                    }
                    case ParserEventType::KEY:
                        sender.send(Event::create_key(parser.key()));
                        break;
                    case ParserEventType::CURSOR:
                        sender.send(Event::create_cursor(parser.cursor()));
                        break;
                    default:
                        break;
                    }
                    parser.clear();
                    break;
                case ParserState::CHARACTER:
                    sender.send(Event::create_character(parser.text()));
                    parser.clear();
                    break;
                case ParserState::SPECIAL:
                    sender.send(Event::create_special_character(parser.code()));
                    parser.clear();
                    break;
                }
            }
        } else {
            Dimensions new_dims = terminal::dimensions();
            if (new_dims != dims) {
                sender.send(Event::create_resize(new_dims));
                dims = new_dims;
            }
        }
    }
}
#endif
}
