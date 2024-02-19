#include "koterm/terminal/terminal.h"
#include "koterm/Dimensions.h"
#include "koterm/terminal/ansi.h"
#include "koterm/util/discard.h"
#include "koterm/util/os.h"
#include "koterm/util/safe_getenv.h"
#include <csignal>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <string_view>

#ifdef OS_LINUX

#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#endif

namespace koterm::terminal {

struct TermInfo {
    ColorSupport color_support;
    Dimensions dimensions;
    Features features;
    std::string error;

    std::stack<std::function<void()>> on_exit;

    void exit() {
        while (!on_exit.empty()) {
            const auto& func = on_exit.top();
            func();
            on_exit.pop();
        }
    }

    ~TermInfo() { exit(); }
};
void update_dimensions();

static std::unique_ptr<TermInfo> g_terminfo = nullptr;

bool contains(std::string_view str, const char* value) {
#ifdef __cpp_lib_string_contains
    return str.contains(value);
#else
    return str.find(value) != std::string_view::npos;
#endif
}

ColorSupport get_color_support() {

    std::string_view colorterm = util::safe_getenv("COLORTERM");
    if (contains(colorterm, "24bit") || contains(colorterm, "truecolor")) {
        return ColorSupport::TRUE_COLOR;
    }

    std::string_view term = util::safe_getenv("TERM");
    if (contains(colorterm, "256") || contains(term, "256")) {
        return ColorSupport::COLOR256;
    }

#ifdef OS_WINDOWS
    return ColorSupport::TRUE_COLOR;
#else
    return ColorSupport::COLOR16;
#endif
}

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

bool has_initialized() { return g_terminfo != nullptr; }

bool init(Features features = FeatureFlags::NONE) {
    if (has_initialized()) {
        return true;
    }

    g_terminfo                = std::make_unique<TermInfo>();
    g_terminfo->color_support = get_color_support();
    g_terminfo->features      = features;

    if (features.contains<FeatureFlags::SUPPORT_TRUE_COLOR>()
        && g_terminfo->color_support != ColorSupport::TRUE_COLOR) {
        g_terminfo->exit();
        g_terminfo->error = "the terminal does not support true color";
        return false;
    }

    g_terminfo->on_exit.emplace([]() {
        std::cout << ansi::STYLE_RESET;
        std::cout.flush();
    });

    for (auto&& signal : { SIGTERM, SIGSEGV, SIGINT, SIGILL, SIGABRT, SIGFPE }) {
        add_signal_handler(signal);
    }

    // line wrapping
    disable_feature(ansi::Features::AUTO_WRAP);

    if (features.contains<FeatureFlags::MOUSE_SUPPORT>()) {
        // enable mouse tracking
        enable_feature(ansi::Features::VT200_MOUSE);
        // report all events
        enable_feature(ansi::Features::ANY_EVENT_MOUSE);
        // Extend position from 223 to 2015
        enable_feature(ansi::Features::EXT_MODE_MOUSE);
    }

    if (features.contains<FeatureFlags::ALTERNATIVE_SCREEN>()) {
        enable_feature(ansi::Features::ALTERNATIVE_SCREEN);
    }

#ifdef OS_LINUX

    add_signal_handler(SIGWINCH);
    add_signal_handler(SIGQUIT);

    if (features.contains<FeatureFlags::NO_ECHO_AND_WAITING>()) {

        // https://man7.org/linux/man-pages/man3/termios.3.html
        struct termios term;
        if (tcgetattr(STDIN_FILENO, &term) != 0) {
            g_terminfo->exit();
            g_terminfo->error = "failed to retrieve terminal information";
            return false;
        }

        // reset terminal to previous state
        g_terminfo->on_exit.emplace([term] { tcsetattr(STDIN_FILENO, TCSANOW, &term); });

        term.c_lflag &= ~ECHO; // no echo
        term.c_lflag &= ~ICANON; // noncanonical mode

        // allow polling
        term.c_cc[VTIME] = 0;
        term.c_cc[VMIN]  = 0;

        if (tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0) {
            g_terminfo->exit();
            g_terminfo->error = "failed to set terminal attributes";
            return false;
        }
    }

#endif
    update_dimensions();

    std::cout.flush();

    return true;
}

ColorSupport color_support() { return g_terminfo->color_support; }
Dimensions dimensions() { return g_terminfo->dimensions; }
Features features() { return g_terminfo->features; }
std::string_view error_msg() { return g_terminfo->error; }

bool register_exit_handle(std::function<void()> handle) {
    if (!has_initialized()) {
        return false;
    }

    g_terminfo->on_exit.push(handle);
    return true;
}

#ifdef OS_LINUX
bool input_available() {
    timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 0;

    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv);

    return FD_ISSET(STDIN_FILENO, &fds);
}

void update_dimensions() {
    winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        ws.ws_row = 0;
        ws.ws_col = 0;
    }

    g_terminfo->dimensions.width  = ws.ws_col;
    g_terminfo->dimensions.height = ws.ws_row;
}

bool is_tty() { return isatty(STDIN_FILENO) == 1; }
#endif

void handle_signal(int sig) {
    switch (sig) {
    case SIGTERM:
    case SIGSEGV:
    case SIGINT:
    case SIGILL:
    case SIGABRT:
    case SIGFPE:
    case SIGQUIT:
        g_terminfo.reset();
        std::quick_exit(1);
        break;
#ifdef OS_LINUX
    case SIGWINCH:
        update_dimensions();
        break;
#endif
    default:
        break;
    }
}

}
