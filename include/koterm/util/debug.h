#ifndef KOTERM_UTIL_DEBUG_H
#define KOTERM_UTIL_DEBUG_H

#include <cassert>
#include <iostream>
namespace koterm::util {

/**
 * @brief Checks if debug mode is enabled.
 *
 * @return True if debug mode is enabled; otherwise, false.
 */
consteval bool debug_mode() {
#ifndef KOTERM_DEBUG
    return false;
#else
    return true;
#endif
}

/**
 * @brief Checks if debug mode for the DOM (Document Object Model) is enabled.
 *
 * @return True if debug mode for the DOM is enabled; otherwise, false.
 */
consteval bool debug_dom() {
#ifndef KOTERM_DEBUG_DOM
    return false;
#else
    return true;
#endif
}

/**
 * @brief Checks if exceptions are enabled.
 *
 * @return True if exceptions are enabled; otherwise, false.
 */
consteval bool enabled_exceptions() {
#ifndef KOTERM_NO_EXCEPTIONS
    return true;
#else
    return false;
#endif
}

/**
 * @brief Safely throws an exception
 *
 * Throws the provided exception (if exceptions are enabled) or asserts with the provided message.
 *
 * @param exception The exception to throw
 * @param assert_msg The message to include in the assertion error
 */
#ifndef KOTERM_NO_EXCEPTIONS
#define safe_throw(exception, assert_msg) throw exception
#else
#define safe_throw(exception, assert_msg) assert(false && (assert_msg))
#endif

/**
 * @brief Macro for asserting a condition and throwing an exception if it fails.
 *
 * This macro asserts that a condition is true and throws an exception if the condition is false (if exceptions are
 * enabled).
 *
 * @param cond The condition to check.
 * @param exception The exception to throw if the condition is not met.
 * @param assert_msg The message to include in the assertion error if the condition is not met.
 */
#define koterm_assert(cond, exception, assert_msg) \
    if (!(cond)) {                                 \
        safe_throw(exception, assert_msg);         \
    }

}

#endif
