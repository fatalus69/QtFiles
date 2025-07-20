#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include "file_operations.h"

std::vector<FileEntry> list_files(const std::string& path, bool hide_hidden_files) {
    std::vector<FileEntry> result;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        FileEntry file_entry;
        std::string filename = entry.path().filename().string();
        
        if (hide_hidden_files == true && filename.rfind(".", 0) == 0) {
            continue;
        }
        
        file_entry.filename = entry.path().filename().u8string();
        file_entry.is_directory = entry.is_directory();

        result.push_back(file_entry);
    }

    //sort files alphabetically
    std::sort(result.begin(), result.end(), [](const FileEntry& a, const FileEntry& b) {
        return a.filename < b.filename;
    });

    return result;
}

bool check_for_dir_contents(const std::string& path) {
    try {
        return !std::filesystem::is_empty(path);
    } catch (const std::filesystem::filesystem_error& e) {
        return true; // Assume it's not empty if we can't check
    }
}

// TODO: search through subdirectories too
std::vector<FileEntry> search_in_dir(const std::string& search_directory, std::string& search_term) {
    std::vector<FileEntry> result;
    for (const auto& entry : std::filesystem::directory_iterator(search_directory)) {
        FileEntry file_entry;
        std::string filename = entry.path().filename().u8string();
        
        //Make lowercase for case-insensitive comparison
        filename = to_lowercase(filename);
        search_term = to_lowercase(search_term);

        if (filename.find(search_term) == std::string::npos) {
            continue;
        }
        
        file_entry.filename = entry.path().filename().string();
        file_entry.is_directory = entry.is_directory();

        result.push_back(file_entry);
    }

    //sort files alphabetically
    std::sort(result.begin(), result.end(), [](const FileEntry& a, const FileEntry& b) {
        return a.filename < b.filename;
    });

    return result;
}

std::string to_lowercase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}