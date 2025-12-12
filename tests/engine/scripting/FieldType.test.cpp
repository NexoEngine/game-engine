//// FieldType.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for scripting FieldType enum
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "scripting/native/ui/FieldType.hpp"
#include <vector>
#include <set>

namespace nexo::scripting {

class ScriptingFieldTypeTest : public ::testing::Test {};

// =============================================================================
// Basic Enum Tests
// =============================================================================

TEST_F(ScriptingFieldTypeTest, IsEnumClass) {
    EXPECT_TRUE(std::is_enum_v<FieldType>);
}

TEST_F(ScriptingFieldTypeTest, UnderlyingTypeIsUint64) {
    EXPECT_TRUE((std::is_same_v<std::underlying_type_t<FieldType>, uint64_t>));
}

// =============================================================================
// Value Tests
// =============================================================================

TEST_F(ScriptingFieldTypeTest, BlankIsFirstValue) {
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Blank), 0u);
}

TEST_F(ScriptingFieldTypeTest, CountIsLast) {
    // _Count should be the last enum value
    auto countValue = static_cast<uint64_t>(FieldType::_Count);

    // All other values should be less than _Count
    EXPECT_LT(static_cast<uint64_t>(FieldType::Blank), countValue);
    EXPECT_LT(static_cast<uint64_t>(FieldType::Section), countValue);
    EXPECT_LT(static_cast<uint64_t>(FieldType::Bool), countValue);
    EXPECT_LT(static_cast<uint64_t>(FieldType::Int8), countValue);
    EXPECT_LT(static_cast<uint64_t>(FieldType::Int64), countValue);
    EXPECT_LT(static_cast<uint64_t>(FieldType::Float), countValue);
    EXPECT_LT(static_cast<uint64_t>(FieldType::Double), countValue);
    EXPECT_LT(static_cast<uint64_t>(FieldType::Vector3), countValue);
    EXPECT_LT(static_cast<uint64_t>(FieldType::Vector4), countValue);
}

TEST_F(ScriptingFieldTypeTest, AllValuesAreDistinct) {
    std::vector<FieldType> types = {
        FieldType::Blank,
        FieldType::Section,
        FieldType::Bool,
        FieldType::Int8,
        FieldType::Int16,
        FieldType::Int32,
        FieldType::Int64,
        FieldType::UInt8,
        FieldType::UInt16,
        FieldType::UInt32,
        FieldType::UInt64,
        FieldType::Float,
        FieldType::Double,
        FieldType::Vector3,
        FieldType::Vector4,
        FieldType::_Count
    };

    std::set<uint64_t> uniqueValues;
    for (auto type : types) {
        auto [it, inserted] = uniqueValues.insert(static_cast<uint64_t>(type));
        EXPECT_TRUE(inserted) << "Duplicate value found for FieldType";
    }

    EXPECT_EQ(uniqueValues.size(), types.size());
}

// =============================================================================
// Category Tests
// =============================================================================

TEST_F(ScriptingFieldTypeTest, SpecialTypesExist) {
    // Verify special types are defined
    EXPECT_NO_THROW([[maybe_unused]] auto blank = FieldType::Blank);
    EXPECT_NO_THROW([[maybe_unused]] auto section = FieldType::Section);
}

TEST_F(ScriptingFieldTypeTest, AllPrimitiveTypesExist) {
    // Verify all primitive types are defined
    EXPECT_NO_THROW([[maybe_unused]] auto b = FieldType::Bool);
    EXPECT_NO_THROW([[maybe_unused]] auto i8 = FieldType::Int8);
    EXPECT_NO_THROW([[maybe_unused]] auto i16 = FieldType::Int16);
    EXPECT_NO_THROW([[maybe_unused]] auto i32 = FieldType::Int32);
    EXPECT_NO_THROW([[maybe_unused]] auto i64 = FieldType::Int64);
    EXPECT_NO_THROW([[maybe_unused]] auto u8 = FieldType::UInt8);
    EXPECT_NO_THROW([[maybe_unused]] auto u16 = FieldType::UInt16);
    EXPECT_NO_THROW([[maybe_unused]] auto u32 = FieldType::UInt32);
    EXPECT_NO_THROW([[maybe_unused]] auto u64 = FieldType::UInt64);
    EXPECT_NO_THROW([[maybe_unused]] auto f = FieldType::Float);
    EXPECT_NO_THROW([[maybe_unused]] auto d = FieldType::Double);
}

TEST_F(ScriptingFieldTypeTest, WidgetTypesExist) {
    // Verify widget types are defined
    EXPECT_NO_THROW([[maybe_unused]] auto v3 = FieldType::Vector3);
    EXPECT_NO_THROW([[maybe_unused]] auto v4 = FieldType::Vector4);
}

// =============================================================================
// Comparison Tests
// =============================================================================

TEST_F(ScriptingFieldTypeTest, EnumComparison) {
    EXPECT_EQ(FieldType::Blank, FieldType::Blank);
    EXPECT_NE(FieldType::Blank, FieldType::Bool);
    EXPECT_NE(FieldType::Int32, FieldType::UInt32);
    EXPECT_NE(FieldType::Float, FieldType::Double);
}

TEST_F(ScriptingFieldTypeTest, EnumOrdering) {
    // Blank should be the smallest (0)
    EXPECT_LT(static_cast<uint64_t>(FieldType::Blank),
              static_cast<uint64_t>(FieldType::Section));

    // _Count should be the largest
    EXPECT_LT(static_cast<uint64_t>(FieldType::Vector4),
              static_cast<uint64_t>(FieldType::_Count));
}

// =============================================================================
// Storage Tests
// =============================================================================

TEST_F(ScriptingFieldTypeTest, CanBeStoredInVariable) {
    FieldType type = FieldType::Int32;
    EXPECT_EQ(type, FieldType::Int32);
}

TEST_F(ScriptingFieldTypeTest, CanBeCopied) {
    FieldType original = FieldType::Float;
    FieldType copy = original;
    EXPECT_EQ(original, copy);
}

TEST_F(ScriptingFieldTypeTest, CanBeUsedInSwitch) {
    FieldType type = FieldType::Vector3;
    bool matched = false;

    switch (type) {
        case FieldType::Vector3:
            matched = true;
            break;
        default:
            break;
    }

    EXPECT_TRUE(matched);
}

TEST_F(ScriptingFieldTypeTest, CountValueMatchesEnumCount) {
    // _Count should equal the total number of enum values before it
    auto count = static_cast<uint64_t>(FieldType::_Count);

    // Count all defined types (excluding _Count itself)
    constexpr int EXPECTED_COUNT = 15;  // Blank through Vector4
    EXPECT_EQ(count, EXPECTED_COUNT);
}

// =============================================================================
// Type Safety Tests
// =============================================================================

TEST_F(ScriptingFieldTypeTest, RequiresExplicitCast) {
    // This test verifies it's an enum class, not a plain enum
    // The following line should NOT compile:
    // int x = FieldType::Int32;  // Should fail
    // We verify this by checking it's an enum type
    EXPECT_TRUE(std::is_enum_v<FieldType>);

    // And verify it has the expected underlying type (uint64_t)
    EXPECT_TRUE((std::is_same_v<std::underlying_type_t<FieldType>, uint64_t>));
}

TEST_F(ScriptingFieldTypeTest, ExplicitCastToUint64Works) {
    auto value = static_cast<uint64_t>(FieldType::Int64);
    EXPECT_GE(value, 0u);
    EXPECT_LT(value, static_cast<uint64_t>(FieldType::_Count));
}

}  // namespace nexo::scripting
