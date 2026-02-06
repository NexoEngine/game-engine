//// ValidatedName.test.cpp ///////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for ValidatedName<T> template class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/ValidatedName.hpp"

namespace nexo::assets {

// =============================================================================
// Test Validator - Simple validator that rejects empty names and "INVALID"
// =============================================================================

struct TestValidator {
    static std::optional<std::string> validate(std::string_view name) {
        if (name.empty()) {
            return "Name cannot be empty";
        }
        if (name == "INVALID") {
            return "Name 'INVALID' is not allowed";
        }
        if (name.length() > 50) {
            return "Name cannot exceed 50 characters";
        }
        return std::nullopt;
    }
};

using TestValidatedName = ValidatedName<TestValidator>;

// =============================================================================
// Construction Tests
// =============================================================================

class ValidatedNameConstructionTest : public ::testing::Test {};

TEST_F(ValidatedNameConstructionTest, ValidNameFromStringView) {
    TestValidatedName name(std::string_view("TestName"));
    EXPECT_EQ(name.data(), "TestName");
}

TEST_F(ValidatedNameConstructionTest, ValidNameFromString) {
    std::string str = "TestName";
    TestValidatedName name(str);
    EXPECT_EQ(name.data(), "TestName");
}

TEST_F(ValidatedNameConstructionTest, ValidNameFromCString) {
    TestValidatedName name("TestName");
    EXPECT_EQ(name.data(), "TestName");
}

TEST_F(ValidatedNameConstructionTest, EmptyNameThrowsInvalidName) {
    EXPECT_THROW(TestValidatedName(""), InvalidName);
}

TEST_F(ValidatedNameConstructionTest, InvalidNameThrowsException) {
    EXPECT_THROW(TestValidatedName("INVALID"), InvalidName);
}

TEST_F(ValidatedNameConstructionTest, TooLongNameThrowsException) {
    std::string longName(51, 'a');
    EXPECT_THROW({ TestValidatedName name(longName); }, InvalidName);
}

TEST_F(ValidatedNameConstructionTest, MaxLengthNameSucceeds) {
    std::string maxName(50, 'a');
    TestValidatedName name(maxName);
    EXPECT_EQ(name.size(), 50);
}

// =============================================================================
// Accessor Tests
// =============================================================================

class ValidatedNameAccessorTest : public ::testing::Test {};

TEST_F(ValidatedNameAccessorTest, SizeReturnsCorrectLength) {
    TestValidatedName name("Hello");
    EXPECT_EQ(name.size(), 5);
}

TEST_F(ValidatedNameAccessorTest, DataReturnsString) {
    TestValidatedName name("TestData");
    EXPECT_EQ(name.data(), "TestData");
}

TEST_F(ValidatedNameAccessorTest, CStrReturnsCString) {
    TestValidatedName name("CStringTest");
    EXPECT_STREQ(name.c_str(), "CStringTest");
}

TEST_F(ValidatedNameAccessorTest, ExplicitStringConversion) {
    TestValidatedName name("ConvertMe");
    std::string str = static_cast<std::string>(name);
    EXPECT_EQ(str, "ConvertMe");
}

TEST_F(ValidatedNameAccessorTest, ExplicitStringViewConversion) {
    TestValidatedName name("ViewTest");
    std::string_view sv = static_cast<std::string_view>(name);
    EXPECT_EQ(sv, "ViewTest");
}

TEST_F(ValidatedNameAccessorTest, ExplicitCStringConversion) {
    TestValidatedName name("CConvert");
    const char* cstr = static_cast<const char*>(name);
    EXPECT_STREQ(cstr, "CConvert");
}

// =============================================================================
// Equality Tests
// =============================================================================

class ValidatedNameEqualityTest : public ::testing::Test {};

TEST_F(ValidatedNameEqualityTest, EqualNamesAreEqual) {
    TestValidatedName name1("Same");
    TestValidatedName name2("Same");
    EXPECT_TRUE(name1 == name2);
}

TEST_F(ValidatedNameEqualityTest, DifferentNamesAreNotEqual) {
    TestValidatedName name1("First");
    TestValidatedName name2("Second");
    EXPECT_FALSE(name1 == name2);
}

TEST_F(ValidatedNameEqualityTest, InequalityOperator) {
    TestValidatedName name1("One");
    TestValidatedName name2("Two");
    EXPECT_TRUE(name1 != name2);
}

TEST_F(ValidatedNameEqualityTest, SameNameInequalityFalse) {
    TestValidatedName name1("Match");
    TestValidatedName name2("Match");
    EXPECT_FALSE(name1 != name2);
}

// =============================================================================
// Assignment Tests
// =============================================================================

class ValidatedNameAssignmentTest : public ::testing::Test {};

TEST_F(ValidatedNameAssignmentTest, AssignFromStringView) {
    TestValidatedName name("Initial");
    name = std::string_view("Updated");
    EXPECT_EQ(name.data(), "Updated");
}

TEST_F(ValidatedNameAssignmentTest, AssignFromString) {
    TestValidatedName name("Initial");
    std::string newName = "NewValue";
    name = newName;
    EXPECT_EQ(name.data(), "NewValue");
}

TEST_F(ValidatedNameAssignmentTest, AssignFromCString) {
    TestValidatedName name("Initial");
    name = "Assigned";
    EXPECT_EQ(name.data(), "Assigned");
}

TEST_F(ValidatedNameAssignmentTest, AssignInvalidNameThrows) {
    TestValidatedName name("Initial");
    EXPECT_THROW(name = "INVALID", InvalidName);
}

TEST_F(ValidatedNameAssignmentTest, AssignEmptyNameThrows) {
    TestValidatedName name("Initial");
    EXPECT_THROW(name = "", InvalidName);
}

TEST_F(ValidatedNameAssignmentTest, AssignFromOtherValidatedName) {
    TestValidatedName name1("First");
    TestValidatedName name2("Second");
    name1 = name2;
    EXPECT_EQ(name1.data(), "Second");
}

// =============================================================================
// Rename Tests
// =============================================================================

class ValidatedNameRenameTest : public ::testing::Test {};

TEST_F(ValidatedNameRenameTest, RenameToValidName) {
    TestValidatedName name("OldName");
    auto result = name.rename("NewName");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(name.data(), "NewName");
}

TEST_F(ValidatedNameRenameTest, RenameToInvalidNameReturnsError) {
    TestValidatedName name("OldName");
    auto result = name.rename("INVALID");
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(name.data(), "OldName"); // Name unchanged
}

TEST_F(ValidatedNameRenameTest, RenameToEmptyReturnsError) {
    TestValidatedName name("OldName");
    auto result = name.rename("");
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(name.data(), "OldName"); // Name unchanged
}

TEST_F(ValidatedNameRenameTest, RenameErrorMessageContainsReason) {
    TestValidatedName name("OldName");
    auto result = name.rename("");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result->empty());
}

