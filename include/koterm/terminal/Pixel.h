#ifndef KOTERM_TERMINAL_PIXEL
#define KOTERM_TERMINAL_PIXEL

#include "koterm/terminal/Color.h"
#include "koterm/util/bits.h"
#include <cstdint>
#include <string>
#include <string_view>
namespace koterm::terminal {

/**
 * @brief The PixelStyle class represents the style attributes of a pixel.
 */
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

    /**
     * @brief Creates a PixelStyle instance with specified style flags.
     *
     * @tparam Flags The style flags to include.
     * @return A PixelStyle instance with the specified style flags.
     */
    template <StyleFlags Flags> static constexpr PixelStyle from() {
        PixelStyle style;
        style.m_style |= bits::create_mask<Flags>();

        return style;
    }

    /**
     * @brief Sets a style flag.
     *
     * @tparam FLAG The style flag to set.
     */
    template <StyleFlags FLAG> void set() { m_style |= bits::bit<FLAG>(); }

    /**
     * @brief Unsets a style flag.
     *
     * @tparam FLAG The style flag to unset.
     */
    template <StyleFlags FLAG> void unset() { m_style &= ~bits::create_inverse_mask<FLAG>(); }

    /**
     * @brief Checks if a style flag is set.
     *
     * @tparam FLAG The style flag to check.
     * @return True if the style flag is set, false otherwise.
     */
    template <StyleFlags FLAG> [[nodiscard]] bool is_set() const { return bits::is_set<FLAG>(m_style); }

    /**
     * @brief Overloaded comparison operator for PixelStyle objects.
     *
     * @param other The PixelStyle object to compare.
     * @return True if the PixelStyle objects are equal, false otherwise.
     */
    auto operator<=>(const PixelStyle&) const = default;

    /**
     * @brief Writes the style attributes to the terminal, based on previous style attributes.
     *
     * @param previous The previous PixelStyle object.
     */
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
