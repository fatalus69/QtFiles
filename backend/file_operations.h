#pragma once
#include <string>
#include <vector>

struct FileEntry {
    std::string filename;
    bool is_directory;
    int filesize;
    int match_score;
};

std::vector<FileEntry> list_files(const std::string& path, bool hide_hidden_files);
std::vector<FileEntry> search_in_dir(const std::string& search_directory, std::string& search_term);
bool check_for_dir_contents(const std::string& directory_path);

std::string to_lowercase(std::string str);