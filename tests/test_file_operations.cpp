#include <iostream>
#include <filesystem>
#include <gtest/gtest.h>

#include "../backend/file_operations.h"

namespace fs = std::filesystem;

fs::path base_test_dir = fs::path(__FILE__).parent_path();

fs::path original_test_dir = base_test_dir / "test_dir";
fs::path original_test_file = original_test_dir / "test_file.txt";

fs::path modified_test_dir = base_test_dir / "modified_dir";
fs::path modified_test_file = modified_test_dir / "modified_file.txt";


TEST(file_operation_test, toLowerCase) {
    EXPECT_EQ(toLowercase("HelloWorld"), "helloworld");
    EXPECT_EQ(toLowercase("123ABC!"), "123abc!");
}

TEST(file_operation_test, getFormattedByte) {
    EXPECT_EQ(getFormattedByte(0), "0 B");
    EXPECT_EQ(getFormattedByte(2048), "2.00 KiB");
    EXPECT_EQ(getFormattedByte(1048576), "1.00 MiB");
}

// Test file creation, renaming and deletion.
// Test once for true and once for false.
TEST(file_operation_test, createFile) {
    EXPECT_EQ(createFile(original_test_dir, FileType::Directory), true);
    EXPECT_EQ(createFile(original_test_dir, FileType::Directory), false);

    EXPECT_EQ(createFile(original_test_file, FileType::File), true);
    EXPECT_EQ(createFile(original_test_file, FileType::File), false);
}

TEST(file_operation_test, renameFile) {
    EXPECT_EQ(renameFile(original_test_dir, modified_test_dir), true);
}

TEST(file_operation_test, deleteFile) {
    EXPECT_EQ(deleteFile(modified_test_dir), true);
    EXPECT_EQ(deleteFile(modified_test_dir), false);
}