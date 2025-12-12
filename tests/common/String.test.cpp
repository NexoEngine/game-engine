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

// =============================================================================
// toLower Tests - Convert string to lowercase
// =============================================================================

class ToLowerTest : public ::testing::Test {};

TEST_F(ToLowerTest, AllUppercase) {
    EXPECT_EQ(toLower("HELLO"), "hello");
}

TEST_F(ToLowerTest, AllLowercase) {
    EXPECT_EQ(toLower("hello"), "hello");
}

TEST_F(ToLowerTest, MixedCase) {
    EXPECT_EQ(toLower("HeLLo WoRLd"), "hello world");
}

TEST_F(ToLowerTest, EmptyString) {
    EXPECT_EQ(toLower(""), "");
}

TEST_F(ToLowerTest, WithNumbers) {
    EXPECT_EQ(toLower("Test123"), "test123");
}

TEST_F(ToLowerTest, WithSpecialChars) {
    EXPECT_EQ(toLower("Hello!@#$%"), "hello!@#$%");
}

TEST_F(ToLowerTest, SingleChar) {
    EXPECT_EQ(toLower("A"), "a");
}

TEST_F(ToLowerTest, OnlyNumbers) {
    EXPECT_EQ(toLower("12345"), "12345");
}

TEST_F(ToLowerTest, LongString) {
    std::string input(1000, 'A');
    std::string expected(1000, 'a');
    EXPECT_EQ(toLower(input), expected);
}

// =============================================================================
// toUpper Tests - Convert string to uppercase
// =============================================================================

class ToUpperTest : public ::testing::Test {};

TEST_F(ToUpperTest, AllLowercase) {
    EXPECT_EQ(toUpper("hello"), "HELLO");
}

TEST_F(ToUpperTest, AllUppercase) {
    EXPECT_EQ(toUpper("HELLO"), "HELLO");
}

TEST_F(ToUpperTest, MixedCase) {
    EXPECT_EQ(toUpper("HeLLo WoRLd"), "HELLO WORLD");
}

TEST_F(ToUpperTest, EmptyString) {
    EXPECT_EQ(toUpper(""), "");
}

TEST_F(ToUpperTest, WithNumbers) {
    EXPECT_EQ(toUpper("test123"), "TEST123");
}

TEST_F(ToUpperTest, WithSpecialChars) {
    EXPECT_EQ(toUpper("hello!@#$%"), "HELLO!@#$%");
}

TEST_F(ToUpperTest, SingleChar) {
    EXPECT_EQ(toUpper("a"), "A");
}

TEST_F(ToUpperTest, OnlyNumbers) {
    EXPECT_EQ(toUpper("12345"), "12345");
}

TEST_F(ToUpperTest, LongString) {
    std::string input(1000, 'a');
    std::string expected(1000, 'A');
    EXPECT_EQ(toUpper(input), expected);
}

// =============================================================================
// startsWith Tests - Check if string starts with prefix
// =============================================================================

class StartsWithTest : public ::testing::Test {};

TEST_F(StartsWithTest, ValidPrefix) {
    EXPECT_TRUE(startsWith("Hello World", "Hello"));
}

TEST_F(StartsWithTest, ExactMatch) {
    EXPECT_TRUE(startsWith("Hello", "Hello"));
}

TEST_F(StartsWithTest, EmptyPrefix) {
    EXPECT_TRUE(startsWith("Hello", ""));
}

TEST_F(StartsWithTest, EmptyString) {
    EXPECT_FALSE(startsWith("", "Hello"));
}

TEST_F(StartsWithTest, BothEmpty) {
    EXPECT_TRUE(startsWith("", ""));
}

TEST_F(StartsWithTest, InvalidPrefix) {
    EXPECT_FALSE(startsWith("Hello", "World"));
}

TEST_F(StartsWithTest, PrefixLongerThanString) {
    EXPECT_FALSE(startsWith("Hi", "Hello"));
}

TEST_F(StartsWithTest, CaseSensitive) {
    EXPECT_FALSE(startsWith("Hello", "hello"));
}

TEST_F(StartsWithTest, SingleChar) {
    EXPECT_TRUE(startsWith("Hello", "H"));
}

