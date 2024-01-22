#ifndef KOTERM_SCREEN_PIXEL
#define KOTERM_SCREEN_PIXEL

#include "koterm/screen/ColorCache.h"
#include "koterm/util/bits.h"
#include <cstdint>
#include <string>
namespace koterm::screen {

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

    template <StyleFlags FLAG> void set() { m_style |= bits::bit<FLAG>(); }
    template <StyleFlags FLAG> void unset() { m_style &= ~bits::create_inverse_mask<FLAG>(); }

    template <StyleFlags FLAG> [[nodiscard]] bool is_set() const { return bits::is_set<FLAG>(m_style); }

    auto operator<=>(const PixelStyle&) const = default;

    void write(std::ostream& stream, PixelStyle previous) const;

private:
    storage_t m_style = bits::BITS_NONE;
};

struct PixelContent {
    constexpr PixelContent()
        : content(" ") { }
    constexpr PixelContent(char ch) { content += ch; }
    constexpr PixelContent(std::string_view value)
        : content(value) { }

    std::string content;
};

struct PixelColor {
    ColorCache::id_t bg;
    ColorCache::id_t fg;
};

}

#endif
