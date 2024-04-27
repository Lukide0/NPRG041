#include <array>
#include <iostream>
#include <koterm/component/Alignment.h>
#include <koterm/component/TextDecoration.h>
#include <koterm/dom/Element.h>
#include <koterm/dom/Text.h>
#include <koterm/dom/drawing/Canvas.h>
#include <koterm/dom/layout/Box.h>
#include <koterm/screen/Screen.h>
#include <koterm/terminal/TerminalGuard.h>
#include <koterm/terminal/colors.h>
#include <koterm/terminal/terminal.h>
#include <memory>

using koterm::dom::Text;
using koterm::screen::Screen;
using koterm::terminal::FeatureFlags;
using koterm::terminal::TerminalGuard;

using koterm::dom::drawing::Canvas;
using koterm::terminal::colors;

constexpr int EXIT_OK   = 0;
constexpr int EXIT_FAIL = 1;

int main() {
    constexpr auto required_features = std::array {
        FeatureFlags::NONE,
    };

    TerminalGuard term;

    if (!term.init(required_features)) {
        std::cout << "ERROR: " << koterm::terminal::error_msg() << '\n';
        return EXIT_FAIL;
    }

    auto dims   = koterm::terminal::dimensions();
    auto screen = Screen::create(dims.width, dims.height);

    auto canvas = Canvas::create(screen.get());
    canvas->set_viewport(10, 20);

    auto viewport = canvas->get_viewport();
    auto width    = viewport.width;
    auto height   = viewport.height;

    // Fill a rectangle with green color
    canvas->fill_rect({ width - 1, height - 1 }, colors::SYS_GREEN);

    // Stroke the border of the rectangle with white color
    canvas->stroke_rect({ width - 1, height - 1 }, colors::SYS_BRIGHT_WHITE);

    // Draw a diagonal line from top-left to bottom-right corner with red color
    canvas->line({ 0, 0 }, { width - 1, height - 1 }, colors::SYS_RED);

    // Draw a dot
    canvas->dot({ (width - 1) / 2, (height - 1) / 2 }, colors::SYS_BRIGHT_MAGENTA);

    screen->set_document(canvas);
    screen->render();

    // wait
    std::cin.get();

    return EXIT_OK;
}
