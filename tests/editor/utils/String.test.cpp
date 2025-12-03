//// String.test.cpp //////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        03/12/2025
//  Description: Test file for String utilities
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "utils/String.hpp"

namespace nexo::editor::utils {

    // ==========================================================================
    // removeIconPrefix Tests
    // ==========================================================================

    TEST(StringUtilsTest, RemoveIconPrefixStripsLeadingIcon) {
        std::string input = "icon text";
        EXPECT_EQ(removeIconPrefix(input), "text");
    }

    TEST(StringUtilsTest, RemoveIconPrefixWithMultipleSpaces) {
        std::string input = "icon text with spaces";
        EXPECT_EQ(removeIconPrefix(input), "text with spaces");
    }

    TEST(StringUtilsTest, RemoveIconPrefixEmptyString) {
        std::string input = "";
        EXPECT_EQ(removeIconPrefix(input), "");
    }

    TEST(StringUtilsTest, RemoveIconPrefixNoSpace) {
        std::string input = "nospace";
        EXPECT_EQ(removeIconPrefix(input), "nospace");
    }

    TEST(StringUtilsTest, RemoveIconPrefixOnlyIcon) {
        std::string input = "icon ";
        EXPECT_EQ(removeIconPrefix(input), "");
    }

    TEST(StringUtilsTest, RemoveIconPrefixOnlySpace) {
        std::string input = " ";
        EXPECT_EQ(removeIconPrefix(input), "");
    }

    TEST(StringUtilsTest, RemoveIconPrefixSpaceAtStart) {
        std::string input = " text";
        EXPECT_EQ(removeIconPrefix(input), "text");
    }

    TEST(StringUtilsTest, RemoveIconPrefixUnicodeIcon) {
        // Simulate a unicode icon prefix like font awesome
        std::string input = "\xEF\x80\x80 Settings";  // Unicode icon
        EXPECT_EQ(removeIconPrefix(input), "Settings");
    }

    // ==========================================================================
    // trim Tests
    // ==========================================================================

    TEST(StringUtilsTest, TrimRemovesLeadingWhitespace) {
        std::string s = "   text";
        trim(s);
        EXPECT_EQ(s, "text");
    }

    TEST(StringUtilsTest, TrimRemovesTrailingWhitespace) {
        std::string s = "text   ";
        trim(s);
        EXPECT_EQ(s, "text");
    }

    TEST(StringUtilsTest, TrimRemovesBothEnds) {
        std::string s = "   text   ";
        trim(s);
        EXPECT_EQ(s, "text");
    }

    TEST(StringUtilsTest, TrimEmptyString) {
        std::string s = "";
        trim(s);
        EXPECT_EQ(s, "");
    }

    TEST(StringUtilsTest, TrimOnlyWhitespace) {
        std::string s = "     ";
        trim(s);
        EXPECT_EQ(s, "");
    }

    TEST(StringUtilsTest, TrimPreservesMiddleSpaces) {
        std::string s = "  hello world  ";
        trim(s);
        EXPECT_EQ(s, "hello world");
    }

    TEST(StringUtilsTest, TrimNoWhitespace) {
        std::string s = "nowhitespace";
        trim(s);
        EXPECT_EQ(s, "nowhitespace");
    }

    TEST(StringUtilsTest, TrimTabsAndNewlines) {
        std::string s = "\t\n text \t\n";
        trim(s);
        EXPECT_EQ(s, "text");
    }

    TEST(StringUtilsTest, TrimMixedWhitespace) {
        std::string s = " \t \n text \n \t ";
        trim(s);
        EXPECT_EQ(s, "text");
    }

    TEST(StringUtilsTest, TrimSingleCharacter) {
        std::string s = " a ";
        trim(s);
        EXPECT_EQ(s, "a");
    }

}
