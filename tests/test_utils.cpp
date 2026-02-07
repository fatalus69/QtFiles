#include <iostream>
#include <filesystem>
#include <gtest/gtest.h>

#include "../src/utils/utils.h"

TEST(utils_test, toLowerCase) {
    EXPECT_EQ(toLowercase("HelloWorld"), "helloworld");
    EXPECT_EQ(toLowercase("123ABC!"), "123abc!");
}

TEST(utils_test, formatByte) {
    EXPECT_EQ(formatByte(0), "0 B");
    EXPECT_EQ(formatByte(1048576), "1.00 M");

    double multiplier_10 = 1000.0;
    EXPECT_EQ(formatByte(0, multiplier_10), "0 B");
    EXPECT_EQ(formatByte(1000000, multiplier_10), "1.00 M");
}

TEST(utils_test, formatTimestamp) {
    int timestamp = 1767280763; // 01.01.2026 16:19:23 CET
    EXPECT_NE(formatTimestamp(0), "01.01.1970 00:00:00");
    EXPECT_EQ(formatTimestamp(timestamp), "01.01.2026 16:19:23");
}