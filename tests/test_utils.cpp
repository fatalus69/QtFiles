#include <iostream>
#include <filesystem>
#include <gtest/gtest.h>

#include "../backend/utils.h"

TEST(utils_test, toLowerCase) {
    EXPECT_EQ(toLowercase("HelloWorld"), "helloworld");
    EXPECT_EQ(toLowercase("123ABC!"), "123abc!");
}

TEST(utils_test, formatByte) {
    EXPECT_EQ(formatByte(0), "0 B");
    EXPECT_EQ(formatByte(1048576), "1.00 MiB");

    double multiplier_10 = 1000.0;
    EXPECT_EQ(formatByte(0, multiplier_10), "0 B");
    EXPECT_EQ(formatByte(1000000, multiplier_10), "1.00 MiB");
}