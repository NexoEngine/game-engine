//// FilenameValidator.test.cpp ////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for FilenameValidator (filename validation logic)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/FilenameValidator.hpp"

namespace nexo::assets {

class FilenameValidatorTest : public ::testing::Test {};

// =============================================================================
// Valid Name Tests
// =============================================================================

TEST_F(FilenameValidatorTest, ValidSimpleName) {
    auto result = FilenameValidator::validate("myfile");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, ValidNameWithNumbers) {
    auto result = FilenameValidator::validate("file123");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, ValidNameWithDot) {
    auto result = FilenameValidator::validate("file.txt");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, ValidNameWithDash) {
    auto result = FilenameValidator::validate("my-file");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, ValidNameWithUnderscore) {
    auto result = FilenameValidator::validate("my_file");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, ValidNameWithAllAllowedChars) {
    auto result = FilenameValidator::validate("My_File-123.txt");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, ValidSingleChar) {
    auto result = FilenameValidator::validate("a");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, ValidSingleNumber) {
    auto result = FilenameValidator::validate("1");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, ValidNameExactly255Chars) {
    std::string name(255, 'a');
    auto result = FilenameValidator::validate(name);
    EXPECT_FALSE(result.has_value());
}

// =============================================================================
// Empty Name Tests
// =============================================================================

TEST_F(FilenameValidatorTest, EmptyNameReturnsError) {
    auto result = FilenameValidator::validate("");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Cannot be empty.");
}

// =============================================================================
// Max Length Tests
// =============================================================================

TEST_F(FilenameValidatorTest, NameExceeds255CharsReturnsError) {
    std::string name(256, 'a');
    auto result = FilenameValidator::validate(name);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Cannot exceed 255 characters.");
}

TEST_F(FilenameValidatorTest, NameWayTooLongReturnsError) {
    std::string name(1000, 'x');
    auto result = FilenameValidator::validate(name);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Cannot exceed 255 characters.");
}

// =============================================================================
// Invalid Character Tests
// =============================================================================

TEST_F(FilenameValidatorTest, SpaceCharacterReturnsError) {
    auto result = FilenameValidator::validate("my file");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
}

TEST_F(FilenameValidatorTest, SlashCharacterReturnsError) {
    auto result = FilenameValidator::validate("my/file");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
}

TEST_F(FilenameValidatorTest, BackslashCharacterReturnsError) {
    auto result = FilenameValidator::validate("my\\file");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
}

TEST_F(FilenameValidatorTest, ColonCharacterReturnsError) {
    auto result = FilenameValidator::validate("file:name");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
}

TEST_F(FilenameValidatorTest, AsteriskCharacterReturnsError) {
    auto result = FilenameValidator::validate("file*name");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
}

TEST_F(FilenameValidatorTest, QuestionMarkCharacterReturnsError) {
    auto result = FilenameValidator::validate("file?name");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
}

TEST_F(FilenameValidatorTest, QuoteCharacterReturnsError) {
    auto result = FilenameValidator::validate("file\"name");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
}

TEST_F(FilenameValidatorTest, LessThanCharacterReturnsError) {
    auto result = FilenameValidator::validate("file<name");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
}

TEST_F(FilenameValidatorTest, GreaterThanCharacterReturnsError) {
    auto result = FilenameValidator::validate("file>name");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
}

TEST_F(FilenameValidatorTest, PipeCharacterReturnsError) {
    auto result = FilenameValidator::validate("file|name");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
}

TEST_F(FilenameValidatorTest, AtSymbolReturnsError) {
    auto result = FilenameValidator::validate("file@name");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
}

TEST_F(FilenameValidatorTest, HashSymbolReturnsError) {
    auto result = FilenameValidator::validate("file#name");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
}

// =============================================================================
// Reserved Keywords Tests (Windows)
// =============================================================================

TEST_F(FilenameValidatorTest, CONReservedKeywordReturnsError) {
    auto result = FilenameValidator::validate("CON");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Cannot be a reserved keyword.");
}

TEST_F(FilenameValidatorTest, PRNReservedKeywordReturnsError) {
    auto result = FilenameValidator::validate("PRN");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Cannot be a reserved keyword.");
}

TEST_F(FilenameValidatorTest, AUXReservedKeywordReturnsError) {
    auto result = FilenameValidator::validate("AUX");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Cannot be a reserved keyword.");
}

TEST_F(FilenameValidatorTest, NULReservedKeywordReturnsError) {
    auto result = FilenameValidator::validate("NUL");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Cannot be a reserved keyword.");
}

TEST_F(FilenameValidatorTest, COM1ReservedKeywordReturnsError) {
    auto result = FilenameValidator::validate("COM1");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Cannot be a reserved keyword.");
}

TEST_F(FilenameValidatorTest, COM9ReservedKeywordReturnsError) {
    auto result = FilenameValidator::validate("COM9");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Cannot be a reserved keyword.");
}

TEST_F(FilenameValidatorTest, LPT1ReservedKeywordReturnsError) {
    auto result = FilenameValidator::validate("LPT1");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Cannot be a reserved keyword.");
}

TEST_F(FilenameValidatorTest, LPT9ReservedKeywordReturnsError) {
    auto result = FilenameValidator::validate("LPT9");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "Cannot be a reserved keyword.");
}

// Reserved keywords are case-sensitive in the validator
TEST_F(FilenameValidatorTest, LowercaseConIsValid) {
    // The validator checks exact match, so lowercase "con" should be valid
    auto result = FilenameValidator::validate("con");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, ReservedKeywordAsPartOfNameIsValid) {
    // "CON" as part of a longer name should be valid
    auto result = FilenameValidator::validate("CONFIG");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, ReservedKeywordWithExtensionIsValid) {
    // "CON.txt" is not exactly "CON", so should be valid
    auto result = FilenameValidator::validate("CON.txt");
    EXPECT_FALSE(result.has_value());
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST_F(FilenameValidatorTest, OnlyDotsIsValid) {
    auto result = FilenameValidator::validate("...");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, OnlyDashesIsValid) {
    auto result = FilenameValidator::validate("---");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, OnlyUnderscoresIsValid) {
    auto result = FilenameValidator::validate("___");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, MixedCaseLettersAreValid) {
    auto result = FilenameValidator::validate("MyFileName");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FilenameValidatorTest, AllDigitsIsValid) {
    auto result = FilenameValidator::validate("12345");
    EXPECT_FALSE(result.has_value());
}

}  // namespace nexo::assets
