#ifndef KOTERM_UTIL_SAFE_GETENV_H
#define KOTERM_UTIL_SAFE_GETENV_H

#include "koterm/util/os.h"

#ifdef OS_WINDOWS
// [C4996] 'getenv': This function of variable may be unsafe
#pragma warning(disable : 4996)
#endif

#include <cstdlib>
#include <string_view>

namespace koterm::util {

std::string_view safe_getenv(const char* name) {
    const char* value = std::getenv(name);
    if (value == nullptr) {
        return "";
    } else {
        return value;
    }
}

}

#endif
