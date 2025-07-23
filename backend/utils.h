#include <string>

const char* get_home_directory();

#ifndef UTILS_H
#define UTILS_H

inline std::string separator()
{
#ifdef _WIN32
    return std::string("\\");
#else
    return std::string("/");
#endif
}

#endif
