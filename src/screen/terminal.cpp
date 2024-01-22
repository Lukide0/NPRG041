#include "koterm/screen/terminal.h"
#include "koterm/screen/ansi.h"
#include "koterm/util/discard.h"
#include "koterm/util/os.h"
#include "koterm/util/safe_getenv.h"
#include <csignal>
#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <string_view>

namespace koterm::screen {

struct TermInfo {
    terminal::ColorSupport color_support;
    terminal::Dimensions dimensions;
    terminal::Features features;

    std::stack<std::function<void()>> on_exit;

    ~TermInfo() {
        while (!on_exit.empty()) {
            const auto& func = on_exit.top();
            func();
            on_exit.pop();
        }
    }
};

static std::unique_ptr<TermInfo> g_terminfo = nullptr;

bool contains(std::string_view str, const char* value) {
#ifdef __cpp_lib_string_contains
    return str.contains(value);
#else
    return str.find(value) != std::string_view::npos;
#endif
}

terminal::ColorSupport get_color_support() {

    std::string_view colorterm = util::safe_getenv("COLORTERM");
    if (contains(colorterm, "24bit") || contains(colorterm, "truecolor")) {
        return terminal::ColorSupport::TRUE_COLOR;
    }

    std::string_view term = util::safe_getenv("TERM");
    if (contains(colorterm, "256") || contains(term, "256")) {
        return terminal::ColorSupport::COLOR256;
    }

#ifdef OS_WINDOWS
    return ColorSupport::TRUE_COLOR;
#else
    return terminal::ColorSupport::COLOR16;
#endif
}

void handle_signal(int sig);

void add_signal_handler(int sig) {
    auto old_handler = std::signal(sig, handle_signal);
    g_terminfo->on_exit.emplace([=]() { util::discard = std::signal(sig, old_handler); });
}

void enable_feature(ansi::Features feature) {
    std::cout << ansi::CPM << static_cast<int>(feature) << 'h';
    g_terminfo->on_exit.emplace([feature]() { std::cout << ansi::CPM << static_cast<int>(feature) << 'l'; });
}

void disable_feature(ansi::Features feature) {
    std::cout << ansi::CPM << static_cast<int>(feature) << 'l';
    g_terminfo->on_exit.emplace([feature]() { std::cout << ansi::CPM << static_cast<int>(feature) << 'h'; });
}

void handle_signal([[maybe_unused]] int sig) { }

bool terminal::init(Features features = FeatureFlags::NONE) {
    if (g_terminfo != nullptr) {
        return true;
    }

    g_terminfo                = std::make_unique<TermInfo>();
    g_terminfo->color_support = get_color_support();
    g_terminfo->features      = features;

    g_terminfo->on_exit.emplace([]() {
        std::cout << ansi::STYLE_RESET;
        std::cout.flush();
    });

    // line wrapping
    disable_feature(ansi::Features::AUTO_WRAP);

    if (features.contains<FeatureFlags::MOUSE_SUPPORT>()) {
        // enable mouse tracking
        enable_feature(ansi::Features::VT200_MOUSE);
        // report all events
        enable_feature(ansi::Features::ANY_EVENT_MOUSE);
        // Extend position from 223 to 2015
        enable_feature(ansi::Features::EXT_MODE_MOUSE);
        // Use SGR format for reporting
        enable_feature(ansi::Features::SRG_EXT_MODE_MOUSE);
    }

    if (features.contains<FeatureFlags::ALTERNATIVE_SCREEN>()) {
        enable_feature(ansi::Features::ALTERNATIVE_SCREEN);
    }

    if (features.contains<FeatureFlags::SUPPORT_TRUE_COLOR>()
        && g_terminfo->color_support != ColorSupport::TRUE_COLOR) {
        return false;
    }

    return true;
}

terminal::ColorSupport terminal::color_support() { return g_terminfo->color_support; }
terminal::Dimensions terminal::dimensions() { return g_terminfo->dimensions; }
terminal::Features terminal::features() { return g_terminfo->features; }
}
