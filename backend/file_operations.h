#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <cctype>

struct FileEntry {
    std::string name;
    std::string path;
    bool is_directory;
    long long size;
    int match_score;
};

enum class FileType {
    File,
    Directory
};

// Listings
std::vector<FileEntry> listFiles(const std::string& directory_path, bool hide_hidden_files);
std::vector<FileEntry> searchDirectory(const std::string& directory_path, std::string& query);

/**
 * TODO: Perhaps rename functions, since those are the general functions for 
 * renaming both files and directories. 
 */
bool renameFile(const std::string& full_path, const std::string& new_path);
bool createFile(const std::string& full_path, FileType type);
bool deleteFile(const std::string& full_path);