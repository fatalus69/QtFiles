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

// UI:
// Perhpas add Favourites, ...
enum class ListMode {
    Normal,
    Search
};