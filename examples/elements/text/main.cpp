#include <array>
#include <iostream>
#include <koterm/component/Alignment.h>
#include <koterm/component/TextDecoration.h>
#include <koterm/dom/Text.h>
#include <koterm/dom/layout/Box.h>
#include <koterm/screen/InteractScreen.h>
#include <koterm/terminal/TerminalGuard.h>
#include <koterm/terminal/terminal.h>

using koterm::dom::Text;
using koterm::screen::InteractScreen;
using koterm::terminal::FeatureFlags;
using koterm::terminal::TerminalGuard;

using koterm::component::Alignment;
using koterm::component::TextDecoration;

constexpr int EXIT_OK   = 0;
constexpr int EXIT_FAIL = 1;

int main() {

    constexpr auto required_features = std::array {
        FeatureFlags::NO_ECHO_AND_WAITING,
    };

    TerminalGuard term;

    if (!term.init(required_features)) {
        std::cout << "ERROR: " << koterm::terminal::error_msg() << '\n';
        return EXIT_FAIL;
    }

    auto dims   = koterm::terminal::dimensions();
    auto screen = InteractScreen::create(dims.width, dims.height);

    auto text = Text::create(screen.get());
    auto box  = koterm::dom::layout::HBox::create(screen.get());

    text->set_text("Hello world!");

    text->modify_decoration().set<TextDecoration::Decoration::BOLD>();
    text->modify_alignment().set_vertical(Alignment::Vertical::CENTER);
    text->modify_alignment().set_horizontal(Alignment::Horizontal::CENTER);

    screen->set_document(text);
    screen->main_loop();

    return EXIT_OK;
}
