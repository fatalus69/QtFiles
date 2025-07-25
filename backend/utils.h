#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstdlib>

#ifdef _WIN32
    constexpr char PATH_SEPARATOR = '\\';
#else
    constexpr char PATH_SEPARATOR = '/';
#endif

inline std::string separator() {
    return std::string(1, PATH_SEPARATOR);
}

inline std::string get_home_directory() {
#ifdef _WIN32
    const char* homedir = std::getenv("USERPROFILE");
#else
    const char* homedir = std::getenv("HOME");
#endif
    return homedir ? std::string(homedir) : std::string{};
}

#endif