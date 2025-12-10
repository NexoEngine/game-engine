//// AssetPackName.test.cpp ////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for AssetPackName type alias
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/AssetPackName.hpp"
#include <type_traits>
#include <utility>

namespace nexo::assets {

// =============================================================================
// AssetPackNameValidator Tests
// =============================================================================

class AssetPackNameValidatorTest : public ::testing::Test {};

TEST_F(AssetPackNameValidatorTest, InheritsFromFilenameValidator) {
    static_assert(std::is_base_of_v<FilenameValidator, AssetPackNameValidator>);
    SUCCEED();
}

// =============================================================================
// AssetPackName Type Tests
// =============================================================================

class AssetPackNameTypeTest : public ::testing::Test {};

TEST_F(AssetPackNameTypeTest, IsValidatedNameSpecialization) {
    static_assert(std::is_same_v<AssetPackName, ValidatedName<AssetPackNameValidator>>);
    SUCCEED();
}

TEST_F(AssetPackNameTypeTest, IsCopyConstructible) {
    static_assert(std::is_copy_constructible_v<AssetPackName>);
    SUCCEED();
}

TEST_F(AssetPackNameTypeTest, IsMoveConstructible) {
    static_assert(std::is_move_constructible_v<AssetPackName>);
    SUCCEED();
}

TEST_F(AssetPackNameTypeTest, IsCopyAssignable) {
    static_assert(std::is_copy_assignable_v<AssetPackName>);
    SUCCEED();
}

TEST_F(AssetPackNameTypeTest, IsMoveAssignable) {
    static_assert(std::is_move_assignable_v<AssetPackName>);
    SUCCEED();
}

// =============================================================================
// AssetPackName Construction Tests
// =============================================================================

class AssetPackNameConstructionTest : public ::testing::Test {};

TEST_F(AssetPackNameConstructionTest, ConstructFromValidString) {
    AssetPackName packName("my-asset-pack");
    EXPECT_EQ(packName.data(), "my-asset-pack");
}

TEST_F(AssetPackNameConstructionTest, ConstructFromValidStringWithNumbers) {
    AssetPackName packName("pack123");
    EXPECT_EQ(packName.data(), "pack123");
}

TEST_F(AssetPackNameConstructionTest, ConstructFromValidStringWithUnderscores) {
    AssetPackName packName("my_asset_pack");
    EXPECT_EQ(packName.data(), "my_asset_pack");
}

TEST_F(AssetPackNameConstructionTest, ConstructFromInvalidStringThrows) {
    EXPECT_THROW(AssetPackName("invalid/pack"), InvalidName);
}

TEST_F(AssetPackNameConstructionTest, ConstructFromEmptyStringThrows) {
    EXPECT_THROW(AssetPackName(""), InvalidName);
}

TEST_F(AssetPackNameConstructionTest, ConstructFromStdString) {
    std::string name = "string-pack";
    AssetPackName packName(name);
    EXPECT_EQ(packName.data(), "string-pack");
}

TEST_F(AssetPackNameConstructionTest, ConstructFromStringView) {
    std::string_view name = "view-pack";
    AssetPackName packName(name);
    EXPECT_EQ(packName.data(), "view-pack");
}

// =============================================================================
// AssetPackName Operations Tests
// =============================================================================

class AssetPackNameOperationsTest : public ::testing::Test {};

TEST_F(AssetPackNameOperationsTest, EqualityOperator) {
    AssetPackName packName1("pack-alpha");
    AssetPackName packName2("pack-beta");
    AssetPackName packName1Copy("pack-alpha");

    EXPECT_TRUE(packName1 == packName1Copy);
    EXPECT_FALSE(packName1 == packName2);
}

TEST_F(AssetPackNameOperationsTest, InequalityOperator) {
    AssetPackName packName1("pack-alpha");
    AssetPackName packName2("pack-beta");
    AssetPackName packName1Copy("pack-alpha");

    EXPECT_FALSE(packName1 != packName1Copy);
    EXPECT_TRUE(packName1 != packName2);
}

TEST_F(AssetPackNameOperationsTest, DataReturnsUnderlyingString) {
    AssetPackName packName1("pack-alpha");
    AssetPackName packName2("pack-beta");

    EXPECT_EQ(packName1.data(), "pack-alpha");
    EXPECT_EQ(packName2.data(), "pack-beta");
}

TEST_F(AssetPackNameOperationsTest, CStrReturnsPointer) {
    AssetPackName packName("test-pack");
    EXPECT_STREQ(packName.c_str(), "test-pack");
}

TEST_F(AssetPackNameOperationsTest, SizeReturnsCorrectLength) {
    AssetPackName packName("12345");
    EXPECT_EQ(packName.size(), 5u);
}

TEST_F(AssetPackNameOperationsTest, CopyConstruction) {
    AssetPackName packName1("pack-alpha");
    AssetPackName copy = packName1;
    EXPECT_EQ(copy.data(), packName1.data());
}

TEST_F(AssetPackNameOperationsTest, MoveConstruction) {
    AssetPackName original("movable");
    AssetPackName moved = std::move(original);
    EXPECT_EQ(moved.data(), "movable");
}

TEST_F(AssetPackNameOperationsTest, CopyAssignment) {
    AssetPackName packName1("pack-alpha");
    AssetPackName assigned("temp");
    assigned = packName1;
    EXPECT_EQ(assigned.data(), packName1.data());
}

TEST_F(AssetPackNameOperationsTest, MoveAssignment) {
    AssetPackName original("to-move");
    AssetPackName assigned("temp");
    assigned = std::move(original);
    EXPECT_EQ(assigned.data(), "to-move");
}

// =============================================================================
// AssetPackName Usage Tests
// =============================================================================

class AssetPackNameUsageTest : public ::testing::Test {};

TEST_F(AssetPackNameUsageTest, CanBeUsedInPair) {
    AssetPackName pack("pack1");
    std::pair<AssetPackName, int> pair = {pack, 100};

    EXPECT_EQ(pair.first.data(), "pack1");
    EXPECT_EQ(pair.second, 100);
}

TEST_F(AssetPackNameUsageTest, CanBeStoredInVector) {
    std::vector<AssetPackName> packs;

    packs.emplace_back("pack-a");
    packs.emplace_back("pack-b");
    packs.emplace_back("pack-c");

    EXPECT_EQ(packs.size(), 3u);
    EXPECT_EQ(packs[0].data(), "pack-a");
    EXPECT_EQ(packs[2].data(), "pack-c");
}

TEST_F(AssetPackNameUsageTest, ExplicitStringConversion) {
    AssetPackName packName("convert-me");
    std::string str = static_cast<std::string>(packName);
    EXPECT_EQ(str, "convert-me");
}

TEST_F(AssetPackNameUsageTest, ExplicitStringViewConversion) {
    AssetPackName packName("view-me");
    std::string_view sv = static_cast<std::string_view>(packName);
    EXPECT_EQ(sv, "view-me");
}

// =============================================================================
// AssetPackName Validation Tests
// =============================================================================

class AssetPackNameValidationTest : public ::testing::Test {};

TEST_F(AssetPackNameValidationTest, AcceptsSimpleNames) {
    EXPECT_NO_THROW(AssetPackName("simple"));
    EXPECT_NO_THROW(AssetPackName("another"));
}

TEST_F(AssetPackNameValidationTest, AcceptsNamesWithHyphens) {
    EXPECT_NO_THROW(AssetPackName("my-pack"));
    EXPECT_NO_THROW(AssetPackName("a-b-c"));
}

TEST_F(AssetPackNameValidationTest, AcceptsNamesWithUnderscores) {
    EXPECT_NO_THROW(AssetPackName("my_pack"));
    EXPECT_NO_THROW(AssetPackName("a_b_c"));
}

TEST_F(AssetPackNameValidationTest, AcceptsNamesWithNumbers) {
    EXPECT_NO_THROW(AssetPackName("pack1"));
    EXPECT_NO_THROW(AssetPackName("123pack"));
    EXPECT_NO_THROW(AssetPackName("pack123"));
}

TEST_F(AssetPackNameValidationTest, AcceptsMixedValidCharacters) {
    EXPECT_NO_THROW(AssetPackName("my_pack-v2"));
    EXPECT_NO_THROW(AssetPackName("Pack_Name-123"));
}

TEST_F(AssetPackNameValidationTest, RejectsSlashes) {
    EXPECT_THROW(AssetPackName("pack/name"), InvalidName);
    EXPECT_THROW(AssetPackName("pack\\name"), InvalidName);
}

TEST_F(AssetPackNameValidationTest, RejectsColons) {
    EXPECT_THROW(AssetPackName("pack:name"), InvalidName);
}

TEST_F(AssetPackNameValidationTest, RejectsSpecialCharacters) {
    EXPECT_THROW(AssetPackName("pack*name"), InvalidName);
    EXPECT_THROW(AssetPackName("pack?name"), InvalidName);
    EXPECT_THROW(AssetPackName("pack<name>"), InvalidName);
    EXPECT_THROW(AssetPackName("pack|name"), InvalidName);
}

TEST_F(AssetPackNameValidationTest, RejectsQuotes) {
    EXPECT_THROW(AssetPackName("pack\"name"), InvalidName);
}

TEST_F(AssetPackNameValidationTest, RejectsEmpty) {
    EXPECT_THROW(AssetPackName(""), InvalidName);
}

// =============================================================================
// AssetPackName Rename Tests
// =============================================================================

class AssetPackNameRenameTest : public ::testing::Test {};

TEST_F(AssetPackNameRenameTest, RenameToValidName) {
    AssetPackName packName("original");
    auto result = packName.rename("renamed");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(packName.data(), "renamed");
}

TEST_F(AssetPackNameRenameTest, RenameToInvalidNameReturnsError) {
    AssetPackName packName("original");
    auto result = packName.rename("invalid/name");
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(packName.data(), "original");  // Name should not change
}

TEST_F(AssetPackNameRenameTest, RenameToEmptyReturnsError) {
    AssetPackName packName("original");
    auto result = packName.rename("");
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(packName.data(), "original");
}

// =============================================================================
// AssetPackName Static Validation Tests
// =============================================================================

class AssetPackNameStaticValidationTest : public ::testing::Test {};

TEST_F(AssetPackNameStaticValidationTest, ValidateReturnsNulloptForValid) {
    auto result = AssetPackName::validate("valid-name");
    EXPECT_FALSE(result.has_value());
}

TEST_F(AssetPackNameStaticValidationTest, ValidateReturnsErrorForInvalid) {
    auto result = AssetPackName::validate("invalid/name");
    EXPECT_TRUE(result.has_value());
}

TEST_F(AssetPackNameStaticValidationTest, ValidateReturnsErrorForEmpty) {
    auto result = AssetPackName::validate("");
    EXPECT_TRUE(result.has_value());
}

}  // namespace nexo::assets
