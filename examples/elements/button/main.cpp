#include <array>
#include <iostream>
#include <koterm/component/Alignment.h>
#include <koterm/component/Border.h>
#include <koterm/component/TextDecoration.h>
#include <koterm/dom/Element.h>
#include <koterm/dom/Text.h>
#include <koterm/dom/input/Button.h>
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

using koterm::dom::input::Button;
using koterm::terminal::colors;

constexpr int EXIT_OK   = 0;
constexpr int EXIT_FAIL = 1;

void style_button(std::shared_ptr<Button>& btn) {
    btn->modify_border().set_style(Border::Style::THIN);
    btn->set_background(colors::SYS_BRIGHT_BLACK);

    auto& text       = btn->modify_text();
    auto& text_align = text.modify_alignment();
    text_align.set_horizontal(Alignment::Horizontal::CENTER);
    text_align.set_vertical(Alignment::Vertical::CENTER);
}

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

    auto buttons_container = HBox::create(screen.get());
    auto add_btn           = Button::create(screen.get());
    auto sub_btn           = Button::create(screen.get());

    int value = 10;

    add_btn->modify_text().set_text("+1");
    style_button(add_btn);

    add_btn->on_press().connect([&value, &text]() {
        value += 1;
        text->set_text(std::to_string(value));
    });

    sub_btn->modify_text().set_text("-1");
    style_button(sub_btn);

    sub_btn->on_press().connect([&value, &text]() {
        value -= 1;
        text->set_text(std::to_string(value));
    });

    text->set_text(std::to_string(value));

    buttons_container->add_element(sub_btn);
    buttons_container->add_element(add_btn);

    box->add_element(text);
    box->add_element(buttons_container);

    screen->set_document(box);
    screen->main_loop();

    return EXIT_OK;
}
