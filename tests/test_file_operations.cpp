#include <iostream>
#include <filesystem>
#include <gtest/gtest.h>

#include "../src/core/file_operations.h"

namespace fs = std::filesystem;

fs::path base_test_dir = fs::path(__FILE__).parent_path();

fs::path original_test_dir = base_test_dir / "test_dir";
fs::path original_test_file = original_test_dir / "test_file.txt";

fs::path modified_test_dir = base_test_dir / "modified_dir";
fs::path modified_test_file = modified_test_dir / "modified_file.txt";

// Test file creation, renaming and deletion.
// Test once for true and once for false.
TEST(file_operation_test, createFile) {
    EXPECT_EQ(createFile(original_test_dir, FileType::Directory), true);
    EXPECT_EQ(createFile(original_test_dir, FileType::Directory), false);

    EXPECT_EQ(createFile(original_test_file, FileType::File), true);
    EXPECT_EQ(createFile(original_test_file, FileType::File), false);
}

TEST(file_operation_test, getFileSize) {
    filesize default_size = 2048;
    filesize custom_default_size = 1234;

    fs::directory_entry original_file(original_test_file);
    fs::directory_entry modified_file(modified_test_file);

    EXPECT_NE(getFileSize(original_file), default_size);
    EXPECT_NE(getFileSize(original_file, custom_default_size), custom_default_size);
    
    // Expect it to return default size on error.
    EXPECT_EQ(getFileSize(modified_file), default_size);
    EXPECT_EQ(getFileSize(modified_file, custom_default_size), custom_default_size);
}

TEST(file_operation_test, renameFile) {
    EXPECT_EQ(renameFile(original_test_dir, modified_test_dir), true);
}

TEST(file_operation_test, deleteFile) {
    EXPECT_EQ(deleteFile(modified_test_dir), true);
    EXPECT_EQ(deleteFile(modified_test_dir), false);
}