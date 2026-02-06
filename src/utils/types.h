#pragma once

#include <string>

using filesize = long long;

enum class FileType {
    File,
    Directory
};

struct FileEntry {
    std::string name;
    std::string path;
    FileType type;
    filesize size;
    int match_score;
};

struct FileProperties {
    std::string path;
    int access_time;
    int modification_time;
    int creation_time;
};
// UI: maybe move this into a gui/types.h file?
// Perhpas add Favourites, ...
enum class ListMode {
    Normal,
    Search
};