#ifndef KOTERM_UTIL_DEBUG_H
#define KOTERM_UTIL_DEBUG_H

namespace koterm::util {

consteval bool debug_mode() {
#ifndef KOTERM_DEBUG
    return false;
#else
    return false;
#endif
}

consteval bool enabled_exceptions() {
#ifndef KOTERM_NO_EXCEPTIONS
    return true;
#else
    return false;
#endif
}

}

#endif
