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

#ifdef OS_LINUX

#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#elif defined(OS_WINDOWS)

#define WIN32_LEAN_AND_MEAN

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <io.h>

#endif

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

inline void parse_bytes(
    const std::span<std::uint8_t>& bytes,
    terminal::Parser& parser,
    event::EventSender<event::Event>& sender,
    event::MouseEvent::Btn& last_pressed
) {
    using event::Event;
    using ParserState     = terminal::Parser::ParserState;
    using ParserEventType = terminal::Parser::EventType;

    for (auto&& byte : bytes) {
        switch (parser.parse(byte)) {
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
}

#ifdef OS_LINUX

bool available_input() {
    timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 0;

    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv);

    return FD_ISSET(STDIN_FILENO, &fds);
}

void event_emiter(event::EventListener<event::Event>* listener, std::atomic<bool>* quit) {
    using event::Event;
    using ParserState     = terminal::Parser::ParserState;
    using ParserEventType = terminal::Parser::EventType;

    auto sender = listener->make_sender();

    auto parser = terminal::Parser();
    std::array<std::uint8_t, 256> buffer;

    Dimensions dims                     = terminal::dimensions();
    event::MouseEvent::Btn last_pressed = event::MouseEvent::Btn::NONE;
    while (!quit->load()) {
        if (terminal::input_available()) {
            int bytes = read(STDIN_FILENO, buffer.data(), buffer.size());
            if (bytes == -1) {
                continue;
            }

            parse_bytes(std::span<std::uint8_t> { buffer.data(), bytes }, parser, sender, last_pressed);
        } else {
            Dimensions new_dims = terminal::dimensions();
            if (new_dims != dims) {
                sender.send(Event::create_resize(new_dims));
                dims = new_dims;
            }
        }
    }
}

#elif defined(OS_WINDOWS)

void event_emiter(event::EventListener<event::Event>* listener, std::atomic<bool>* quit) {
    using event::Event;
    using ParserState     = terminal::Parser::ParserState;
    using ParserEventType = terminal::Parser::EventType;

    constexpr int TIMEOUT_MS          = 250;
    constexpr std::size_t BUFFER_SIZE = 64;

    auto sender = listener->make_sender();

    auto parser                         = terminal::Parser();
    Dimensions dims                     = terminal::dimensions();
    event::MouseEvent::Btn last_pressed = event::MouseEvent::Btn::NONE;

    HANDLE console_in = GetStdHandle(STD_INPUT_HANDLE);

    std::vector<INPUT_RECORD> records { BUFFER_SIZE };

    while (!quit->load()) {
        const auto wait = WaitForSingleObject(console_in, TIMEOUT_MS);
        if (wait != WAIT_OBJECT_0) {
            continue;
        }

        DWORD events_count = 0;
        if (!GetNumberOfConsoleInputEvents(console_in, &events_count) || events_count <= 0) {
            continue;
        }

        records.reserve(events_count);
        DWORD read_events = 0;
        ReadConsoleInput(console_in, records.data(), events_count, &read_events);

        records.resize(read_events);

        for (const auto& record : records) {
            switch (record.EventType) {
            case KEY_EVENT: {
                const auto& key = record.Event.KeyEvent;

                if (key.bKeyDown == false) {
                    break;
                }

                wchar_t key_data = key.uChar.UnicodeChar;
                auto* data_raw   = reinterpret_cast<std::uint8_t*>(&key_data);

                std::span<std::uint8_t> buffer { data_raw, sizeof(wchar_t) };

                parse_bytes(buffer, parser, sender, last_pressed);
                break;
            }
            case WINDOW_BUFFER_SIZE_EVENT:
                terminal::update_dimensions();
                sender.send(Event::create_resize(terminal::dimensions()));
                break;
            default:
                break;
            }
        }
    }

    CloseHandle(console_in);
}
#endif
}
