#ifndef KOTERM_UTIL_SAFE_GETENV_H
#define KOTERM_UTIL_SAFE_GETENV_H

#include "koterm/util/os.h"

#ifdef OS_WINDOWS
// [C4996] 'getenv': This function or variable may be unsafe
#pragma warning(disable : 4996)
#endif

#include <cstdlib>
#include <string_view>

namespace koterm::util {

/**
 * @brief Safely retrieves the value of an environment variable.
 *
 * This function retrieves the value of the environment variable with the specified name.
 * If the environment variable is not set or is empty, it returns an empty string.
 *
 * @param name The name of the environment variable to retrieve.
 * @return A string view containing the value of the environment variable, or an empty string if not set or empty.
 */
inline std::string_view safe_getenv(const char* name) {
    const char* value = std::getenv(name);
    if (value == nullptr) {
        return "";
    } else {
        return value;
    }
}

}

#endif
