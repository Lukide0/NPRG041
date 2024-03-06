#ifndef KOTERM_TERMINAL_TERMINAL_H
#define KOTERM_TERMINAL_TERMINAL_H

#include "koterm/Dimensions.h"
#include "koterm/unit.h"
#include "koterm/util/bits.h"
#include <cstdint>
#include <functional>
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
 * @brief Sets the terminal to the state before calling init.
 */
void cleanup();

/**
 * @brief Registers a callback function to be called before terminal cleanup.
 *
 * @param handle The callback function to register.
 * @return True if the callback function is successfully registered; otherwise, false.
 */
bool register_exit_handle(std::function<void()> handle);

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
 * @brief Updates the dimensions of the terminal.
 */
void update_dimensions();

/**
 * @brief Handles a signal received by the program.
 *
 * @param sig The signal to handle.
 */
void handle_signal(int sig);

/**
 * @brief Retrieves the error message associated with the last operation.
 *
 * @return A string view containing the error message, or an empty string view if no error occurred.
 */
std::string_view error_msg();

}

#endif
