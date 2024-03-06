#ifndef KOTERM_EXCEPTIONS_H
#define KOTERM_EXCEPTIONS_H

#include <stdexcept>
#include <string>
namespace koterm::exception {

class KotermException : public std::runtime_error {
public:
    KotermException(const char* msg)
        : std::runtime_error(msg)
        , m_msg(msg) { }
    KotermException(const std::string& msg)
        : std::runtime_error(msg)
        , m_msg(msg) { }

    ~KotermException() noexcept override = default;

    [[nodiscard]] const char* what() const noexcept override { return m_msg.c_str(); }

private:
    std::string m_msg;
};

class InvalidPositionException : public KotermException {
public:
    using KotermException::KotermException;
};

class InvalidRangeException : public KotermException {
public:
    using KotermException::KotermException;
};

}

#endif
