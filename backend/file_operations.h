#pragma once
#include <string>
#include <vector>

struct FileEntry {
    std::string name;
    std::string path;
    bool is_directory;
    std::string size;
    int match_score;
};

std::vector<FileEntry> listFiles(const std::string& directory_path, bool hide_hidden_files);
std::vector<FileEntry> searchDirectory(const std::string& directory_path, std::string& query);
void renameFile(const std::string& full_path, const std::string& new_path);
std::string getFormattedByte(long long bytes);

std::string toLowercase(std::string str);
