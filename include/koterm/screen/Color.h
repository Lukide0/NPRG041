#ifndef KOTERM_SCREEN_COLOR_H
#define KOTERM_SCREEN_COLOR_H

#include <cassert>
#include <cstdint>
#include <string_view>
namespace koterm::screen {

class Color {
public:
    using value_t = std::uint8_t;

    enum class Type {
        RGB,
        SYSTEM,
    };

    constexpr Color(value_t index)
        : m_type(Type::SYSTEM)
        , m_r(index) { }

    constexpr Color(value_t r, value_t g, value_t b)
        : m_type(Type::RGB)
        , m_r(r)
        , m_g(g)
        , m_b(b) { }

    consteval Color(std::string_view hex) {
        if (!hex.starts_with('#')) {
            throw "Invalid color!";
        }

        std::uint8_t blue;
        std::uint8_t green;
        std::uint8_t red;

        // #rgb
        if (hex.size() == 4) {
            blue  = extract_value(hex[1]) | (extract_value(hex[1]) << 4);
            green = extract_value(hex[2]) | (extract_value(hex[2]) << 4);
            red   = extract_value(hex[3]) | (extract_value(hex[3]) << 4);

        }
        // #rrggbb
        else if (hex.size() == 7) {
            blue  = extract_value(hex[1]) | (extract_value(hex[2]) << 4);
            green = extract_value(hex[3]) | (extract_value(hex[4]) << 4);
            red   = extract_value(hex[5]) | (extract_value(hex[6]) << 4);

        } else {
            throw "Invalid color!";
        }

        m_r = red;
        m_g = green;
        m_b = blue;
    }

    [[nodiscard]] constexpr value_t code() const { return m_r; }
    [[nodiscard]] constexpr value_t red() const { return m_r; }
    [[nodiscard]] constexpr value_t green() const { return m_g; }
    [[nodiscard]] constexpr value_t blue() const { return m_b; }

    bool constexpr operator==(const Color& other) const {
        return m_b == other.m_b && m_r == other.m_r && m_g == other.m_g;
    }

    static constexpr Color from_hsv(value_t h, value_t s, value_t v) {

        constexpr value_t HUE_PART = 43;

        if (s == 0) {
            return { v, v, v };
        }

        const value_t region = h / HUE_PART;
        const value_t rem    = (h - (region * HUE_PART)) * 6;

        const value_t p = (v * (255 - s)) >> 8;
        const value_t q = (v * (255 - ((s * rem) >> 8))) >> 8;
        const value_t t = (v * (255 - ((s * (255 - rem)) >> 8))) >> 8;

        value_t r;
        value_t g;
        value_t b;
        switch (region) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        case 5:
        default:
            r = v;
            g = p;
            b = q;
            break;
        }

        return { r, g, b };
    }

    static constexpr Color from_rgb(value_t r, value_t g, value_t b) { return { r, g, b }; }
    static constexpr Color from_id(value_t id) { return { id }; }
    static consteval Color from_hex(std::string_view hex) { return { hex }; }

    [[nodiscard]] constexpr Type type() const { return m_type; }

private:
    static constexpr std::uint8_t extract_value(char val) {
        if ('a' <= val && val <= 'f') {
            return 10 + val - 'a';
        } else if ('A' <= val && val <= 'F') {
            return 10 + val - 'A';
        } else {
            return val - '0';
        }
    }

    Type m_type;
    value_t m_r = 0, m_g = 0, m_b = 0;
};

}

#endif
