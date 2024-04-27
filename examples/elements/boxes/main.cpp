#include <array>
#include <cstddef>
#include <iostream>
#include <koterm/component/Alignment.h>
#include <koterm/component/TextDecoration.h>
#include <koterm/dom/Element.h>
#include <koterm/dom/Text.h>
#include <koterm/dom/layout/Box.h>
#include <koterm/screen/InteractScreen.h>
#include <koterm/terminal/TerminalGuard.h>
#include <koterm/terminal/terminal.h>
#include <string>

using koterm::dom::Text;
using koterm::screen::InteractScreen;
using koterm::terminal::FeatureFlags;
using koterm::terminal::TerminalGuard;

using koterm::component::Alignment;
using koterm::component::TextDecoration;

using koterm::dom::layout::BoxDirection;
using koterm::dom::layout::HBox;
using koterm::dom::layout::VBox;

constexpr int EXIT_OK   = 0;
constexpr int EXIT_FAIL = 1;

koterm::dom::element_t create_table(std::size_t width, std::size_t height, InteractScreen* screen) {
    auto table = VBox::create(screen);

    for (std::size_t y = 0; y < height; ++y) {
        auto row = HBox::create(screen);

        for (std::size_t x = 0; x < width; ++x) {
            auto cell = Text::create(screen);

            cell->set_text(std::to_string(x + y * width));
            cell->modify_decoration().set<TextDecoration::Decoration::BOLD>();

            row->add_element(cell);
        }

        table->add_element(row);
    }

    return table;
}

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

    auto table = create_table(5, 5, screen.get());

    screen->set_document(table);
    screen->main_loop();

    return EXIT_OK;
}