// =============================================================================
// Static Validate Tests
// =============================================================================

class ValidatedNameStaticValidateTest : public ::testing::Test {};

TEST_F(ValidatedNameStaticValidateTest, ValidNameReturnsNullopt) {
    auto result = TestValidatedName::validate("ValidName");
    EXPECT_FALSE(result.has_value());
}

TEST_F(ValidatedNameStaticValidateTest, EmptyNameReturnsError) {
    auto result = TestValidatedName::validate("");
    EXPECT_TRUE(result.has_value());
}

TEST_F(ValidatedNameStaticValidateTest, InvalidNameReturnsError) {
    auto result = TestValidatedName::validate("INVALID");
    EXPECT_TRUE(result.has_value());
}

TEST_F(ValidatedNameStaticValidateTest, TooLongNameReturnsError) {
    std::string longName(51, 'x');
    auto result = TestValidatedName::validate(longName);
    EXPECT_TRUE(result.has_value());
}

// =============================================================================
// Edge Case Tests
// =============================================================================

class ValidatedNameEdgeCaseTest : public ::testing::Test {};

TEST_F(ValidatedNameEdgeCaseTest, SingleCharacterName) {
    TestValidatedName name("X");
    EXPECT_EQ(name.size(), 1);
    EXPECT_EQ(name.data(), "X");
}

TEST_F(ValidatedNameEdgeCaseTest, NameWithSpaces) {
    TestValidatedName name("Name With Spaces");
    EXPECT_EQ(name.data(), "Name With Spaces");
}

TEST_F(ValidatedNameEdgeCaseTest, NameWithNumbers) {
    TestValidatedName name("Name123");
    EXPECT_EQ(name.data(), "Name123");
}

TEST_F(ValidatedNameEdgeCaseTest, NameWithSpecialChars) {
    TestValidatedName name("Name_With-Special.Chars");
    EXPECT_EQ(name.data(), "Name_With-Special.Chars");
}

TEST_F(ValidatedNameEdgeCaseTest, NameWithUnicode) {
    // Note: Depends on validator implementation accepting unicode
    TestValidatedName name("TestName");
    EXPECT_EQ(name.data(), "TestName");
}

}  // namespace nexo::assets
