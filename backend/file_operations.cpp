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
        long long default_size = 2048; // Size of directories on Unix systems
        
        file_entry.filename = reinterpret_cast<const char*>(entry.path().filename().u8string().c_str());        
        file_entry.is_directory = entry.is_directory();
        file_entry.filesize = !file_entry.is_directory ? get_formatted_byte(static_cast<long long>(entry.file_size())) : get_formatted_byte(default_size);

        result.push_back(file_entry);
    }

    std::sort(result.begin(), result.end(), [](const FileEntry& a, const FileEntry& b) {
        return a.filename < b.filename;
    });

    return result;
}

std::string get_formatted_byte(long long bytes) {
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

bool check_for_dir_contents(const std::string& path) {
    try {
        return !std::filesystem::is_empty(path);
    } catch (const std::filesystem::filesystem_error& e) {
        return true; // Assume it's not empty if we can't check
    }
}

std::string to_lowercase(std::string str) {
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

std::vector<FileEntry> search_in_dir(const std::string& search_directory, std::string& search_term) {
    //better results for smaller search terms
    int max_distance = std::max(1, static_cast<int>(search_term.length() * 0.3));

    std::vector<FileEntry> result;
    std::string search_lc = to_lowercase(search_term);

    for (const auto& entry : std::filesystem::recursive_directory_iterator(search_directory)) {
        if (!entry.is_regular_file() && !entry.is_directory()) {
            continue;
        }

        std::string filename = reinterpret_cast<const char*>(entry.path().filename().u8string().c_str());
        std::string filename_lc = to_lowercase(filename);

        int distance;
        if (filename_lc.find(search_lc) != std::string::npos) {
            distance = 0;
        } else {
            distance = levenshtein(search_lc, filename_lc);
        }

        if (distance <= max_distance) {
            FileEntry file_entry;
            file_entry.filename = reinterpret_cast<const char*>(entry.path().filename().u8string().c_str());
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