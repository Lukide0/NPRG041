#ifndef KOTERM_UTIL_BITS_H
#define KOTERM_UTIL_BITS_H

#include "koterm/util/type.h"
#include <concepts>

namespace koterm::util {

template <std::integral Storage> struct bits {
    using value_t                     = Storage;
    static constexpr std::size_t BITS = sizeof(Storage) * 8;

    static constexpr value_t BITS_NONE = 0;
    static constexpr value_t BITS_ALL  = ~static_cast<value_t>(0);

    bits() = delete;

    template <std::size_t INDEX> static consteval value_t flag() { return bit<INDEX>(); }
    template <value_t VALUE> static consteval value_t flag_to_bit() {
        std::size_t index = 0;
        for (auto value = VALUE; (value & 0b1) == 0; value >> 1) {
            index++;
        }
        return index;
    }
    template <std::size_t INDEX> static consteval value_t bit() {
        static_assert(INDEX < BITS);
        return 1 << INDEX;
    }

    template <std::size_t... INDEXES> static inline constexpr bool is_set(value_t value) {
        return (((bit<INDEXES>() & value) != BITS_NONE) && ...);
    }

    template <std::size_t... INDEXES> static consteval value_t create_mask() { return (bit<INDEXES>() | ...); }
    template <std::size_t... OTHER> static consteval value_t create_inverse_mask() {
        return ~(create_mask<OTHER...>());
    }
};

template <enum_or_integral Flags> class bit_flags {
public:
    using storage_t = std::underlying_type_t<Flags>;
    using bits_t    = bits<storage_t>;

    constexpr bit_flags() = default;

    constexpr bit_flags(Flags default_flags)
        : m_data(static_cast<storage_t>(default_flags)) { }

    template <Flags... FLAGS> constexpr bit_flags<Flags>& set() {
        m_data |= ((static_cast<storage_t>(FLAGS)) | ...);
        return *this;
    }

    template <Flags... FLAGS> [[nodiscard]] constexpr bool contains() const {
        static_assert(sizeof...(FLAGS) != 0);

        return (m_data & (static_cast<storage_t>(FLAGS) | ...)) == (static_cast<storage_t>(FLAGS) | ...);
    }

    template <Flags... FLAGS> [[nodiscard]] constexpr bool contains_any() const {
        static_assert(sizeof...(FLAGS) != 0);

        return (m_data & (static_cast<storage_t>(FLAGS) | ...)) != 0;
    }

    constexpr storage_t data() const { return m_data; }

private:
    storage_t m_data = 0;
};

}

#endif