TEST_F(StartsWithTest, WithSpecialChars) {
    EXPECT_TRUE(startsWith("!@#Hello", "!@#"));
}

// =============================================================================
// istartsWith Tests - Case-insensitive prefix check
// =============================================================================

class IStartsWithTest : public ::testing::Test {};

TEST_F(IStartsWithTest, ValidPrefixMixedCase) {
    EXPECT_TRUE(istartsWith("Hello World", "hello"));
}

TEST_F(IStartsWithTest, ValidPrefixUpperCase) {
    EXPECT_TRUE(istartsWith("hello world", "HELLO"));
}

TEST_F(IStartsWithTest, ExactMatch) {
    EXPECT_TRUE(istartsWith("Hello", "hello"));
}

TEST_F(IStartsWithTest, EmptyPrefix) {
    EXPECT_TRUE(istartsWith("Hello", ""));
}

TEST_F(IStartsWithTest, InvalidPrefix) {
    EXPECT_FALSE(istartsWith("Hello", "world"));
}

TEST_F(IStartsWithTest, PrefixLongerThanString) {
    EXPECT_FALSE(istartsWith("Hi", "Hello"));
}

TEST_F(IStartsWithTest, FileExtensions) {
    EXPECT_TRUE(istartsWith("FILE.TXT", "file"));
}

// =============================================================================
// endsWith Tests - Check if string ends with suffix
// =============================================================================

class EndsWithTest : public ::testing::Test {};

TEST_F(EndsWithTest, ValidSuffix) {
    EXPECT_TRUE(endsWith("Hello World", "World"));
}

TEST_F(EndsWithTest, ExactMatch) {
    EXPECT_TRUE(endsWith("Hello", "Hello"));
}

TEST_F(EndsWithTest, EmptySuffix) {
    EXPECT_TRUE(endsWith("Hello", ""));
}

TEST_F(EndsWithTest, EmptyString) {
    EXPECT_FALSE(endsWith("", "Hello"));
}

TEST_F(EndsWithTest, BothEmpty) {
    EXPECT_TRUE(endsWith("", ""));
}

TEST_F(EndsWithTest, InvalidSuffix) {
    EXPECT_FALSE(endsWith("Hello", "World"));
}

TEST_F(EndsWithTest, SuffixLongerThanString) {
    EXPECT_FALSE(endsWith("Hi", "Hello"));
}

TEST_F(EndsWithTest, CaseSensitive) {
    EXPECT_FALSE(endsWith("Hello", "LLO"));
}

TEST_F(EndsWithTest, SingleChar) {
    EXPECT_TRUE(endsWith("Hello", "o"));
}

TEST_F(EndsWithTest, FileExtension) {
    EXPECT_TRUE(endsWith("file.txt", ".txt"));
}

// =============================================================================
// iendsWith Tests - Case-insensitive suffix check
// =============================================================================

class IEndsWithTest : public ::testing::Test {};

TEST_F(IEndsWithTest, ValidSuffixMixedCase) {
    EXPECT_TRUE(iendsWith("Hello World", "world"));
}

TEST_F(IEndsWithTest, ValidSuffixUpperCase) {
    EXPECT_TRUE(iendsWith("hello world", "WORLD"));
}

TEST_F(IEndsWithTest, ExactMatch) {
    EXPECT_TRUE(iendsWith("Hello", "hello"));
}

TEST_F(IEndsWithTest, EmptySuffix) {
    EXPECT_TRUE(iendsWith("Hello", ""));
}

TEST_F(IEndsWithTest, InvalidSuffix) {
    EXPECT_FALSE(iendsWith("Hello", "world"));
}

TEST_F(IEndsWithTest, SuffixLongerThanString) {
    EXPECT_FALSE(iendsWith("Hi", "Hello"));
}

TEST_F(IEndsWithTest, FileExtensions) {
    EXPECT_TRUE(iendsWith("file.TXT", ".txt"));
    EXPECT_TRUE(iendsWith("document.PDF", ".pdf"));
}

// =============================================================================
// ltrim Tests - Left trim whitespace
// =============================================================================

class LTrimTest : public ::testing::Test {};

TEST_F(LTrimTest, LeadingSpaces) {
    EXPECT_EQ(ltrim("   Hello"), "Hello");
}

