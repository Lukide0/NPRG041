#ifndef KOTERM_TESTS_UTILS_H
#define KOTERM_TESTS_UTILS_H

#include <algorithm>
#include <array>
#include <string>

#include <doctest.h>
#include <utility>

#define KOTERM_TEST_GENERATOR(data, min, max)                                                                       \
    for (std::size_t _i = min; _i < max; ++_i) {                                                                    \
        DOCTEST_SUBCASE(("SUBCASE[" + std::to_string(_i) + '|' + std::to_string(max) + "]").c_str()) { data = _i; } \
    }

#define KOTERM_TEST_ITERATOR(data, container)                                                                  \
    {                                                                                                          \
        std::size_t _i = 0;                                                                                    \
        for (auto&& _val : container) {                                                                        \
            DOCTEST_SUBCASE((std::string(#container "[") + std::to_string(_i) + "]").c_str()) { data = _val; } \
            _i++;                                                                                              \
        }                                                                                                      \
    }

namespace detail {
template <typename T, std::size_t N, std::size_t... I> constexpr auto create_sequence_impl(std::index_sequence<I...>) {
    return std::array<T, N> { { I... } };
}
}

template <typename T, std::size_t N> constexpr auto create_sequence() {
    return detail::create_sequence_impl<T, N>(std::make_index_sequence<N> {});
}

#endif
