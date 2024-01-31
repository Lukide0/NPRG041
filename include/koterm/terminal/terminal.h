#ifndef KOTERM_TERMINAL_TERMINAL_H
#define KOTERM_TERMINAL_TERMINAL_H

#include "koterm/Dimensions.h"
#include "koterm/unit.h"
#include "koterm/util/bits.h"
#include <cstdint>
#include <string_view>

namespace koterm::terminal {

enum class ColorSupport {
    COLOR16,
    COLOR256,
    TRUE_COLOR,
};

enum class FeatureFlags : std::uint8_t {
    NONE                = 0,
    MOUSE_SUPPORT       = 1 << 0,
    ALTERNATIVE_SCREEN  = 1 << 1,
    SUPPORT_TRUE_COLOR  = 1 << 2,
    NO_ECHO_AND_WAITING = 1 << 3,

};

using Features = util::bit_flags<FeatureFlags>;

/**
 * @brief Initializes the terminal with the specified features.
 *
 * @param features The feature flags to enable.
 * @return True if initialization is successful; otherwise, false.
 */
bool init(Features features);

/**
 * @brief Checks if the terminal has been initialized.
 *
 * @return True if the terminal has been initialized; otherwise, false.
 */
bool has_initialized();

/**
 * @brief Retrieves the level of color support for the terminal.
 *
 * @return The color support level.
 */
ColorSupport color_support();

/**
 * @brief Retrieves the dimensions (width and height) of the terminal.
 *
 * @return The dimensions of the terminal.
 */
Dimensions dimensions();

/**
 * @brief Retrieves the feature flags currently enabled for the terminal.
 *
 * @return The enabled feature flags.
 */
Features features();

/**
 * @brief Checks if the application is running in a terminal environment.
 *
 * @return True if running in a terminal; otherwise, false.
 */
bool is_tty();

/**
 * @brief Checks if there is input available in the terminal.
 *
 * @return True if input is available; otherwise, false.
 */
bool input_available();

void handle_signal(int sig);
std::string_view error_msg();

}

#endif