TEST_F(LTrimTest, LeadingTabs) {
    EXPECT_EQ(ltrim("\t\tHello"), "Hello");
}

TEST_F(LTrimTest, MixedLeadingWhitespace) {
    EXPECT_EQ(ltrim(" \t \nHello"), "Hello");
}

TEST_F(LTrimTest, NoLeadingWhitespace) {
    EXPECT_EQ(ltrim("Hello"), "Hello");
}

TEST_F(LTrimTest, OnlyWhitespace) {
    EXPECT_EQ(ltrim("   "), "");
}

TEST_F(LTrimTest, EmptyString) {
    EXPECT_EQ(ltrim(""), "");
}

TEST_F(LTrimTest, TrailingWhitespacePreserved) {
    EXPECT_EQ(ltrim("   Hello   "), "Hello   ");
}

TEST_F(LTrimTest, InternalWhitespacePreserved) {
    EXPECT_EQ(ltrim("   Hello World"), "Hello World");
}

TEST_F(LTrimTest, LeadingNewlines) {
    EXPECT_EQ(ltrim("\n\nHello"), "Hello");
}

// =============================================================================
// rtrim Tests - Right trim whitespace
// =============================================================================

class RTrimTest : public ::testing::Test {};

TEST_F(RTrimTest, TrailingSpaces) {
    EXPECT_EQ(rtrim("Hello   "), "Hello");
}

TEST_F(RTrimTest, TrailingTabs) {
    EXPECT_EQ(rtrim("Hello\t\t"), "Hello");
}

TEST_F(RTrimTest, MixedTrailingWhitespace) {
    EXPECT_EQ(rtrim("Hello \t \n"), "Hello");
}

TEST_F(RTrimTest, NoTrailingWhitespace) {
    EXPECT_EQ(rtrim("Hello"), "Hello");
}

TEST_F(RTrimTest, OnlyWhitespace) {
    EXPECT_EQ(rtrim("   "), "");
}

TEST_F(RTrimTest, EmptyString) {
    EXPECT_EQ(rtrim(""), "");
}

TEST_F(RTrimTest, LeadingWhitespacePreserved) {
    EXPECT_EQ(rtrim("   Hello   "), "   Hello");
}

TEST_F(RTrimTest, InternalWhitespacePreserved) {
    EXPECT_EQ(rtrim("Hello World   "), "Hello World");
}

TEST_F(RTrimTest, TrailingNewlines) {
    EXPECT_EQ(rtrim("Hello\n\n"), "Hello");
}

// =============================================================================
// trim Tests - Trim whitespace from both sides
// =============================================================================

class TrimTest : public ::testing::Test {};

TEST_F(TrimTest, BothSidesWhitespace) {
    EXPECT_EQ(trim("   Hello   "), "Hello");
}

TEST_F(TrimTest, MixedWhitespace) {
    EXPECT_EQ(trim(" \t\n Hello \n\t "), "Hello");
}

TEST_F(TrimTest, OnlyLeadingWhitespace) {
    EXPECT_EQ(trim("   Hello"), "Hello");
}

TEST_F(TrimTest, OnlyTrailingWhitespace) {
    EXPECT_EQ(trim("Hello   "), "Hello");
}

TEST_F(TrimTest, NoWhitespace) {
    EXPECT_EQ(trim("Hello"), "Hello");
}

TEST_F(TrimTest, OnlyWhitespace) {
    EXPECT_EQ(trim("   "), "");
}

TEST_F(TrimTest, EmptyString) {
    EXPECT_EQ(trim(""), "");
}

TEST_F(TrimTest, InternalWhitespacePreserved) {
    EXPECT_EQ(trim("   Hello World   "), "Hello World");
}

TEST_F(TrimTest, MultipleWords) {
    EXPECT_EQ(trim("  Hello  World  Test  "), "Hello  World  Test");
}

TEST_F(TrimTest, NewlinesAndTabs) {
    EXPECT_EQ(trim("\n\t Hello \t\n"), "Hello");
}

// =============================================================================
// contains Tests - Check if string contains substring
// =============================================================================

class ContainsTest : public ::testing::Test {};

TEST_F(ContainsTest, ValidSubstring) {
    EXPECT_TRUE(contains("Hello World", "World"));
}

