#include "file_operations.h"

namespace fs = std::filesystem;

/**
 * To see hidden files you'll have to change this value to false
 * until the Settings have been implemented.
 */
std::vector<FileEntry> listFiles(const std::string& directory_path, bool hide_hidden_files) {    
    std::vector<FileEntry> result;

    for (const auto& entry : fs::directory_iterator(directory_path)) {
        FileEntry file_entry;
        std::string filename = entry.path().filename().string();
        
        if (hide_hidden_files == true && filename.rfind(".", 0) == 0) {
            continue;
        }

        file_entry.name = entry.path().filename().string();
        file_entry.path = entry.path().string();
        file_entry.type = entry.is_directory() ? FileType::Directory : FileType::File;
        file_entry.size = getFileSize(entry);

        result.push_back(file_entry);
    }

    std::sort(result.begin(), result.end(), [](const FileEntry& a, const FileEntry& b) {
        return a.name < b.name;
    });

    return result;
}

int levenshtein(const std::string& s1, const std::string& s2, int max_dist) {
    const int len1 = s1.size();
    const int len2 = s2.size();

    if (std::abs(len1 - len2) > max_dist)
        return max_dist + 1;

    std::vector<int> prev(len2 + 1), curr(len2 + 1);

    for (int j = 0; j <= len2; ++j)
        prev[j] = j;

    for (int i = 1; i <= len1; ++i) {
        curr[0] = i;
        int min_in_row = curr[0];

        for (int j = 1; j <= len2; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            curr[j] = std::min({
                prev[j] + 1,
                curr[j - 1] + 1,
                prev[j - 1] + cost
            });
            min_in_row = std::min(min_in_row, curr[j]);
        }

        if (min_in_row > max_dist)
            return max_dist + 1;

        std::swap(prev, curr);
    }

    return prev[len2];
}

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
        } else if (std::abs((int)filename_lc.length() - (int)search_lc.length()) > max_distance) {
            continue;
        } else if (filename_lc[0] != search_lc[0]) {
            continue;
        } else {
            distance = levenshtein(search_lc, filename_lc, max_distance);
        }

        if (distance <= max_distance) {
            FileEntry file_entry;
            file_entry.name = entry.path().filename().string();
            file_entry.path = entry.path().string();
            file_entry.type = entry.is_directory() ? FileType::Directory : FileType::File;
            file_entry.size = getFileSize(entry);
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

bool renameFile(const std::string& full_path, const std::string& new_path) {
    fs::path src(full_path);
    fs::path destination = new_path;

    if (fs::exists(src)) {
        fs::rename(src, destination);
        return fs::exists(destination);
    }

    return false;
}

bool createFile(const std::string& full_path, FileType type) {
    fs::path path = full_path;

    if (fs::exists(path)) return false;
    
    switch (type)
    {
        case FileType::File : {
            fs::create_directories(path.parent_path());
            std::ofstream ofs(path);

            break;
        }
        case FileType::Directory : {
            fs::create_directory(path);
            
            break;
        }
        default:
            return false;
    }

    return fs::exists(path);
}

bool deleteFile(const std::string& full_path) {
    fs::path path = full_path;

    if (!fs::exists(path)) return false;

    fs::remove_all(path);
    return !fs::exists(path);
}

filesize getFileSize(const fs::directory_entry& entry, filesize default_size) {
    try {
        if (!entry.exists()) return default_size;

        if (entry.is_regular_file()) {
            std::error_code error_code;

            fs::perms permissions = entry.status(error_code).permissions();
            
            if (error_code) return default_size;

            if ((permissions & fs::perms::owner_read) == fs::perms::none
             && (permissions & fs::perms::group_read) == fs::perms::none
             && (permissions & fs::perms::others_read) == fs::perms::none) {
                return default_size;
            }

            std::uintmax_t file_size = entry.file_size(error_code);
            if (error_code) return default_size;
            return static_cast<filesize>(file_size);
        }
    } catch (const fs::filesystem_error&) {
        return default_size;
    }

    return default_size;
}