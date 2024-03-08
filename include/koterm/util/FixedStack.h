#ifndef KOTERM_UTIL_FIXEDSTACK_H
#define KOTERM_UTIL_FIXEDSTACK_H

#include <array>
#include <cstdint>

namespace koterm::util
{

template<typename T, std::size_t Size> class FixedStack {
public:
    bool push(const T& val) {
        if (m_size >= Size)
        {
            return false;
        }

        m_storage[m_size] = val;
        m_size += 1;
        return true;
    }

    bool push(T&& val) {
        if (m_size >= Size) {
            return false;
        }

        m_storage[m_size] = std::move(val);
        m_size += 1;
        return true;
    }

    void pop() {
        if (m_size > 0) {
            m_size -= 1;
        }
    }

    [[nodiscard]] const T& top() const { return m_storage[m_size - 1];
    }

    std::size_t size() const { return m_size;  }
    bool empty() const { return m_size == 0; }

private:
    std::array<T, Size> m_storage;
    std::size_t m_size = 0;

};
}

#endif