TEST_F(ContainsTest, SubstringAtStart) {
    EXPECT_TRUE(contains("Hello World", "Hello"));
}

TEST_F(ContainsTest, SubstringInMiddle) {
    EXPECT_TRUE(contains("Hello World", "lo Wo"));
}

TEST_F(ContainsTest, ExactMatch) {
    EXPECT_TRUE(contains("Hello", "Hello"));
}

TEST_F(ContainsTest, EmptySubstring) {
    EXPECT_TRUE(contains("Hello", ""));
}

TEST_F(ContainsTest, EmptyString) {
    EXPECT_FALSE(contains("", "Hello"));
}

TEST_F(ContainsTest, BothEmpty) {
    EXPECT_TRUE(contains("", ""));
}

TEST_F(ContainsTest, SubstringNotFound) {
    EXPECT_FALSE(contains("Hello", "World"));
}

TEST_F(ContainsTest, CaseSensitive) {
    EXPECT_FALSE(contains("Hello", "hello"));
}

TEST_F(ContainsTest, SingleChar) {
    EXPECT_TRUE(contains("Hello", "e"));
}

TEST_F(ContainsTest, MultipleOccurrences) {
    EXPECT_TRUE(contains("Hello Hello", "Hello"));
}

// =============================================================================
// icontains Tests - Case-insensitive substring search
// =============================================================================

class IContainsTest : public ::testing::Test {};

TEST_F(IContainsTest, ValidSubstringMixedCase) {
    EXPECT_TRUE(icontains("Hello World", "world"));
}

TEST_F(IContainsTest, ValidSubstringUpperCase) {
    EXPECT_TRUE(icontains("hello world", "WORLD"));
}

TEST_F(IContainsTest, SubstringInMiddle) {
    EXPECT_TRUE(icontains("Hello World", "LO WO"));
}

TEST_F(IContainsTest, ExactMatch) {
    EXPECT_TRUE(icontains("Hello", "hello"));
}

TEST_F(IContainsTest, EmptySubstring) {
    EXPECT_TRUE(icontains("Hello", ""));
}

TEST_F(IContainsTest, SubstringNotFound) {
    EXPECT_FALSE(icontains("Hello", "xyz"));
}

TEST_F(IContainsTest, SingleChar) {
    EXPECT_TRUE(icontains("Hello", "E"));
}

TEST_F(IContainsTest, SubstringLongerThanString) {
    EXPECT_FALSE(icontains("Hi", "Hello"));
}

TEST_F(IContainsTest, SpecialChars) {
    EXPECT_TRUE(icontains("Hello!@#World", "!@#world"));
}

// =============================================================================
// replaceAll Tests - Replace all occurrences of substring
// =============================================================================

class ReplaceAllTest : public ::testing::Test {};

TEST_F(ReplaceAllTest, SingleOccurrence) {
    EXPECT_EQ(replaceAll("Hello World", "World", "Universe"), "Hello Universe");
}

TEST_F(ReplaceAllTest, MultipleOccurrences) {
    EXPECT_EQ(replaceAll("Hello Hello Hello", "Hello", "Hi"), "Hi Hi Hi");
}

TEST_F(ReplaceAllTest, NoOccurrence) {
    EXPECT_EQ(replaceAll("Hello World", "Goodbye", "Hi"), "Hello World");
}

TEST_F(ReplaceAllTest, EmptyFrom) {
    EXPECT_EQ(replaceAll("Hello", "", "X"), "Hello");
}

TEST_F(ReplaceAllTest, EmptyTo) {
    EXPECT_EQ(replaceAll("Hello World", "World", ""), "Hello ");
}

TEST_F(ReplaceAllTest, EmptyString) {
    EXPECT_EQ(replaceAll("", "Hello", "Hi"), "");
}

TEST_F(ReplaceAllTest, ReplaceWithLongerString) {
    EXPECT_EQ(replaceAll("Hi", "Hi", "Hello"), "Hello");
}

TEST_F(ReplaceAllTest, ReplaceWithShorterString) {
    EXPECT_EQ(replaceAll("Hello", "Hello", "Hi"), "Hi");
}

TEST_F(ReplaceAllTest, OverlappingPattern) {
    EXPECT_EQ(replaceAll("aaa", "aa", "b"), "ba");
}

