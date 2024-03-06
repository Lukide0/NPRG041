#ifndef KOTERM_SCREEN_SCREEN_H
#define KOTERM_SCREEN_SCREEN_H

#include "koterm/screen/BaseScreen.h"
#include "koterm/unit.h"
#include <memory>

namespace koterm::screen {

class Screen : public BaseScreen {
public:
    /**
     * @brief Creates a new screen with the specified dimensions.
     *
     * @param width  The width of the screen.
     * @param height The height of the screen.
     * @return A unique pointer to the created screen, or nullptr if the terminal is not initialized.
     */
    static std::unique_ptr<Screen> create(unit_t width, unit_t height);

    /**
     * @brief Draws the elements to the screen.
     */
    virtual void render();

    /**
     * @brief Checks if rendering is needed and performs rendering if necessary.
     */
    void try_render() {
        if (need_render()) {
            render();
        }
    }

protected:
    /**
     * @brief Constructs a screen with the specified dimensions.
     *
     * @param width  The width of the screen.
     * @param height The height of the screen.
     */
    Screen(unit_t width, unit_t height)
        : BaseScreen(width, height) { }
};

}

#endif
