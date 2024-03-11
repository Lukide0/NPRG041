#include "utils.h"
#include <array>
#include <doctest.h>
#include <koterm/terminal/Parser.h>
#include <koterm/unit.h>
#include <span>
#include <string>
#include <string_view>

using namespace koterm;
using namespace koterm::terminal;
using namespace std::string_view_literals;

inline std::ostream& operator<<(std::ostream& stream, point_t point) {
    stream << "point_t{ x: " << point.x << ", y: " << point.y << " }";
    return stream;
}

#pragma warning(push)
#pragma warning(disable : 4566)
constexpr auto pos_1    = "\u0021"sv;
constexpr auto pos_10   = "\u002a"sv;
constexpr auto pos_100  = "\u0084"sv;
constexpr auto pos_1000 = "\u0408"sv;
#pragma warning(pop)

constexpr auto positions = std::to_array<std::pair<std::string_view, unit_t>>({
    {   pos_1,    1},
    {  pos_10,   10},
    { pos_100,  100},
    {pos_1000, 1000}
});

std::vector<std::uint8_t> create_btn_event(std::string_view cx, std::string_view cy, char cb) {

    std::vector<std::uint8_t> bytes;

    bytes.push_back('\x1B');
    bytes.push_back('[');
    bytes.push_back('M');
    bytes.push_back(cb);

    for (char i : cx) {
        bytes.push_back(i);
    }

    for (char i : cy) {
        bytes.push_back(i);
    }

    return bytes;
}

enum class Btn {
    MB1            = 0,
    MB2            = 1,
    MB3            = 2,
    SCROLL_BACK    = 0,
    SCROLL_FORWARD = 1,
};

template <bool IS_SCROLL = false>
constexpr char create_cb(Btn btn, bool is_pressed, bool shift = false, bool meta = false, bool control = false) {
    auto code = static_cast<std::uint8_t>(btn);
    code |= 32;
    if (!is_pressed) {
        code = 3;
    }

    if (shift) {
        code |= 4;
    }

    if (meta) {
        code |= 8;
    }

    if (control) {
        code |= 16;
    }

    if constexpr (IS_SCROLL) {
        code |= 64;
    }

    return static_cast<char>(code);
}

TEST_CASE("Parser: ascii text") {
    Parser parser;

    for (std::uint8_t byte = 32; byte < 127; byte++) {
        REQUIRE_EQ(parser.parse(byte), Parser::ParserState::CHARACTER);
        REQUIRE_EQ(parser.text(), std::string(1, byte));
        parser.clear();
    }
}

TEST_CASE("Parser: utf-8 text") {
    Parser parser;

    constexpr auto text = std::to_array<std::string_view>({
        // 2-bytes
        "¢"sv,
        "£"sv,
        "¤"sv,
        "¥"sv,
        // 3-bytes
        "ऄ"sv,
        "अ"sv,
        "आ"sv,
        "इ"sv,
        // 4-bytes
        "𐅃"sv,
        "𐅄"sv,
        "𐅅"sv,
        "𐅆"sv,
    });

    for (auto&& literal : text) {
        for (std::size_t i = 0; i < literal.size() - 1; i++) {
            REQUIRE_EQ(parser.parse(literal.at(i)), Parser::ParserState::UNCOMPLETE);
        }

        REQUIRE_EQ(parser.parse(literal.back()), Parser::ParserState::CHARACTER);
        CHECK_EQ(literal, parser.text());

        parser.clear();
    }
}

TEST_CASE("Parser: buttons") {
    using namespace std::string_view_literals;

    constexpr auto btns = std::to_array({ Btn::MB1, Btn::MB2, Btn::MB3 });

    for (auto&& [pos_x, value_x] : positions) {
        for (auto&& [pos_y, value_y] : positions) {
            for (auto&& btn : btns) {
                auto input = create_btn_event(pos_x, pos_y, create_cb(btn, true));
                Parser p;

                for (auto&& lit : std::span { input.begin(), std::prev(input.end()) }) {
                    REQUIRE_EQ(p.parse(lit), Parser::ParserState::UNCOMPLETE);
                }

                REQUIRE_EQ(p.parse(input.back()), Parser::ParserState::EVENT);
                CHECK_EQ(p.event_type(), Parser::EventType::MOUSE);

                const auto& event = p.mouse();
                CHECK_EQ(event.is_scroll(), false);
                CHECK_EQ(event.is_button(), true);

                CHECK_EQ(event.mouse, point_t { value_x, value_y });

                CHECK_EQ(event.btn_press(), true);

                switch (btn) {
                case Btn::MB1:
                    CHECK_EQ(event.btn1(), true);
                    break;
                case Btn::MB2:
                    CHECK_EQ(event.btn2(), true);
                    break;
                case Btn::MB3:
                    CHECK_EQ(event.btn3(), true);
                    break;
                }
            }
        }
    }
}

