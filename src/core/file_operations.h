#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <cctype>

#include "../utils/utils.h"
#include "../utils/types.h"

// Listings
std::vector<FileEntry> listFiles(const std::string& directory_path, bool hide_hidden_files = true);
std::vector<FileEntry> searchDirectory(const std::string& directory_path, std::string& query);

long long getFileSize(const std::filesystem::directory_entry& entry, filesize default_size = 2048);

/**
 * TODO: Perhaps rename functions, since those are the general functions for 
 * renaming both files and directories. 
 */
bool renameFile(const std::string& full_path, const std::string& new_path);
bool createFile(const std::string& full_path, FileType type);
bool deleteFile(const std::string& full_path);