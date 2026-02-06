#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstdlib>
#include <algorithm>
#include <QString>
#include <QDateTime>
#include <sstream>
#include <iomanip>

#include "types.h"

#ifdef _WIN32
    constexpr char PATH_SEPARATOR = '\\';
#else
    constexpr char PATH_SEPARATOR = '/';
#endif

inline std::string separator() {
    return std::string(1, PATH_SEPARATOR);
}

inline std::string getHomeDirectory() {
#ifdef _WIN32
    const char* homedir = std::getenv("USERPROFILE");
#else
    const char* homedir = std::getenv("HOME");
#endif
    return homedir ? std::string(homedir) : std::string{};
}

inline QString formatByte(filesize bytes, double multiplier = 1024.0) {
    const double KB = multiplier;
    const double MB = KB * multiplier;
    const double GB = MB * multiplier;
    const double TB = GB * multiplier;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    if (bytes >= TB) {
        oss << (bytes / TB) << " T";
    } else if (bytes >= GB) {
        oss << (bytes / GB) << " G";
    } else if (bytes >= MB) {
        oss << (bytes / MB) << " M";
    } else if (bytes >= KB) {
        oss << (bytes / KB) << " K";
    } else {
        oss << bytes << " B";
    }

    return QString::fromStdString(oss.str());
}

inline std::string toLowercase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

inline QString formatTimestamp(int timestamp) {
    return QDateTime::fromSecsSinceEpoch(timestamp).toString("dd-MM-yyy HH:mm:ss");
}

#endif