TEST_CASE("Parser: wheel") {
    using namespace std::string_view_literals;

    constexpr auto btns = std::to_array({ Btn::SCROLL_BACK, Btn::SCROLL_FORWARD });

    for (auto&& [pos_x, value_x] : positions) {
        for (auto&& [pos_y, value_y] : positions) {
            for (auto&& btn : btns) {
                auto input = create_btn_event(pos_x, pos_y, create_cb<true>(btn, true));
                Parser p;

                for (auto&& lit : std::span { input.begin(), std::prev(input.end()) }) {
                    REQUIRE_EQ(p.parse(lit), Parser::ParserState::UNCOMPLETE);
                }

                REQUIRE_EQ(p.parse(input.back()), Parser::ParserState::EVENT);
                CHECK_EQ(p.event_type(), Parser::EventType::MOUSE);

                const auto& event = p.mouse();
                CHECK_EQ(event.is_scroll(), true);
                CHECK_EQ(event.is_button(), false);

                CHECK_EQ(event.mouse, point_t { value_x, value_y });

                switch (btn) {
                case Btn::SCROLL_FORWARD:
                    CHECK_EQ(event.scroll_forward(), true);
                    break;
                case Btn::SCROLL_BACK:
                    CHECK_EQ(event.scroll_back(), true);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

TEST_CASE("Parser: Keys") {
    using koterm::event::KeyCode;

    constexpr auto KEYS = std::to_array<std::pair<std::string_view, KeyCode>>({
        {  "\x1B[A"sv,    KeyCode::ARROW_UP},
        {  "\x1B[B"sv,  KeyCode::ARROW_DOWN},
        {  "\x1B[C"sv, KeyCode::ARROW_RIGHT},
        {  "\x1B[D"sv,  KeyCode::ARROW_LEFT},
        {  "\x1BOP"sv,          KeyCode::F1},
        {  "\x1BOQ"sv,          KeyCode::F2},
        {  "\x1BOR"sv,          KeyCode::F3},
        {  "\x1BOS"sv,          KeyCode::F4},
        {"\x1B[15~"sv,          KeyCode::F5},
        {"\x1B[17~"sv,          KeyCode::F6},
        {"\x1B[18~"sv,          KeyCode::F7},
        {"\x1B[19~"sv,          KeyCode::F8},
        {"\x1B[20~"sv,          KeyCode::F9},
        {"\x1B[21~"sv,         KeyCode::F10},
        {"\x1B[23~"sv,         KeyCode::F11},
        {"\x1B[24~"sv,         KeyCode::F12},
        {  "\x1B[H"sv,        KeyCode::HOME},
        {  "\x1B[F"sv,         KeyCode::END},
        { "\x1B[2~"sv,      KeyCode::INSERT},
        { "\x1B[3~"sv,         KeyCode::DEL},
        { "\x1B[5~"sv,     KeyCode::PAGE_UP},
        { "\x1B[6~"sv,   KeyCode::PAGE_DOWN},
    });

    for (auto&& key : KEYS) {
        std::string_view input = key.first;
        Parser p;

        for (auto&& lit : std::span { input.begin(), std::prev(input.end()) }) {
            REQUIRE_EQ(p.parse(lit), Parser::ParserState::UNCOMPLETE);
        }

        REQUIRE_EQ(p.parse(input.back()), Parser::ParserState::EVENT);
        CHECK_EQ(p.event_type(), Parser::EventType::KEY);
        CHECK_EQ(p.key(), key.second);
    }
}

TEST_CASE("Parser: cursor") {
    auto create_event
        = [](unit_t x, unit_t y) -> std::string { return "\x1B[" + std::to_string(y) + ';' + std::to_string(x) + 'R'; };

    constexpr auto positions_cursor = std::to_array<unit_t>({ 1, 5, 10, 50, 100, 500, 1000 });

    for (auto&& pos_x : positions_cursor) {
        for (auto&& pos_y : positions_cursor) {
            std::string input = create_event(pos_x, pos_y);
            Parser p;

            for (auto&& lit : std::span { input.begin(), std::prev(input.end()) }) {
                REQUIRE_EQ(p.parse(lit), Parser::ParserState::UNCOMPLETE);
            }

            REQUIRE_EQ(p.parse(input.back()), Parser::ParserState::EVENT);

            CHECK_EQ(p.event_type(), Parser::EventType::CURSOR);
            CHECK_EQ(p.cursor(), point_t { pos_x, pos_y });
        }
    }
}
