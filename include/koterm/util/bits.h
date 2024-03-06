#ifndef KOTERM_UTIL_BITS_H
#define KOTERM_UTIL_BITS_H

#include "koterm/util/type.h"
#include <array>
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace koterm::util {

/**
 * @brief Template class for managing individual bits within an integral storage type.
 *
 * This template class provides functionality for managing individual bits within an integral storage type. It is
 * primarily used for handling flag-based operations.
 *
 * @tparam Storage The integral storage type to manage.
 */
template <std::unsigned_integral Storage> struct bits {
    using value_t                     = Storage;
    static constexpr std::size_t BITS = sizeof(Storage) * 8;

    static constexpr value_t BITS_NONE = 0;
    static constexpr value_t BITS_ALL  = static_cast<value_t>(-1L);

    bits() = delete;

    /**
     * @brief Returns the value of the bit at the specified index.
     *
     * This function returns the value of the bit at the specified index.
     *
     * @tparam INDEX The index of the bit to retrieve.
     * @return The value of the bit at the specified index.
     */
    template <std::size_t INDEX> static consteval value_t flag() { return bit<INDEX>(); }

    /**
     * @brief Converts a flag value to its corresponding bit representation.
     *
     * This function converts a flag value to its corresponding bit representation.
     *
     * @tparam VALUE The flag value to convert.
     * @return The bit representation of the specified flag value.
     */
    template <value_t VALUE> static consteval value_t flag_to_bit() {
        std::size_t index = 0;
        for (auto value = VALUE; (value & 0b1) == 0; value >> 1) {
            index++;
        }
        return index;
    }

    /**
     * @brief Returns the bit value at the specified index.
     *
     * This function returns the bit value at the specified index.
     *
     * @tparam INDEX The index of the bit to retrieve.
     * @return The bit value at the specified index.
     */
    template <std::size_t INDEX> static consteval value_t bit() {
        static_assert(INDEX < BITS);
        return 1 << INDEX;
    }

    /**
     * @brief Checks if the specified bits are set in the given value.
     *
     * This function checks if the specified bits are set in the given value.
     *
     * @tparam INDEXES The indexes of the bits to check.
     * @param value The value to check.
     * @return True if all specified bits are set; otherwise, false.
     */
    template <std::size_t... INDEXES> static inline constexpr bool is_set(value_t value) {
        return (((bit<INDEXES>() & value) != BITS_NONE) && ...);
    }

    /**
     * @brief Creates a mask with the specified bits set.
     *
     * This function creates a mask with the specified bits set.
     *
     * @tparam INDEXES The indexes of the bits to set in the mask.
     * @return The mask with the specified bits set.
     */
    template <std::size_t... INDEXES> static consteval value_t create_mask() { return (bit<INDEXES>() | ...); }

    /**
     * @brief Creates an inverse mask with the specified bits unset.
     *
     * This function creates an inverse mask with the specified bits unset.
     *
     * @tparam OTHER The indexes of the bits to unset in the inverse mask.
     * @return The inverse mask with the specified bits unset.
     */
    template <std::size_t... OTHER> static consteval value_t create_inverse_mask() {
        return static_cast<value_t>(~(create_mask<OTHER...>()));
    }
};

/**
 * @brief Template class for managing sets of bit flags.
 *
 * This template class provides functionality for managing sets of bit flags.
 *
 * @tparam Flags The enumeration type representing the individual flags.
 */
template <enum_or_integral Flags> class bit_flags {
public:
    using storage_t = std::underlying_type_t<Flags>;
    using bits_t    = bits<storage_t>;

    constexpr bit_flags() = default;

    /**
     * @brief Constructor for bit_flags that initializes flags with the provided array.
     *
     * This constructor initializes the flags with the provided array of flag values.
     *
     * @param flags The array of flag values to initialize.
     */
    template <std::size_t N> constexpr bit_flags(const std::array<Flags, N>& flags) { set(flags); }

    /**
     * @brief Constructor for bit_flags that initializes flags with the provided default value.
     *
     * This constructor initializes the flags with the provided default value.
     *
     * @param default_flags The default value to initialize.
     */
    constexpr bit_flags(Flags default_flags)
        : m_data(static_cast<storage_t>(default_flags)) { }

    /**
     * @brief Sets the specified flags.
     *
     * This function sets the specified flags.
     *
     * @tparam FLAGS The flags to set.
     * @return A reference to the modified bit_flags object.
     */
    template <Flags... FLAGS> constexpr bit_flags<Flags>& set() {
        m_data |= ((static_cast<storage_t>(FLAGS)) | ...);
        return *this;
    }

    /**
     * @brief Sets the flags specified in the provided array.
     *
     * This function sets the flags specified in the provided array.
     *
     * @param flags The array of flags to set.
     * @return A reference to the modified bit_flags object.
     */
    template <std::size_t N> constexpr bit_flags<Flags>& set(const std::array<Flags, N>& flags) {
        for (auto&& flag : flags) {
            m_data |= static_cast<storage_t>(flag);
        }

        return *this;
    }

    /**
     * @brief Checks if all of the specified flags are set.
     *
     * This function checks if all of the specified flags are set.
     *
     * @tparam FLAGS The flags to check.
     * @return True if all specified flags are set; otherwise, false.
     */
    template <Flags... FLAGS> [[nodiscard]] constexpr bool contains() const {
        static_assert(sizeof...(FLAGS) != 0);

        return (m_data & (static_cast<storage_t>(FLAGS) | ...)) == (static_cast<storage_t>(FLAGS) | ...);
    }

    /**
     * @brief Checks if any of the specified flags are set.
     *
     * This function checks if any of the specified flags are set.
     *
     * @tparam FLAGS The flags to check.
     * @return True if any specified flag is set; otherwise, false.
     */
    template <Flags... FLAGS> [[nodiscard]] constexpr bool contains_any() const {
        static_assert(sizeof...(FLAGS) != 0);

        return (m_data & (static_cast<storage_t>(FLAGS) | ...)) != 0;
    }

    /**
     * @brief Retrieves the underlying storage value.
     *
     * This function retrieves the underlying storage value representing the set flags.
     *
     * @return The underlying storage value.
     */
    constexpr storage_t data() const { return m_data; }

private:
    storage_t m_data = 0;
};

}

#endif