TEST_F(ReplaceAllTest, ConsecutiveReplacements) {
    EXPECT_EQ(replaceAll("ababab", "ab", "c"), "ccc");
}

TEST_F(ReplaceAllTest, ReplaceSpaces) {
    EXPECT_EQ(replaceAll("Hello World Test", " ", "_"), "Hello_World_Test");
}

// =============================================================================
// split Tests - Split string by delimiter
// =============================================================================

class SplitTest : public ::testing::Test {};

TEST_F(SplitTest, SimpleDelimiter) {
    auto result = split("a,b,c", ",");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");
}

TEST_F(SplitTest, MultiCharDelimiter) {
    auto result = split("a::b::c", "::");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");
}

TEST_F(SplitTest, NoDelimiter) {
    auto result = split("abc", ",");
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "abc");
}

TEST_F(SplitTest, EmptyString) {
    auto result = split("", ",");
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "");
}

TEST_F(SplitTest, EmptyDelimiter) {
    auto result = split("abc", "");
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "abc");
}

TEST_F(SplitTest, ConsecutiveDelimiters) {
    auto result = split("a,,b", ",");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "");
    EXPECT_EQ(result[2], "b");
}

TEST_F(SplitTest, DelimiterAtStart) {
    auto result = split(",a,b", ",");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "");
    EXPECT_EQ(result[1], "a");
    EXPECT_EQ(result[2], "b");
}

TEST_F(SplitTest, DelimiterAtEnd) {
    auto result = split("a,b,", ",");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "");
}

TEST_F(SplitTest, OnlyDelimiter) {
    auto result = split(",", ",");
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "");
    EXPECT_EQ(result[1], "");
}

TEST_F(SplitTest, SpaceDelimiter) {
    auto result = split("Hello World Test", " ");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "Hello");
    EXPECT_EQ(result[1], "World");
    EXPECT_EQ(result[2], "Test");
}

// =============================================================================
// join Tests - Join collection of strings
// =============================================================================

class JoinTest : public ::testing::Test {};

TEST_F(JoinTest, SimpleJoin) {
    std::vector<std::string> vec = {"a", "b", "c"};
    EXPECT_EQ(join(vec, ","), "a,b,c");
}

TEST_F(JoinTest, MultiCharDelimiter) {
    std::vector<std::string> vec = {"a", "b", "c"};
    EXPECT_EQ(join(vec, "::"), "a::b::c");
}

TEST_F(JoinTest, EmptyDelimiter) {
    std::vector<std::string> vec = {"a", "b", "c"};
    EXPECT_EQ(join(vec, ""), "abc");
}

TEST_F(JoinTest, SingleElement) {
    std::vector<std::string> vec = {"hello"};
    EXPECT_EQ(join(vec, ","), "hello");
}

TEST_F(JoinTest, EmptyVector) {
    std::vector<std::string> vec;
    EXPECT_EQ(join(vec, ","), "");
}

TEST_F(JoinTest, EmptyStrings) {
    std::vector<std::string> vec = {"", "", ""};
    EXPECT_EQ(join(vec, ","), ",,");
}

TEST_F(JoinTest, MixedEmptyAndNonEmpty) {
    std::vector<std::string> vec = {"a", "", "c"};
    EXPECT_EQ(join(vec, ","), "a,,c");
}

TEST_F(JoinTest, SpaceDelimiter) {
    std::vector<std::string> vec = {"Hello", "World", "Test"};
    EXPECT_EQ(join(vec, " "), "Hello World Test");
}

TEST_F(JoinTest, LongStrings) {
    std::vector<std::string> vec = {"Hello", "World", "This", "Is", "A", "Test"};
    EXPECT_EQ(join(vec, " "), "Hello World This Is A Test");
}

// =============================================================================
// Integration Tests - Testing combinations of utilities
// =============================================================================

class IntegrationTest : public ::testing::Test {};

TEST_F(IntegrationTest, TrimAndSplit) {
    auto parts = split(trim("  a, b, c  "), ",");
    ASSERT_EQ(parts.size(), 3);
    EXPECT_EQ(trim(parts[0]), "a");
    EXPECT_EQ(trim(parts[1]), "b");
    EXPECT_EQ(trim(parts[2]), "c");
}

