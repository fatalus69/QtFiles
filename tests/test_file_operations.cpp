#include <iostream>
#include <filesystem>
#include <gtest/gtest.h>

#include "../backend/file_operations.h"

TEST(file_operation_test, toLowerCase) {
    EXPECT_EQ(toLowercase("HelloWorld"), "helloworld");
    EXPECT_EQ(toLowercase("123ABC!"), "123abc!");
}

TEST(file_operation_test, getFormattedByte) {
    EXPECT_EQ(getFormattedByte(0), "0 B");
    EXPECT_EQ(getFormattedByte(2048), "2.00 KiB");
    EXPECT_EQ(getFormattedByte(1048576), "1.00 MiB");
}
