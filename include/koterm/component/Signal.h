#ifndef KOTERM_COMPONENT_SIGNAL_H
#define KOTERM_COMPONENT_SIGNAL_H

#include <cstddef>
#include <functional>
#include <unordered_set>
namespace koterm::component {

/**
 * @brief A signal class for implementing observer patterns.
 *
 * @tparam Args The types of arguments expected by the signal.
 */
template <typename... Args> class Signal {
public:
    using delegate_t = std::function<void(Args...)>;

    Signal()                         = default;
    Signal(const Signal&)            = delete;
    Signal(Signal&&)                 = delete;
    Signal& operator=(Signal&&)      = delete;
    Signal& operator=(const Signal&) = delete;

    /**
     * @brief Connects a delegate function to the signal.
     * @param delegate The delegate function to connect.
     */
    void connect(delegate_t delegate);

    /**
     * @brief Disconnects a delegate function from the signal.
     * @param delegate The delegate function to disconnect.
     */
    void disconnect(delegate_t delegate);

    /**
     * @brief Blocks the signal from emitting.
     */
    void block() { m_blocking = true; }

    /**
     * @brief Unblocks the signal, allowing it to emit again.
     */
    void unblock() { m_blocking = false; }

    /**
     * @brief Emits the signal with the given arguments.
     * @param args The arguments to be passed to the connected delegate functions.
     */
    void emit(Args... args);
    void operator()(Args... args);

private:
    struct Hash {
        std::size_t operator()(const delegate_t& value) const { return reinterpret_cast<std::size_t>(&value); }
    };

    struct Comp {
        bool operator()(const delegate_t& lhs, const delegate_t& rhs) const { return &lhs == &rhs; }
    };

    std::unordered_set<delegate_t, Hash, Comp> m_delegates;
    bool m_blocking = false;
};

template <typename... Args> void Signal<Args...>::connect(delegate_t delegate) { m_delegates.insert(delegate); }
template <typename... Args> void Signal<Args...>::disconnect(delegate_t delegate) { m_delegates.erase(delegate); }

template <typename... Args> void Signal<Args...>::emit(Args... args) {
    if (m_blocking) {
        return;
    }

    for (auto&& delegate : m_delegates) {
        delegate(args...);
    }
}
template <typename... Args> void Signal<Args...>::operator()(Args... args) { emit(args...); }

}
#endif
