#include "koterm/event/KeyCodes.h"
#include "koterm/unit.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <koterm/terminal/Parser.h>
#include <koterm/terminal/terminal.h>
#include <unistd.h>

constexpr int EXIT_OK   = 0;
constexpr int EXIT_FAIL = 1;

using namespace koterm;
using terminal::FeatureFlags;

void print_event(const terminal::Parser& parser);
void print_event_mouse(const terminal::Parser::MouseEvent& event);
void print_event_key(event::KeyCode key);
void print_special_character(const terminal::Parser& parser);

int main() {

    constexpr auto required_features = std::array {
        FeatureFlags::ALTERNATIVE_SCREEN,
        FeatureFlags::NO_ECHO_AND_WAITING,
        FeatureFlags::MOUSE_SUPPORT,
    };

    if (!terminal::init(required_features)) {
        std::cout << "ERROR: " << terminal::error_msg() << '\n';
        return EXIT_FAIL;
    }

    terminal::Parser parser;
    while (true) {
        std::uint8_t byte;
        long size = read(STDIN_FILENO, &byte, sizeof(byte));

        if (size == 0) {
            continue;
        }

        auto state = parser.parse(byte);

        switch (state) {
        case terminal::Parser::ParserState::UNCOMPLETE:
            break;
        case terminal::Parser::ParserState::UNKNOWN:
            parser.clear();
            break;
        case terminal::Parser::ParserState::EVENT:
            print_event(parser);
            parser.clear();
            break;
        case terminal::Parser::ParserState::CHARACTER:
            std::cout << "CHARACTER: " << parser.text() << '\n';
            parser.clear();
            break;
        case terminal::Parser::ParserState::SPECIAL:
            print_special_character(parser);
            parser.clear();
            break;
        }
    }

    return EXIT_OK;
}

void print_event(const terminal::Parser& parser) {
    switch (parser.event_type()) {
    case terminal::Parser::EventType::NONE:
        break;
    case terminal::Parser::EventType::MOUSE:
        print_event_mouse(parser.mouse());
        break;
    case terminal::Parser::EventType::KEY:
        print_event_key(parser.key());
        break;
    case terminal::Parser::EventType::CURSOR: {
        auto cursor = parser.cursor();
        std::cout << "CURSOR:\n"
                  << "  x: " << cursor.x << "\n  y: " << cursor.y << '\n';
        break;
    }
    }
}

void print_event_mouse(const terminal::Parser::MouseEvent& event) {

    const unit_t pos_x = event.mouse.x;
    const unit_t pos_y = event.mouse.y;

    std::cout << "MOUSE:\n"
              << "  x: " << pos_x << '\n'
              << "  y: " << pos_y << '\n';
    std::cout << "  code: " << static_cast<int>(event.code) << '\n';

    if (event.is_scroll()) {
        std::cout << "  type: scroll\n"
                  << "  btn: " << (event.scroll_back() ? "scroll back" : "scroll forward") << '\n';
    } else if (!event.is_button()) {
        std::cout << "  type: move\n";
    } else {
        std::cout << "  type: click\n  btn: ";
        if (event.btn_release()) {
            std::cout << "release";
        } else if (event.btn1_press()) {
            std::cout << "left press";
        } else if (event.btn2_press()) {
            std::cout << "middle press";
        } else {
            std::cout << "right press";
        }
        std::cout << '\n';
    }
}

void print_event_key(event::KeyCode key) {
    switch (key) {
    case koterm::event::KeyCode::ARROW_UP:
        std::cout << "KEY["
                  << "ARROW_UP"
                  << "]\n";
        break;
    case koterm::event::KeyCode::ARROW_DOWN:
        std::cout << "KEY["
                  << "ARROW_DOWN"
                  << "]\n";
        break;
    case koterm::event::KeyCode::ARROW_LEFT:
        std::cout << "KEY["
                  << "ARROW_LEFT"
                  << "]\n";
        break;
    case koterm::event::KeyCode::ARROW_RIGHT:
        std::cout << "KEY["
                  << "ARROW_RIGHT"
                  << "]\n";
        break;
    case koterm::event::KeyCode::F1:
        std::cout << "KEY["
                  << "F1"
                  << "]\n";
        break;
    case koterm::event::KeyCode::F2:
        std::cout << "KEY["
                  << "F2"
                  << "]\n";
        break;
    case koterm::event::KeyCode::F3:
        std::cout << "KEY["
                  << "F3"
                  << "]\n";
        break;
    case koterm::event::KeyCode::F4:
        std::cout << "KEY["
                  << "F4"
                  << "]\n";
        break;
    case koterm::event::KeyCode::F5:
        std::cout << "KEY["
                  << "F5"
                  << "]\n";
        break;
    case koterm::event::KeyCode::F6:
        std::cout << "KEY["
                  << "F6"
                  << "]\n";
        break;
    case koterm::event::KeyCode::F7:
        std::cout << "KEY["
                  << "F7"
                  << "]\n";
        break;
    case koterm::event::KeyCode::F8:
        std::cout << "KEY["
                  << "F8"
                  << "]\n";
        break;
    case koterm::event::KeyCode::F9:
        std::cout << "KEY["
                  << "F9"
                  << "]\n";
        break;
    case koterm::event::KeyCode::F10:
        std::cout << "KEY["
                  << "F10"
                  << "]\n";
        break;
    case koterm::event::KeyCode::F11:
        std::cout << "KEY["
                  << "F11"
                  << "]\n";
        break;
    case koterm::event::KeyCode::F12:
        std::cout << "KEY["
                  << "F12"
                  << "]\n";
        break;
    case koterm::event::KeyCode::INSERT:
        std::cout << "KEY["
                  << "INSERT"
                  << "]\n";
        break;
    case koterm::event::KeyCode::DELETE:
        std::cout << "KEY["
                  << "DELETE"
                  << "]\n";
        break;
    case koterm::event::KeyCode::HOME:
        std::cout << "KEY["
                  << "HOME"
                  << "]\n";
        break;
    case koterm::event::KeyCode::END:
        std::cout << "KEY["
                  << "END"
                  << "]\n";
        break;
    case koterm::event::KeyCode::PAGE_UP:
        std::cout << "KEY["
                  << "PAGE_UP"
                  << "]\n";
        break;
    case koterm::event::KeyCode::PAGE_DOWN:
        std::cout << "KEY["
                  << "PAGE_DOWN"
                  << "]\n";
        break;
    }
}

void print_special_character(const terminal::Parser& parser) {
    auto text = parser.text();
    if (text.empty()) {
        return;
    }

    std::cout << "SPECIAL KEY: '" << static_cast<int>(text[0]) << "'\n";
}
