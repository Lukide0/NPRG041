#include <array>
#include <cstddef>
#include <iostream>
#include <koterm/component/Alignment.h>
#include <koterm/component/Border.h>
#include <koterm/component/TextDecoration.h>
#include <koterm/dom/Element.h>
#include <koterm/dom/Text.h>
#include <koterm/dom/input/Slider.h>
#include <koterm/dom/layout/Box.h>
#include <koterm/screen/InteractScreen.h>
#include <koterm/terminal/TerminalGuard.h>
#include <koterm/terminal/colors.h>
#include <koterm/terminal/terminal.h>
#include <memory>
#include <string>

using koterm::dom::Text;
using koterm::screen::InteractScreen;
using koterm::terminal::FeatureFlags;
using koterm::terminal::TerminalGuard;

using koterm::component::Alignment;
using koterm::component::Border;
using koterm::component::TextDecoration;

using koterm::dom::layout::BoxDirection;
using koterm::dom::layout::HBox;
using koterm::dom::layout::VBox;

using koterm::terminal::colors;

using koterm::dom::input::Slider;

constexpr int EXIT_OK   = 0;
constexpr int EXIT_FAIL = 1;

int main() {
    constexpr auto required_features = std::array {
        FeatureFlags::NO_ECHO_AND_WAITING,
        FeatureFlags::MOUSE_SUPPORT,
    };

    TerminalGuard term;

    if (!term.init(required_features)) {
        std::cout << "ERROR: " << koterm::terminal::error_msg() << '\n';
        return EXIT_FAIL;
    }

    auto dims   = koterm::terminal::dimensions();
    auto screen = InteractScreen::create(dims.width, dims.height);

    auto box  = VBox::create(screen.get());
    auto text = Text::create(screen.get());

    auto input = Slider::create(screen.get());

    input->set_range(0, 100);

    input->on_change().connect([&text](int value) { text->set_text(std::to_string(value)); });

    box->add_element(text);
    box->add_element(input);

    screen->set_document(box);
    screen->main_loop();

    return EXIT_OK;
}
