#ifndef KOTERM_UTIL_OS_H
#define KOTERM_UTIL_OS_H

namespace koterm::util {

#if defined(__linux__)
#define OS_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_APPLE
#elif defined(_WIN32) || defined(_WIN64)
#define OS_WINDOWS
#else
#error "Error: Unsupported operating system"
#endif

}

#endif
