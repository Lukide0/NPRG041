#include <array>
#include <iostream>
#include <koterm/component/Alignment.h>
#include <koterm/component/Border.h>
#include <koterm/component/TextDecoration.h>
#include <koterm/dom/Element.h>
#include <koterm/dom/Text.h>
#include <koterm/dom/input/Button.h>
#include <koterm/dom/input/Checkbox.h>
#include <koterm/dom/input/Input.h>
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

using koterm::dom::input::Input;

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

    auto input = Input::create(screen.get());

    input->on_change().connect([&text](const std::string& value) { text->set_text(value); });
    input->set_value("sample value");
    input->focus();

    box->add_element(text);
    box->add_element(input);

    screen->set_document(box);
    screen->main_loop();

    return EXIT_OK;
}
