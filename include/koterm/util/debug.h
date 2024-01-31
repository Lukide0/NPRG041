#ifndef KOTERM_UTIL_DEBUG_H
#define KOTERM_UTIL_DEBUG_H

#include <cassert>
#include <iostream>
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

#ifndef KOTERM_NO_EXCEPTIONS
#define safe_throw(exception, assert_msg) throw exception
#else
#define safe_throw(exception, assert_msg) assert(false && (assert_msg))
#endif

#define koterm_assert(cond, exception, assert_msg) \
    if (!(cond)) {                                 \
        safe_throw(exception, assert_msg);         \
    }

}

#endif
