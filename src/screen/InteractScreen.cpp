#include "koterm/screen/InteractScreen.h"
#include "koterm/Dimensions.h"
#include "koterm/event/Event.h"
#include "koterm/event/EventListener.h"
#include "koterm/event/EventSender.h"
#include "koterm/event/KeyCodes.h"
#include "koterm/terminal/Cursor.h"
#include "koterm/terminal/Parser.h"
#include "koterm/terminal/terminal.h"
#include "koterm/unit.h"
#include "koterm/util/ascii.h"
#include "koterm/util/os.h"
#include <algorithm>
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <span>
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

static InteractScreen* g_active_screen = nullptr;

void event_emiter(event::EventListener<event::Event>* listener, std::atomic<bool>* quit);

InteractScreen::~InteractScreen() { exit_loop(); }

void InteractScreen::before_run() {
    if (g_active_screen != nullptr) {
        g_active_screen->exit_loop();
    }

    g_active_screen = this;
    m_quit          = false;

    terminal::Cursor::hide();

    if (!m_parser_thread_running) {
        start_event_listener();
    }
}

void InteractScreen::exit_loop() {
    if (g_active_screen == this) {
        g_active_screen = nullptr;
        terminal::Cursor::show();
    }

    if (!m_quit) {
        m_quit = true;

        if (m_parser_thread_running) {
            m_parser_thread.join();
            m_parser_thread_running = false;
        }
    }
}

void stop_active() {
    if (g_active_screen != nullptr) {
        g_active_screen->exit_loop();
    }
}

std::unique_ptr<InteractScreen> InteractScreen::create(unit_t width, unit_t height) {
    using Features = terminal::FeatureFlags;

    if (!terminal::has_initialized()) {
        return nullptr;
    }

    if (!terminal::features().contains<Features::NO_ECHO_AND_WAITING>()) {
        return nullptr;
    }

    static bool has_signal_handler = false;
    if (!has_signal_handler) {

        std::at_quick_exit(stop_active);
        has_signal_handler = true;
    }

    return std::unique_ptr<InteractScreen>(new InteractScreen(width, height));
}

void InteractScreen::handle_events() {
    using EventType = event::Event::EventType;

    event::Event event;
    while (m_event_listener.get_non_blocking(event)) {
        if (event.type() == EventType::RESIZE) {

            auto resize             = event.get<EventType::RESIZE>();
            const auto& buffer_size = buffer().box();

            if (!m_can_grow) {
                resize.width  = std::min(resize.width, buffer_size.width());
                resize.height = std::min(resize.height, buffer_size.height());
            } else if (resize.width > buffer_size.width() || resize.height > buffer_size.height()) {
                buffer().resize(resize.width, resize.height);
            }

            m_dom.handle_event(event::Event::create_resize(resize));
            continue;
        }
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

    before_run();
    start_event_listener();

    terminal::Cursor::hide();

    while (!m_quit) {
        handle_events();
        try_render();
    }

    exit_loop();
    terminal::Cursor::show();
}

inline void
parse_bytes(const std::span<std::uint8_t>& bytes, terminal::Parser& parser, event::EventSender<event::Event>& sender) {
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
            case ParserEventType::MOUSE:
                sender.send(Event::create_from_mouse(parser.mouse()));
                break;
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

    auto sender = listener->make_sender();

    auto parser = terminal::Parser();
    std::array<std::uint8_t, 256> buffer;

    Dimensions dims = terminal::dimensions();
    while (!quit->load()) {
        if (available_input()) {
            long bytes = read(STDIN_FILENO, buffer.data(), buffer.size());
            if (bytes == -1) {
                continue;
            }

            std::span<std::uint8_t> bytes_data { buffer.data(), static_cast<std::size_t>(bytes) };

            if (bytes_data.size() == 1 && bytes_data[0] == util::ascii::ESC && parser.empty()) {
                sender.send(Event::create_key(event::KeyCode::ESC));
            } else {
                parse_bytes(std::span<std::uint8_t> { buffer.data(), static_cast<std::size_t>(bytes) }, parser, sender);
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

#elif defined(OS_WINDOWS)

void event_emiter(event::EventListener<event::Event>* listener, std::atomic<bool>* quit) {
    using event::Event;
    using ParserState     = terminal::Parser::ParserState;
    using ParserEventType = terminal::Parser::EventType;

    constexpr int TIMEOUT_MS          = 250;
    constexpr std::size_t BUFFER_SIZE = 64;

    auto sender = listener->make_sender();

    auto parser     = terminal::Parser();
    Dimensions dims = terminal::dimensions();

    HANDLE console_in = GetStdHandle(STD_INPUT_HANDLE);

    std::vector<INPUT_RECORD> records { BUFFER_SIZE };
    std::vector<std::uint8_t> character_bytes(4);

    while (!quit->load()) {
        const auto wait = WaitForSingleObject(console_in, TIMEOUT_MS);
        if (wait != WAIT_OBJECT_0) {
            continue;
        }

        DWORD events_count = 0;
        if (!GetNumberOfConsoleInputEvents(console_in, &events_count) || events_count <= 0) {
            continue;
        }

        records.resize(events_count);
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

                character_bytes.resize(4);
                int size = WideCharToMultiByte(
                    CP_UTF8,
                    0,
                    &key.uChar.UnicodeChar,
                    1,
                    reinterpret_cast<char*>(character_bytes.data()),
                    4,
                    nullptr,
                    nullptr
                );

                character_bytes.resize(size);
                if (size == 1 && character_bytes.front() == '\r') {
                    character_bytes[0] = '\n';
                }

                std::span<std::uint8_t> buffer { character_bytes };

                parse_bytes(buffer, parser, sender);
                break;
            }
            case WINDOW_BUFFER_SIZE_EVENT:
                terminal::update_dimensions();
                sender.send(Event::create_resize(terminal::dimensions()));
                break;
            case MOUSE_EVENT:
            default:
                break;
            }
        }
    }
}
#endif
}
