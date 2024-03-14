#include "koterm/component/Alignment.h"
#include "koterm/component/Border.h"
#include "koterm/terminal/terminal.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <koterm/dom/Text.h>
#include <koterm/dom/input/Button.h>
#include <koterm/dom/layout/Box.h>
#include <koterm/screen/InteractScreen.h>
#include <koterm/terminal/TerminalGuard.h>
#include <koterm/terminal/colors.h>
#include <memory>
#include <string>
#include <vector>

constexpr int EXIT_OK   = 0;
constexpr int EXIT_FAIL = 1;

using koterm::component::Alignment;
using koterm::component::Border;
using koterm::dom::input::Button;
using koterm::terminal::colors;
using koterm::terminal::FeatureFlags;

void style_button(std::shared_ptr<Button>& btn) {
    btn->modify_border().set_style(Border::Style::THIN);
    btn->set_background(colors::SYS_BRIGHT_BLACK);

    auto& text       = btn->modify_text();
    auto& text_align = text.modify_alignment();
    text_align.set_horizontal(Alignment::Horizontal::CENTER);
    text_align.set_vertical(Alignment::Vertical::CENTER);
}

int main() {

    using koterm::screen::InteractScreen;
    using koterm::terminal::TerminalGuard;

    using koterm::dom::Text;
    using koterm::dom::layout::HBox;
    using koterm::dom::layout::VBox;

    // Defining required terminal features
    constexpr auto required_features = std::array {
        FeatureFlags::ALTERNATIVE_SCREEN,
        FeatureFlags::NO_ECHO_AND_WAITING,
        FeatureFlags::MOUSE_SUPPORT,
    };

    // Creating an instance of TerminalGuard to manage terminal initialization and cleanup.
    TerminalGuard term;

    // Initializing the terminal with the required features.
    if (!term.init(required_features)) {
        std::cout << "ERROR: " << koterm::terminal::error_msg() << '\n';
        return EXIT_FAIL;
    }

    auto dims   = koterm::terminal::dimensions();
    auto screen = InteractScreen::create(dims.width, dims.height);
    // alternative is to set base width and height
    // InteractScreen::create(50, 20);

    // Disable screen buffer growing
    screen->set_grow(false);

    // Creating UI elements for the interactive screen.
    auto box          = VBox::create(screen.get());
    auto counter_text = Text::create(screen.get());
    auto btn_exit     = Button::create(screen.get());

    auto btn_add     = Button::create(screen.get());
    auto btn_sub     = Button::create(screen.get());
    auto buttons_box = HBox::create(screen.get(), { btn_add, btn_sub });

    // Styling the exit button.
    style_button(btn_exit);

    // Handling exit button press event.
    btn_exit->on_press().connect([&]() { screen->exit_loop(); });

    // Setting text for the exit button.
    {
        auto& text = btn_exit->modify_text();
        text.set_text("Exit");
    }

    // Styling the add button.
    style_button(btn_add);
    btn_add->set_grow(1, 1);

    // Styling the subtract button.
    style_button(btn_sub);
    btn_sub->set_grow(1, 1);

    // Handling add button press event and updating the counter.
    int counter = 0;

    btn_add->on_press().connect([&]() {
        counter += 1;
        counter_text->set_text(std::to_string(counter));
    });

    // Handling subtract button press event and updating the counter.
    btn_sub->on_press().connect([&]() {
        counter -= 1;
        counter_text->set_text(std::to_string(counter));
    });

    // Setting text for the add button.
    {
        auto& text = btn_add->modify_text();
        text.set_text("+");
    }
    // Setting text for the subtract button.
    {
        auto& text = btn_sub->modify_text();
        text.set_text("-");
    }

    // Setting initial value for the counter text.
    counter_text->set_text("0");

    // Adding UI elements to the vertical box layout.
    box->add_element(counter_text);
    box->add_element(buttons_box);
    box->add_element(btn_exit);

    // Setting the document (UI layout) for the interactive screen.
    screen->set_document(box);

    // Entering the main event loop for the interactive screen.
    screen->main_loop();

    return EXIT_OK;
}
