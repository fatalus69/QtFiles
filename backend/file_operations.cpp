#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <cctype>
#include "file_operations.h"

/**
 * We currently crash when entering an invalid path.
 * Perhaps prevent this by returning and empty FileEntry vector?
 */
std::vector<FileEntry> listFiles(const std::string& directory_path, bool hide_hidden_files) {
    std::vector<FileEntry> result;
    for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
        FileEntry file_entry;
        std::string filename = entry.path().filename().string();
        
        if (hide_hidden_files == true && filename.rfind(".", 0) == 0) {
            continue;
        }
        long long default_size = 2048; // Default size of directories on Unix
        
        file_entry.name = entry.path().filename().string();
        file_entry.path = entry.path().string();
        file_entry.is_directory = entry.is_directory();
        file_entry.size = !file_entry.is_directory ? getFormattedByte(static_cast<long long>(entry.file_size())) : getFormattedByte(default_size);

        result.push_back(file_entry);
    }

    std::sort(result.begin(), result.end(), [](const FileEntry& a, const FileEntry& b) {
        return a.name < b.name;
    });

    return result;
}

std::string getFormattedByte(long long bytes) {
    const double KB = 1024.0;
    const double MB = KB * 1024;
    const double GB = MB * 1024;
    const double TB = GB * 1024;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    if (bytes >= TB) {
        oss << (bytes / TB) << " TiB";
    } else if (bytes >= GB) {
        oss << (bytes / GB) << " GiB";
    } else if (bytes >= MB) {
        oss << (bytes / MB) << " MiB";
    } else if (bytes >= KB) {
        oss << (bytes / KB) << " KiB";
    } else {
        oss << bytes << " B";
    }

    return oss.str();
}

std::string toLowercase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

int levenshtein(const std::string& s1, const std::string& s2) {
    const size_t len1 = s1.size(), len2 = s2.size();
    std::vector<std::vector<int>> dp(len1 + 1, std::vector<int>(len2 + 1));

    for (size_t i = 0; i <= len1; ++i) dp[i][0] = i;
    for (size_t j = 0; j <= len2; ++j) dp[0][j] = j;

    for (size_t i = 1; i <= len1; ++i) {
        for (size_t j = 1; j <= len2; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::min({ dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + 1 });
            }
        }
    }

    return dp[len1][len2];
}


/**
 * Add a default return value when there are no results.
 */
std::vector<FileEntry> searchDirectory(const std::string& directory_path, std::string& query) {
    //better results for smaller search terms
    int max_distance = std::max(1, static_cast<int>(query.length() * 0.3));

    std::vector<FileEntry> result;
    std::string search_lc = toLowercase(query);

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory_path)) {
        if (!entry.is_regular_file() && !entry.is_directory()) {
            continue;
        }

        std::string filename = entry.path().filename().string();
        std::string filename_lc = toLowercase(filename);

        int distance;
        if (filename_lc.find(search_lc) != std::string::npos) {
            distance = 0;
        } else {
            distance = levenshtein(search_lc, filename_lc);
        }

        if (distance <= max_distance) {
            FileEntry file_entry;
            file_entry.name = filename;
            file_entry.path = entry.path().string();
            file_entry.is_directory = entry.is_directory();
            file_entry.match_score = distance;

            result.push_back(file_entry);
        }
    }

    // Sort by best result
    std::sort(result.begin(), result.end(), [](const FileEntry& a, const FileEntry& b) {
        return a.match_score < b.match_score;
    });

    return result;
}

void renameFile(const std::string& full_path, const std::string& new_path) {
    namespace fs = std::filesystem;

    fs::path src(full_path);
    fs::path destination = new_path;

    if (fs::exists(src)) {
        fs::rename(src, destination);
    }
}
