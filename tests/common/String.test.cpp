//// String.test.cpp //////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for common String utilities (iequals)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "String.hpp"

namespace nexo {

// =============================================================================
// iequals Tests - Case-insensitive string comparison
// =============================================================================

class IEqualsTest : public ::testing::Test {};

TEST_F(IEqualsTest, EqualStringsLowercase) {
    EXPECT_TRUE(iequals("hello", "hello"));
}

TEST_F(IEqualsTest, EqualStringsUppercase) {
    EXPECT_TRUE(iequals("HELLO", "HELLO"));
}

TEST_F(IEqualsTest, EqualStringsMixedCase) {
    EXPECT_TRUE(iequals("Hello", "hELLO"));
}

TEST_F(IEqualsTest, EqualStringsAllCapsVsAllLower) {
    EXPECT_TRUE(iequals("WORLD", "world"));
}

TEST_F(IEqualsTest, DifferentStrings) {
    EXPECT_FALSE(iequals("hello", "world"));
}

TEST_F(IEqualsTest, DifferentLengthsFirstLonger) {
    EXPECT_FALSE(iequals("hello", "hel"));
}

TEST_F(IEqualsTest, DifferentLengthsSecondLonger) {
    EXPECT_FALSE(iequals("hel", "hello"));
}

TEST_F(IEqualsTest, EmptyStrings) {
    EXPECT_TRUE(iequals("", ""));
}

TEST_F(IEqualsTest, EmptyVsNonEmpty) {
    EXPECT_FALSE(iequals("", "a"));
}

TEST_F(IEqualsTest, NonEmptyVsEmpty) {
    EXPECT_FALSE(iequals("a", ""));
}

TEST_F(IEqualsTest, SingleCharacterSameCase) {
    EXPECT_TRUE(iequals("a", "a"));
}

TEST_F(IEqualsTest, SingleCharacterDifferentCase) {
    EXPECT_TRUE(iequals("A", "a"));
}

TEST_F(IEqualsTest, SingleCharacterDifferent) {
    EXPECT_FALSE(iequals("a", "b"));
}

TEST_F(IEqualsTest, StringWithNumbers) {
    EXPECT_TRUE(iequals("Test123", "TEST123"));
}

TEST_F(IEqualsTest, NumbersOnly) {
    EXPECT_TRUE(iequals("12345", "12345"));
}

TEST_F(IEqualsTest, StringWithSpecialChars) {
    EXPECT_TRUE(iequals("Hello!@#", "HELLO!@#"));
}

TEST_F(IEqualsTest, StringWithSpaces) {
    EXPECT_TRUE(iequals("Hello World", "hello world"));
}

TEST_F(IEqualsTest, StringWithTabs) {
    EXPECT_TRUE(iequals("Hello\tWorld", "HELLO\tWORLD"));
}

TEST_F(IEqualsTest, MixedAlphanumericAndSymbols) {
    EXPECT_TRUE(iequals("Test_123-ABC", "test_123-abc"));
}

TEST_F(IEqualsTest, LongStrings) {
    std::string long1(1000, 'a');
    std::string long2(1000, 'A');
    EXPECT_TRUE(iequals(long1, long2));
}

TEST_F(IEqualsTest, LongStringsDifferent) {
    std::string long1(1000, 'a');
    std::string long2(1000, 'b');
    EXPECT_FALSE(iequals(long1, long2));
}

TEST_F(IEqualsTest, AlmostEqualLastCharDifferent) {
    EXPECT_FALSE(iequals("HelloA", "HelloB"));
}

TEST_F(IEqualsTest, AlmostEqualFirstCharDifferent) {
    EXPECT_FALSE(iequals("Aello", "Bello"));
}

TEST_F(IEqualsTest, StringViewCompatibility) {
    std::string_view sv1 = "Test";
    std::string_view sv2 = "TEST";
    EXPECT_TRUE(iequals(sv1, sv2));
}

TEST_F(IEqualsTest, FileExtensionComparison) {
    EXPECT_TRUE(iequals(".TXT", ".txt"));
    EXPECT_TRUE(iequals(".PNG", ".png"));
    EXPECT_TRUE(iequals(".Cpp", ".CPP"));
}

TEST_F(IEqualsTest, PathComparison) {
    EXPECT_TRUE(iequals("C:\\Users\\Test", "c:\\users\\test"));
}

}  // namespace nexo
