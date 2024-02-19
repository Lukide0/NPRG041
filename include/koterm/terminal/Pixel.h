#ifndef KOTERM_TERMINAL_PIXEL
#define KOTERM_TERMINAL_PIXEL

#include "koterm/terminal/Color.h"
#include "koterm/util/bits.h"
#include <cstdint>
#include <string>
#include <string_view>
namespace koterm::terminal {

class PixelStyle {
public:
    using storage_t = std::uint8_t;
    using bits      = util::bits<storage_t>;

    enum StyleFlags : storage_t {
        BOLD,
        DIM,
        ITALIC,
        UNDERLINE,
        BLINK,
        INVERSE,
        HIDDEN,
        STRIKETHROUGH,
    };

    PixelStyle() = default;

    template <StyleFlags Flags> static constexpr PixelStyle from() {
        PixelStyle style;
        style.m_style |= bits::create_mask<Flags>();

        return style;
    }
    template <StyleFlags FLAG> void set() { m_style |= bits::bit<FLAG>(); }
    template <StyleFlags FLAG> void unset() { m_style &= ~bits::create_inverse_mask<FLAG>(); }

    template <StyleFlags FLAG> [[nodiscard]] bool is_set() const { return bits::is_set<FLAG>(m_style); }

    auto operator<=>(const PixelStyle&) const = default;

    void write(PixelStyle previous) const;

private:
    storage_t m_style = bits::BITS_NONE;
};

struct PixelContent {
    PixelContent()
        : content(" ") { }
    PixelContent(char ch) { content += ch; }
    PixelContent(std::string_view value)
        : content(value) { }

    std::string content;
};

struct PixelColor {
    Color::color_id bg;
    Color::color_id fg;
};

}

#endif
