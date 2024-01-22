#ifndef KOTERM_TESTS_UTILS_H
#define KOTERM_TESTS_UTILS_H

#include <algorithm>
#include <string>

#include <doctest.h>

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

#endif
