#ifndef KOTERM_UTIL_UTF8_H
#define KOTERM_UTIL_UTF8_H

#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>
namespace koterm::util {

/**
 * @brief Computes the Unicode code point from the given UTF-8 encoded bytes.
 *
 * This function interprets the UTF-8 encoded bytes and computes the Unicode code point.
 *
 * @param bytes A span representing the UTF-8 encoded bytes.
 * @return The Unicode code point computed from the UTF-8 encoded bytes.
 */
inline constexpr std::size_t utf8_codepoint(std::span<std::uint8_t> bytes) {
    constexpr std::size_t UTF8_BYTE_MASK = 0b0011'1111;

    const std::size_t size = bytes.size();
    const auto mask        = static_cast<std::size_t>(std::numeric_limits<std::uint8_t>::max() >> size);

    std::size_t codepoint = bytes.front() & mask;
    for (std::size_t i = 1; i < size; i++) {
        codepoint = (codepoint << 6) | (bytes[i] & UTF8_BYTE_MASK);
    }

    return codepoint;
}

}

#endif
