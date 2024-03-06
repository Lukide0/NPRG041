#ifndef KOTERM_UTIL_CONSTMAP_H
#define KOTERM_UTIL_CONSTMAP_H

#include <array>
#include <cstddef>
#include <limits>
#include <utility>
namespace koterm::util {

/**
 * @brief Template class representing a constant map with compile-time lookup.
 *
 * This template class provides a constant map with compile-time lookup capabilities. It allows for efficient retrieval
 * of values associated with keys at compile time.
 *
 * @tparam Key The type of keys in the map.
 * @tparam Value The type of values associated with keys.
 * @tparam Size The size of the map.
 */
template <typename Key, typename Value, std::size_t Size> class ConstMap {
public:
    using pair_t               = std::pair<Key, Value>;
    using pairs_t              = std::array<pair_t, Size>;
    static constexpr auto npos = std::numeric_limits<std::size_t>::max();

    /**
     * @brief Constructs a ConstMap with the specified pairs.
     *
     * Constructs a ConstMap with the specified pairs of keys and values.
     *
     * @param pairs The array of pairs to initialize the map.
     */
    consteval ConstMap(pairs_t pairs)
        : m_data(std::move(pairs)) { }

    /**
     * @brief Checks if the map contains duplicate keys.
     *
     * This function checks if the map contains any duplicate keys.
     *
     * @return True if the map contains duplicate keys; otherwise, false.
     */
    [[nodiscard]] consteval bool contains_duplicate_key() const {

        for (std::size_t i = 0; i < Size; i++) {
            for (std::size_t j = i + 1; j < Size; j++) {
                if (m_data[i].first == m_data[j].first) {
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * @brief Finds the index of the specified key.
     *
     * This function finds the index of the specified key in the map.
     *
     * @param key The key to search for.
     * @return The index of the key if found; otherwise, returns npos.
     */
    [[nodiscard]] constexpr std::size_t find(const Key& key) const {
        for (std::size_t i = 0; i < Size; i++) {
            if (m_data.at(i).first == key) {
                return i;
            }
        }

        return npos;
    }

    /**
     * @brief Retrieves the value associated with the specified key.
     *
     * This function retrieves the value associated with the specified key.
     *
     * @param key The key whose associated value is to be retrieved.
     * @return The value associated with the specified key.
     */
    [[nodiscard]] constexpr const Value& operator[](const Key& key) const { return get(key); }

    /**
     * @brief Retrieves the value associated with the specified key.
     *
     * This function retrieves the value associated with the specified key.
     *
     * @param key The key whose associated value is to be retrieved.
     * @return The value associated with the specified key.
     */
    [[nodiscard]] constexpr const Value& at(const Key& key) const {
        const auto index = find(key);
        return m_data[index].second;
    }

    /**
     * @brief Retrieves the pair associated with the specified key.
     *
     * This function retrieves the pair associated with the specified key.
     *
     * @param key The key whose associated pair is to be retrieved.
     * @return The pair associated with the specified key.
     */
    [[nodiscard]] constexpr const pair_t& get_pair(const Key& key) const { return m_data[find(key)]; }

    /**
     * @brief Checks if the map contains the specified key.
     *
     * This function checks if the map contains the specified key.
     *
     * @param key The key to check for.
     * @return True if the map contains the key; otherwise, false.
     */
    [[nodiscard]] constexpr bool contains(const Key& key) const { return find(key) != npos; }

private:
    pairs_t m_data;
};

/**
 * @brief Converts an array of pairs to a ConstMap.
 *
 * This function converts an array of pairs to a ConstMap.
 *
 * @tparam Key The type of keys in the map.
 * @tparam Value The type of values associated with keys.
 * @tparam Size The size of the map.
 * @param pairs The array of pairs to convert.
 * @return A ConstMap initialized with the provided pairs.
 */
template <typename Key, typename Value, std::size_t Size>
consteval ConstMap<Key, Value, Size> to_map(std::array<std::pair<Key, Value>, Size> pairs) {
    return { pairs };
}

}

#endif