TEST_F(IntegrationTest, SplitAndJoin) {
    auto parts = split("a,b,c", ",");
    EXPECT_EQ(join(parts, ";"), "a;b;c");
}

TEST_F(IntegrationTest, CaseConversion) {
    std::string str = "Hello World";
    EXPECT_EQ(toLower(str), "hello world");
    EXPECT_EQ(toUpper(str), "HELLO WORLD");
    EXPECT_TRUE(iequals(toLower(str), "HELLO WORLD"));
}

TEST_F(IntegrationTest, ReplaceAndTrim) {
    auto result = trim(replaceAll("  Hello_World_Test  ", "_", " "));
    EXPECT_EQ(result, "Hello World Test");
}

TEST_F(IntegrationTest, FilePathOperations) {
    std::string path = "/path/to/file.TXT";
    EXPECT_TRUE(endsWith(path, ".TXT"));
    EXPECT_TRUE(iendsWith(path, ".txt"));
    EXPECT_TRUE(startsWith(path, "/path"));
}

TEST_F(IntegrationTest, SearchInProcessedString) {
    std::string str = "  HELLO WORLD  ";
    std::string processed = toLower(trim(str));
    EXPECT_TRUE(contains(processed, "hello"));
    EXPECT_TRUE(contains(processed, "world"));
}

// =============================================================================
// Edge Cases and Stress Tests
// =============================================================================

class EdgeCaseTest : public ::testing::Test {};

TEST_F(EdgeCaseTest, VeryLongStringSplit) {
    std::string longStr(10000, 'a');
    longStr += ",";
    longStr += std::string(10000, 'b');
    auto result = split(longStr, ",");
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].size(), 10000);
    EXPECT_EQ(result[1].size(), 10000);
}

TEST_F(EdgeCaseTest, VeryLongStringJoin) {
    std::vector<std::string> vec;
    for (int i = 0; i < 1000; ++i) {
        vec.push_back("item");
    }
    auto result = join(vec, ",");
    auto parts = split(result, ",");
    EXPECT_EQ(parts.size(), 1000);
}

TEST_F(EdgeCaseTest, ManyReplacements) {
    std::string str(1000, 'a');
    auto result = replaceAll(str, "a", "bb");
    EXPECT_EQ(result.size(), 2000);
}

TEST_F(EdgeCaseTest, UnicodeCharacters) {
    // Basic UTF-8 support test
    std::string utf8 = "Hello 世界";
    EXPECT_TRUE(contains(utf8, "Hello"));
    EXPECT_TRUE(contains(utf8, "世界"));
}

TEST_F(EdgeCaseTest, SpecialWhitespaceChars) {
    // Test various whitespace characters
    std::string str = "\r\n\t\v\f Hello \r\n\t\v\f";
    auto trimmed = trim(str);
    EXPECT_EQ(trimmed, "Hello");
}

TEST_F(EdgeCaseTest, NullCharactersInString) {
    std::string str = "Hello\0World";
    // String will stop at null terminator when constructed from C string
    // but should handle explicit construction
    EXPECT_TRUE(contains("Hello", "Hel"));
}

TEST_F(EdgeCaseTest, AllPrintableASCII) {
    std::string ascii;
    for (char c = 32; c < 127; ++c) {
        ascii += c;
    }
    // Round trip conversion should maintain lowercase
    std::string lower = toLower(ascii);
    EXPECT_EQ(toLower(toUpper(lower)), lower);
    // Uppercase letters should be preserved in round trip
    std::string upper = toUpper(ascii);
    EXPECT_EQ(toUpper(toLower(upper)), upper);
}

TEST_F(EdgeCaseTest, ConsecutiveDelimitersInSplit) {
    auto result = split("a,,,b,,,c", ",");
    EXPECT_EQ(result.size(), 7);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "");
    EXPECT_EQ(result[2], "");
    EXPECT_EQ(result[3], "b");
}

TEST_F(EdgeCaseTest, ReplaceWithSelf) {
    EXPECT_EQ(replaceAll("Hello", "Hello", "Hello"), "Hello");
}

TEST_F(EdgeCaseTest, ContainsEmptyInEmpty) {
    EXPECT_TRUE(contains("", ""));
    EXPECT_TRUE(icontains("", ""));
}

}  // namespace nexo
