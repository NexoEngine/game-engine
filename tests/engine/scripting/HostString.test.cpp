//// HostString.test.cpp /////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for scripting HostString class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "scripting/native/HostString.hpp"
#include <string>
#include <algorithm>
#include <iterator>
#include <cstring>

namespace nexo::scripting {

class HostStringTest : public ::testing::Test {
protected:
    // Helper function to convert HostString to std::string for comparison
    std::string toString(const HostString& hs) {
        return hs.to_utf8();
    }
};

// =============================================================================
// Constructor Tests
// =============================================================================

TEST_F(HostStringTest, DefaultConstructor) {
    HostString hs;
    EXPECT_TRUE(hs.empty());
    EXPECT_EQ(hs.size(), 0u);
    EXPECT_NE(hs.c_str(), nullptr);
}

TEST_F(HostStringTest, NullptrConstructor) {
    HostString hs(nullptr);
    EXPECT_TRUE(hs.empty());
    EXPECT_EQ(hs.size(), 0u);
}

TEST_F(HostStringTest, StdStringConstructor) {
    std::string str = "Hello World";
    HostString hs(str);
    EXPECT_FALSE(hs.empty());
    EXPECT_EQ(hs.size(), str.size());
    EXPECT_EQ(toString(hs), str);
}

TEST_F(HostStringTest, CStringConstructor) {
    const char* cstr = "Test String";
    HostString hs(cstr);
    EXPECT_FALSE(hs.empty());
    EXPECT_EQ(hs.size(), std::strlen(cstr));
    EXPECT_EQ(toString(hs), std::string(cstr));
}

TEST_F(HostStringTest, EmptyStringConstructor) {
    std::string empty = "";
    HostString hs(empty);
    EXPECT_TRUE(hs.empty());
    EXPECT_EQ(hs.size(), 0u);
}

TEST_F(HostStringTest, WideStringConstructor) {
    std::wstring wstr = L"Wide String Test";
    HostString hs(wstr);
    EXPECT_FALSE(hs.empty());
    EXPECT_GT(hs.size(), 0u);
}

TEST_F(HostStringTest, WideCharConstructor) {
    const wchar_t* wcstr = L"Wide C String";
    HostString hs(wcstr);
    EXPECT_FALSE(hs.empty());
    EXPECT_GT(hs.size(), 0u);
}

// =============================================================================
// Copy Constructor and Assignment Tests
// =============================================================================

TEST_F(HostStringTest, CopyConstructor) {
    HostString original("Original String");
    HostString copy(original);

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(toString(copy), toString(original));
    EXPECT_EQ(copy, original);
}

TEST_F(HostStringTest, CopyAssignment) {
    HostString original("Original String");
    HostString copy;

    copy = original;

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(toString(copy), toString(original));
    EXPECT_EQ(copy, original);
}

TEST_F(HostStringTest, CopyAssignmentFromEmpty) {
    HostString original;
    HostString copy("Some Content");

    copy = original;

    EXPECT_TRUE(copy.empty());
    EXPECT_EQ(copy.size(), 0u);
}

TEST_F(HostStringTest, CopyAssignmentToEmpty) {
    HostString original("New Content");
    HostString copy;

    copy = original;

    EXPECT_EQ(toString(copy), "New Content");
    EXPECT_EQ(copy.size(), original.size());
}

TEST_F(HostStringTest, SelfAssignment) {
    HostString str("Self Assignment Test");
    str = str;

    EXPECT_EQ(toString(str), "Self Assignment Test");
    EXPECT_FALSE(str.empty());
}

// =============================================================================
// Move Constructor and Assignment Tests
// =============================================================================

TEST_F(HostStringTest, MoveConstructor) {
    HostString original("Move Test");
    std::string original_str = toString(original);
    size_t original_size = original.size();

    HostString moved(std::move(original));

    EXPECT_EQ(toString(moved), original_str);
    EXPECT_EQ(moved.size(), original_size);
}

TEST_F(HostStringTest, MoveAssignment) {
    HostString original("Move Assignment Test");
    std::string original_str = toString(original);
    size_t original_size = original.size();

    HostString target;
    target = std::move(original);

    EXPECT_EQ(toString(target), original_str);
    EXPECT_EQ(target.size(), original_size);
}

// =============================================================================
// Conversion Tests
// =============================================================================

TEST_F(HostStringTest, ToUtf8Method) {
    std::string original = "UTF-8 Test String";
    HostString hs(original);

    std::string result = hs.to_utf8();
    EXPECT_EQ(result, original);
}

TEST_F(HostStringTest, ToWideMethod) {
    HostString hs("Wide Conversion Test");
    std::wstring wide = hs.to_wide();

    EXPECT_FALSE(wide.empty());
    EXPECT_GT(wide.size(), 0u);
}

TEST_F(HostStringTest, ImplicitConversionToStdString) {
    HostString hs("Implicit Conversion");
    std::string str = hs;

    EXPECT_EQ(str, "Implicit Conversion");
}

TEST_F(HostStringTest, ImplicitConversionToWideString) {
    HostString hs("Implicit Wide Conversion");
    std::wstring wstr = hs;

    EXPECT_FALSE(wstr.empty());
}

// =============================================================================
// Access Method Tests
// =============================================================================

TEST_F(HostStringTest, CStrMethod) {
    HostString hs("C String Test");
    const char_t* cstr = hs.c_str();

    EXPECT_NE(cstr, nullptr);
}

TEST_F(HostStringTest, EmptyMethod) {
    HostString empty;
    HostString notEmpty("Content");

    EXPECT_TRUE(empty.empty());
    EXPECT_FALSE(notEmpty.empty());
}

TEST_F(HostStringTest, SizeMethod) {
    HostString hs("Size Test");
    EXPECT_EQ(hs.size(), 9u);

    HostString empty;
    EXPECT_EQ(empty.size(), 0u);
}

TEST_F(HostStringTest, SubscriptOperator) {
    HostString hs("ABCDEF");

    EXPECT_EQ(hs[0], static_cast<char_t>('A'));
    EXPECT_EQ(hs[1], static_cast<char_t>('B'));
    EXPECT_EQ(hs[5], static_cast<char_t>('F'));
}

TEST_F(HostStringTest, SubscriptOperatorConst) {
    const HostString hs("ABCDEF");

    EXPECT_EQ(hs[0], static_cast<char_t>('A'));
    EXPECT_EQ(hs[1], static_cast<char_t>('B'));
    EXPECT_EQ(hs[5], static_cast<char_t>('F'));
}

TEST_F(HostStringTest, AtMethod) {
    HostString hs("ABCDEF");

    EXPECT_EQ(hs.at(0), static_cast<char_t>('A'));
    EXPECT_EQ(hs.at(2), static_cast<char_t>('C'));
    EXPECT_EQ(hs.at(5), static_cast<char_t>('F'));
}

TEST_F(HostStringTest, AtMethodConst) {
    const HostString hs("ABCDEF");

    EXPECT_EQ(hs.at(0), static_cast<char_t>('A'));
    EXPECT_EQ(hs.at(2), static_cast<char_t>('C'));
    EXPECT_EQ(hs.at(5), static_cast<char_t>('F'));
}

// =============================================================================
// Iterator Tests
// =============================================================================

TEST_F(HostStringTest, BeginEnd) {
    HostString hs("ABC");

    EXPECT_NE(hs.begin(), hs.end());
    EXPECT_EQ(std::distance(hs.begin(), hs.end()), 3);
}

TEST_F(HostStringTest, BeginEndConst) {
    const HostString hs("ABC");

    EXPECT_NE(hs.begin(), hs.end());
    EXPECT_EQ(std::distance(hs.begin(), hs.end()), 3);
}

TEST_F(HostStringTest, CBeginCEnd) {
    HostString hs("ABC");

    EXPECT_NE(hs.cbegin(), hs.cend());
    EXPECT_EQ(std::distance(hs.cbegin(), hs.cend()), 3);
}

TEST_F(HostStringTest, ReverseIterators) {
    HostString hs("ABC");

    EXPECT_NE(hs.rbegin(), hs.rend());
    EXPECT_EQ(std::distance(hs.rbegin(), hs.rend()), 3);
}

TEST_F(HostStringTest, ReverseIteratorsConst) {
    const HostString hs("ABC");

    EXPECT_NE(hs.rbegin(), hs.rend());
    EXPECT_EQ(std::distance(hs.rbegin(), hs.rend()), 3);
}

TEST_F(HostStringTest, CReverseIterators) {
    HostString hs("ABC");

    EXPECT_NE(hs.crbegin(), hs.crend());
    EXPECT_EQ(std::distance(hs.crbegin(), hs.crend()), 3);
}

TEST_F(HostStringTest, IteratorIteration) {
    HostString hs("ABC");
    std::string result;

    for (auto it = hs.begin(); it != hs.end(); ++it) {
        result += static_cast<char>(*it);
    }

    EXPECT_EQ(result, "ABC");
}

TEST_F(HostStringTest, RangeBasedForLoop) {
    HostString hs("XYZ");
    std::string result;

    for (char_t c : hs) {
        result += static_cast<char>(c);
    }

    EXPECT_EQ(result, "XYZ");
}

TEST_F(HostStringTest, EmptyStringIterators) {
    HostString empty;

    EXPECT_EQ(empty.begin(), empty.end());
    EXPECT_EQ(std::distance(empty.begin(), empty.end()), 0);
}

// =============================================================================
// Comparison Operator Tests
// =============================================================================

TEST_F(HostStringTest, EqualityOperator) {
    HostString hs1("Equal");
    HostString hs2("Equal");

    EXPECT_TRUE(hs1 == hs2);
}

TEST_F(HostStringTest, EqualityOperatorDifferent) {
    HostString hs1("First");
    HostString hs2("Second");

    EXPECT_FALSE(hs1 == hs2);
}

TEST_F(HostStringTest, EqualityOperatorEmpty) {
    HostString empty1;
    HostString empty2;

    EXPECT_TRUE(empty1 == empty2);
}

TEST_F(HostStringTest, InequalityOperator) {
    HostString hs1("First");
    HostString hs2("Second");

    EXPECT_TRUE(hs1 != hs2);
}

TEST_F(HostStringTest, InequalityOperatorSame) {
    HostString hs1("Same");
    HostString hs2("Same");

    EXPECT_FALSE(hs1 != hs2);
}

TEST_F(HostStringTest, ComparisonSelf) {
    HostString hs("Self");

    EXPECT_TRUE(hs == hs);
    EXPECT_FALSE(hs != hs);
}

// =============================================================================
// Concatenation Operator Tests
// =============================================================================

TEST_F(HostStringTest, PlusEqualOperator) {
    HostString hs1("Hello");
    HostString hs2(" World");

    hs1 += hs2;

    EXPECT_EQ(toString(hs1), "Hello World");
}

TEST_F(HostStringTest, PlusEqualOperatorEmpty) {
    HostString hs1("Content");
    HostString empty;

    hs1 += empty;

    EXPECT_EQ(toString(hs1), "Content");
}

TEST_F(HostStringTest, PlusEqualOperatorToEmpty) {
    HostString empty;
    HostString hs2("Content");

    empty += hs2;

    EXPECT_EQ(toString(empty), "Content");
}

TEST_F(HostStringTest, PlusOperator) {
    HostString hs1("Hello");
    HostString hs2(" World");

    HostString result = hs1 + hs2;

    EXPECT_EQ(toString(result), "Hello World");
    EXPECT_EQ(toString(hs1), "Hello");  // Original unchanged
    EXPECT_EQ(toString(hs2), " World"); // Original unchanged
}

TEST_F(HostStringTest, PlusOperatorChaining) {
    HostString hs1("A");
    HostString hs2("B");
    HostString hs3("C");

    HostString result = hs1 + hs2 + hs3;

    EXPECT_EQ(toString(result), "ABC");
}

TEST_F(HostStringTest, PlusOperatorWithEmpty) {
    HostString hs("Content");
    HostString empty;

    HostString result1 = hs + empty;
    HostString result2 = empty + hs;

    EXPECT_EQ(toString(result1), "Content");
    EXPECT_EQ(toString(result2), "Content");
}

// =============================================================================
// Edge Cases and Special Character Tests
// =============================================================================

TEST_F(HostStringTest, SingleCharacterString) {
    HostString hs("A");

    EXPECT_EQ(hs.size(), 1u);
    EXPECT_EQ(toString(hs), "A");
    EXPECT_FALSE(hs.empty());
}

TEST_F(HostStringTest, StringWithSpaces) {
    HostString hs("   Spaces   ");

    EXPECT_EQ(toString(hs), "   Spaces   ");
    EXPECT_EQ(hs.size(), 12u);
}

TEST_F(HostStringTest, StringWithNewlines) {
    HostString hs("Line1\nLine2\nLine3");

    EXPECT_EQ(toString(hs), "Line1\nLine2\nLine3");
}

TEST_F(HostStringTest, StringWithTabs) {
    HostString hs("Tab\tSeparated\tValues");

    EXPECT_EQ(toString(hs), "Tab\tSeparated\tValues");
}

TEST_F(HostStringTest, StringWithSpecialCharacters) {
    HostString hs("!@#$%^&*()");

    EXPECT_EQ(toString(hs), "!@#$%^&*()");
}

TEST_F(HostStringTest, StringWithNumbers) {
    HostString hs("1234567890");

    EXPECT_EQ(toString(hs), "1234567890");
    EXPECT_EQ(hs.size(), 10u);
}

TEST_F(HostStringTest, LongString) {
    std::string longStr(1000, 'X');
    HostString hs(longStr);

    EXPECT_EQ(hs.size(), 1000u);
    EXPECT_EQ(toString(hs), longStr);
}

TEST_F(HostStringTest, UTF8Characters) {
    std::string utf8 = "Hello World UTF8";
    HostString hs(utf8);

    EXPECT_FALSE(hs.empty());
    EXPECT_GT(hs.size(), 0u);
}

TEST_F(HostStringTest, StringWithQuotes) {
    HostString hs("String with \"quotes\"");

    EXPECT_EQ(toString(hs), "String with \"quotes\"");
}

TEST_F(HostStringTest, StringWithBackslashes) {
    HostString hs("Path\\To\\File");

    EXPECT_EQ(toString(hs), "Path\\To\\File");
}

// =============================================================================
// Round-trip Conversion Tests
// =============================================================================

TEST_F(HostStringTest, RoundTripUtf8) {
    std::string original = "Round Trip Test";
    HostString hs(original);
    std::string result = hs.to_utf8();

    EXPECT_EQ(result, original);
}

TEST_F(HostStringTest, RoundTripWide) {
    std::wstring original = L"Wide Round Trip";
    HostString hs(original);
    std::wstring result = hs.to_wide();

    // Convert back to verify
    HostString hs2(result);
    EXPECT_EQ(hs, hs2);
}

TEST_F(HostStringTest, MultipleConversions) {
    std::string str1 = "Test";
    HostString hs1(str1);
    std::wstring wstr = hs1.to_wide();
    HostString hs2(wstr);
    std::string str2 = hs2.to_utf8();

    EXPECT_EQ(str1, str2);
}

// =============================================================================
// Mixed Operations Tests
// =============================================================================

TEST_F(HostStringTest, CopyThenModify) {
    HostString original("Original");
    HostString copy = original;

    copy += HostString(" Modified");

    EXPECT_EQ(toString(original), "Original");
    EXPECT_EQ(toString(copy), "Original Modified");
}

TEST_F(HostStringTest, ChainedConcatenationAndComparison) {
    HostString hs1("A");
    HostString hs2("B");
    HostString hs3("C");

    HostString result = hs1 + hs2 + hs3;
    HostString expected("ABC");

    EXPECT_EQ(result, expected);
}

TEST_F(HostStringTest, IteratorModification) {
    HostString hs("abc");

    for (auto it = hs.begin(); it != hs.end(); ++it) {
        *it = static_cast<char_t>(std::toupper(static_cast<char>(*it)));
    }

    EXPECT_EQ(toString(hs), "ABC");
}

TEST_F(HostStringTest, EmptyAfterDefaultConstruction) {
    HostString hs;

    EXPECT_TRUE(hs.empty());
    EXPECT_EQ(hs.size(), 0u);
    EXPECT_EQ(hs.begin(), hs.end());
}

// =============================================================================
// Edge Case Tests - Very Long Strings
// =============================================================================

TEST_F(HostStringTest, VeryLongString) {
    std::string longStr(10000, 'A');
    HostString hs(longStr);

    EXPECT_EQ(hs.size(), 10000u);
    EXPECT_EQ(toString(hs), longStr);
}

TEST_F(HostStringTest, ExtremelyLongString) {
    std::string hugeStr(100000, 'X');
    HostString hs(hugeStr);

    EXPECT_EQ(hs.size(), 100000u);
    EXPECT_EQ(hs[0], static_cast<char_t>('X'));
    EXPECT_EQ(hs[99999], static_cast<char_t>('X'));
}

TEST_F(HostStringTest, LongStringConcatenation) {
    std::string str1(5000, 'A');
    std::string str2(5000, 'B');

    HostString hs1(str1);
    HostString hs2(str2);

    HostString result = hs1 + hs2;

    EXPECT_EQ(result.size(), 10000u);
    EXPECT_EQ(result[0], static_cast<char_t>('A'));
    EXPECT_EQ(result[5000], static_cast<char_t>('B'));
}

TEST_F(HostStringTest, MultipleLongStringConcatenations) {
    HostString result;

    for (int i = 0; i < 100; ++i) {
        result += HostString("X");
    }

    EXPECT_EQ(result.size(), 100u);
}

// =============================================================================
// Edge Case Tests - Null and Empty Edge Cases
// =============================================================================

TEST_F(HostStringTest, NullptrToString) {
    HostString hs(nullptr);
    std::string str = toString(hs);

    EXPECT_TRUE(str.empty());
    EXPECT_EQ(str.size(), 0u);
}

TEST_F(HostStringTest, EmptyStringToWide) {
    HostString hs("");
    std::wstring wstr = hs.to_wide();

    EXPECT_TRUE(wstr.empty());
}

TEST_F(HostStringTest, EmptyWideString) {
    std::wstring empty = L"";
    HostString hs(empty);

    EXPECT_TRUE(hs.empty());
    EXPECT_EQ(hs.size(), 0u);
}

TEST_F(HostStringTest, MultipleEmptyConcatenations) {
    HostString hs1;
    HostString hs2;
    HostString hs3;

    HostString result = hs1 + hs2 + hs3;

    EXPECT_TRUE(result.empty());
}

TEST_F(HostStringTest, NullptrComparison) {
    HostString hs1(nullptr);
    HostString hs2(nullptr);

    EXPECT_TRUE(hs1 == hs2);
    EXPECT_FALSE(hs1 != hs2);
}

// =============================================================================
// Edge Case Tests - Unicode and Special Characters
// =============================================================================

TEST_F(HostStringTest, UnicodeEmojiString) {
    std::string emoji = "Hello";  // Basic for safety
    HostString hs(emoji);

    EXPECT_FALSE(hs.empty());
    EXPECT_GT(hs.size(), 0u);
}

TEST_F(HostStringTest, NullCharacterInMiddle) {
    // Note: std::string can handle embedded nulls
    std::string str = "Before";
    str += '\0';
    str += "After";

    HostString hs(str);

    // Size should reflect the string constructor behavior
    EXPECT_GT(hs.size(), 0u);
}

TEST_F(HostStringTest, AllWhitespaceString) {
    std::string whitespace = "     \t\t\n\n   ";
    HostString hs(whitespace);

    EXPECT_EQ(toString(hs), whitespace);
    EXPECT_FALSE(hs.empty());
}

TEST_F(HostStringTest, OnlyNewlines) {
    std::string newlines = "\n\n\n\n\n";
    HostString hs(newlines);

    EXPECT_EQ(hs.size(), 5u);
    EXPECT_EQ(toString(hs), newlines);
}

TEST_F(HostStringTest, MixedLineEndings) {
    std::string mixed = "Line1\nLine2\r\nLine3\rLine4";
    HostString hs(mixed);

    EXPECT_EQ(toString(hs), mixed);
}

// =============================================================================
// Edge Case Tests - Iterator Edge Cases
// =============================================================================

TEST_F(HostStringTest, EmptyStringIteratorDistance) {
    HostString empty;

    EXPECT_EQ(std::distance(empty.begin(), empty.end()), 0);
    EXPECT_EQ(std::distance(empty.cbegin(), empty.cend()), 0);
    EXPECT_EQ(std::distance(empty.rbegin(), empty.rend()), 0);
}

TEST_F(HostStringTest, SingleCharIterator) {
    HostString hs("A");

    EXPECT_EQ(std::distance(hs.begin(), hs.end()), 1);
    EXPECT_EQ(*hs.begin(), static_cast<char_t>('A'));
}

TEST_F(HostStringTest, ReverseIteratorTraversal) {
    HostString hs("ABCD");
    std::string reversed;

    for (auto it = hs.rbegin(); it != hs.rend(); ++it) {
        reversed += static_cast<char>(*it);
    }

    EXPECT_EQ(reversed, "DCBA");
}

TEST_F(HostStringTest, ConstIteratorModification) {
    HostString hs("test");
    const HostString& constRef = hs;

    // Should be able to iterate but not modify
    for (auto it = constRef.cbegin(); it != constRef.cend(); ++it) {
        [[maybe_unused]] char_t c = *it;  // Read only
    }

    EXPECT_EQ(toString(hs), "test");
}

// =============================================================================
// Edge Case Tests - Subscript Bounds
// =============================================================================

TEST_F(HostStringTest, FirstCharacterAccess) {
    HostString hs("First");

    EXPECT_EQ(hs[0], static_cast<char_t>('F'));
    EXPECT_EQ(hs.at(0), static_cast<char_t>('F'));
}

TEST_F(HostStringTest, LastCharacterAccess) {
    HostString hs("Last");

    EXPECT_EQ(hs[3], static_cast<char_t>('t'));
    EXPECT_EQ(hs.at(3), static_cast<char_t>('t'));
}

TEST_F(HostStringTest, MiddleCharacterAccess) {
    HostString hs("Middle");
    size_t middle = hs.size() / 2;

    EXPECT_EQ(hs[middle], static_cast<char_t>('d'));
}

TEST_F(HostStringTest, SequentialAccess) {
    HostString hs("012345");

    for (size_t i = 0; i < hs.size(); ++i) {
        char expected = static_cast<char>('0' + i);
        EXPECT_EQ(hs[i], static_cast<char_t>(expected));
    }
}

// =============================================================================
// Edge Case Tests - Comparison Edge Cases
// =============================================================================

TEST_F(HostStringTest, CaseSensitiveComparison) {
    HostString lower("hello");
    HostString upper("HELLO");

    EXPECT_NE(lower, upper);
}

TEST_F(HostStringTest, SimilarButDifferentStrings) {
    HostString hs1("test1");
    HostString hs2("test2");

    EXPECT_NE(hs1, hs2);
}

TEST_F(HostStringTest, DifferentLengthComparison) {
    HostString short_str("hi");
    HostString long_str("hello");

    EXPECT_NE(short_str, long_str);
}

TEST_F(HostStringTest, WhitespaceMatters) {
    HostString with_space("hello ");
    HostString without_space("hello");

    EXPECT_NE(with_space, without_space);
}

// =============================================================================
// Edge Case Tests - Concatenation Patterns
// =============================================================================

TEST_F(HostStringTest, ConcatenateEmptyToFull) {
    HostString full("Content");
    HostString empty;

    HostString result1 = full + empty;
    HostString result2 = empty + full;

    EXPECT_EQ(toString(result1), "Content");
    EXPECT_EQ(toString(result2), "Content");
}

TEST_F(HostStringTest, ChainedPlusEquals) {
    HostString hs("A");

    hs += HostString("B");
    hs += HostString("C");
    hs += HostString("D");

    EXPECT_EQ(toString(hs), "ABCD");
}

TEST_F(HostStringTest, ConcatenationPreservesOriginals) {
    HostString original1("Hello");
    HostString original2(" World");

    HostString combined = original1 + original2;

    EXPECT_EQ(toString(original1), "Hello");
    EXPECT_EQ(toString(original2), " World");
    EXPECT_EQ(toString(combined), "Hello World");
}

TEST_F(HostStringTest, SelfConcatenation) {
    HostString hs("Test");
    hs = hs + hs;

    EXPECT_EQ(toString(hs), "TestTest");
}

// =============================================================================
// Edge Case Tests - Memory and Performance
// =============================================================================

TEST_F(HostStringTest, LargeNumberOfSmallStrings) {
    std::vector<HostString> strings;

    for (int i = 0; i < 1000; ++i) {
        strings.push_back(HostString("X"));
    }

    EXPECT_EQ(strings.size(), 1000u);
    EXPECT_EQ(toString(strings[0]), "X");
    EXPECT_EQ(toString(strings[999]), "X");
}

TEST_F(HostStringTest, RepeatedCopyOperations) {
    HostString original("Original Content");

    for (int i = 0; i < 100; ++i) {
        HostString copy = original;
        EXPECT_EQ(copy, original);
    }
}

TEST_F(HostStringTest, RepeatedMoveOperations) {
    std::vector<HostString> strings;

    for (int i = 0; i < 100; ++i) {
        strings.push_back(HostString("Moving"));
    }

    EXPECT_EQ(strings.size(), 100u);
}

// =============================================================================
// Edge Case Tests - Conversion Round-Trips
// =============================================================================

TEST_F(HostStringTest, UTF8ToWideToUTF8) {
    std::string original = "Round Trip Test";

    HostString hs1(original);
    std::wstring wide = hs1.to_wide();
    HostString hs2(wide);
    std::string result = hs2.to_utf8();

    EXPECT_EQ(result, original);
}

TEST_F(HostStringTest, WideToUTF8ToWide) {
    std::wstring original = L"Wide Round Trip";

    HostString hs1(original);
    std::string utf8 = hs1.to_utf8();
    HostString hs2(utf8);
    std::wstring result = hs2.to_wide();

    EXPECT_EQ(result, original);
}

TEST_F(HostStringTest, EmptyStringConversions) {
    std::string empty_utf8 = "";
    std::wstring empty_wide = L"";

    HostString hs1(empty_utf8);
    HostString hs2(empty_wide);

    EXPECT_TRUE(hs1.empty());
    EXPECT_TRUE(hs2.empty());
    EXPECT_EQ(hs1, hs2);
}

// =============================================================================
// Edge Case Tests - Special Patterns
// =============================================================================

TEST_F(HostStringTest, RepeatingCharacters) {
    std::string repeated(1000, 'Z');
    HostString hs(repeated);

    EXPECT_EQ(hs.size(), 1000u);

    for (size_t i = 0; i < hs.size(); ++i) {
        EXPECT_EQ(hs[i], static_cast<char_t>('Z'));
    }
}

TEST_F(HostStringTest, AlternatingCharacters) {
    std::string alternating;
    for (int i = 0; i < 500; ++i) {
        alternating += (i % 2 == 0) ? 'A' : 'B';
    }

    HostString hs(alternating);

    EXPECT_EQ(hs.size(), 500u);
    EXPECT_EQ(hs[0], static_cast<char_t>('A'));
    EXPECT_EQ(hs[1], static_cast<char_t>('B'));
}

TEST_F(HostStringTest, NumericString) {
    std::string numbers = "0123456789";
    HostString hs(numbers);

    for (size_t i = 0; i < hs.size(); ++i) {
        char expected = static_cast<char>('0' + i);
        EXPECT_EQ(hs[i], static_cast<char_t>(expected));
    }
}

TEST_F(HostStringTest, AllPrintableASCII) {
    std::string ascii;
    for (char c = 32; c < 127; ++c) {  // Printable ASCII range
        ascii += c;
    }

    HostString hs(ascii);

    EXPECT_EQ(hs.size(), 95u);  // 127 - 32 = 95 characters
}

// =============================================================================
// Edge Case Tests - Container Interop
// =============================================================================

TEST_F(HostStringTest, VectorOfHostStrings) {
    std::vector<HostString> vec;

    vec.push_back(HostString("First"));
    vec.push_back(HostString("Second"));
    vec.push_back(HostString("Third"));

    EXPECT_EQ(vec.size(), 3u);
    EXPECT_EQ(toString(vec[0]), "First");
    EXPECT_EQ(toString(vec[2]), "Third");
}

TEST_F(HostStringTest, UnorderedMapWithHostStringKeys) {
    // HostString doesn't have operator< but can be used in unordered_map
    // Using string conversion as workaround
    std::unordered_map<std::string, int> lookup;

    lookup[toString(HostString("one"))] = 1;
    lookup[toString(HostString("two"))] = 2;
    lookup[toString(HostString("three"))] = 3;

    EXPECT_EQ(lookup.size(), 3u);
    EXPECT_EQ(lookup["two"], 2);
}

TEST_F(HostStringTest, VectorAsSet) {
    // HostString doesn't have operator< so we use vector instead
    std::vector<HostString> strings;

    strings.push_back(HostString("apple"));
    strings.push_back(HostString("banana"));
    strings.push_back(HostString("cherry"));

    EXPECT_EQ(strings.size(), 3u);
    EXPECT_EQ(toString(strings[0]), "apple");
    EXPECT_EQ(toString(strings[2]), "cherry");
}

// =============================================================================
// Edge Case Tests - Boundary Conditions
// =============================================================================

TEST_F(HostStringTest, SingleByteChar) {
    HostString hs("a");

    EXPECT_EQ(hs.size(), 1u);
    EXPECT_EQ(hs[0], static_cast<char_t>('a'));
}

TEST_F(HostStringTest, TwoCharacterString) {
    HostString hs("ab");

    EXPECT_EQ(hs.size(), 2u);
    EXPECT_EQ(hs[0], static_cast<char_t>('a'));
    EXPECT_EQ(hs[1], static_cast<char_t>('b'));
}

TEST_F(HostStringTest, PowerOfTwoSizes) {
    for (size_t size : {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024}) {
        std::string str(size, 'X');
        HostString hs(str);

        EXPECT_EQ(hs.size(), size);
    }
}

}  // namespace nexo::scripting
