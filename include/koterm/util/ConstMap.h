#ifndef KOTERM_UTIL_CONSTMAP_H
#define KOTERM_UTIL_CONSTMAP_H

#include <array>
#include <cstddef>
#include <limits>
#include <utility>
namespace koterm::util {

template <typename Key, typename Value, std::size_t Size> class ConstMap {
public:
    using pair_t               = std::pair<Key, Value>;
    using pairs_t              = std::array<pair_t, Size>;
    static constexpr auto npos = std::numeric_limits<std::size_t>::max();

    consteval ConstMap(pairs_t pairs)
        : m_data(std::move(pairs)) { }

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

    [[nodiscard]] constexpr std::size_t find(const Key& key) const {
        for (std::size_t i = 0; i < Size; i++) {
            if (m_data.at(i).first == key) {
                return i;
            }
        }

        return npos;
    }

    [[nodiscard]] constexpr const Value& operator[](const Key& key) const { return get(key); }
    [[nodiscard]] constexpr const Value& at(const Key& key) const {
        const auto index = find(key);
        return m_data[index].second;
    }

    [[nodiscard]] constexpr const pair_t& get_pair(const Key& key) const { return m_data[find(key)]; }

    [[nodiscard]] constexpr bool contains(const Key& key) const { return find(key) != npos; }

private:
    pairs_t m_data;
};

template <typename Key, typename Value, std::size_t Size>
consteval ConstMap<Key, Value, Size> to_map(std::array<std::pair<Key, Value>, Size> pairs) {
    return { pairs };
}

}

#endif
