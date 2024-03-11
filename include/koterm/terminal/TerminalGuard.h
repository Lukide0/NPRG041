#ifndef KOTERM_TERMINAL_TERMINALGUARD_H
#define KOTERM_TERMINAL_TERMINALGUARD_H

#include "koterm/terminal/terminal.h"

namespace koterm::terminal
{

class TerminalGuard
{
public:
    TerminalGuard() = default;
    TerminalGuard(TerminalGuard&&) = delete;
    TerminalGuard(const TerminalGuard&) = delete;

    bool init(Features features) {
        m_initialized |= koterm::terminal::init(features);
        return m_initialized;
    }


    ~TerminalGuard() {
        if (m_initialized) {
            koterm::terminal::cleanup();
        }
    }

private:
    bool m_initialized = false;

};

}


#endif
