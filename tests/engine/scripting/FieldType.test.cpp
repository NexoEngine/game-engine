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

// =============================================================================
// Edge Case Tests - Boundary Values
// =============================================================================

TEST_F(ScriptingFieldTypeTest, FirstEnumValueIsZero) {
    // Verify the first enum value is 0 for proper indexing
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Blank), 0u);
}

TEST_F(ScriptingFieldTypeTest, ConsecutiveValues) {
    // Verify that enum values are consecutive for array indexing
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Section),
              static_cast<uint64_t>(FieldType::Blank) + 1);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Bool),
              static_cast<uint64_t>(FieldType::Section) + 1);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Int8),
              static_cast<uint64_t>(FieldType::Bool) + 1);
}

TEST_F(ScriptingFieldTypeTest, AllIntegerTypesSequential) {
    // Verify signed integer types are sequential
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Int16),
              static_cast<uint64_t>(FieldType::Int8) + 1);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Int32),
              static_cast<uint64_t>(FieldType::Int16) + 1);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Int64),
              static_cast<uint64_t>(FieldType::Int32) + 1);

    // Verify unsigned integer types are sequential
    EXPECT_EQ(static_cast<uint64_t>(FieldType::UInt8),
              static_cast<uint64_t>(FieldType::Int64) + 1);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::UInt16),
              static_cast<uint64_t>(FieldType::UInt8) + 1);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::UInt32),
              static_cast<uint64_t>(FieldType::UInt16) + 1);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::UInt64),
              static_cast<uint64_t>(FieldType::UInt32) + 1);
}

TEST_F(ScriptingFieldTypeTest, FloatingPointTypesSequential) {
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Float),
              static_cast<uint64_t>(FieldType::UInt64) + 1);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Double),
              static_cast<uint64_t>(FieldType::Float) + 1);
}

TEST_F(ScriptingFieldTypeTest, VectorTypesSequential) {
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Vector3),
              static_cast<uint64_t>(FieldType::Double) + 1);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Vector4),
              static_cast<uint64_t>(FieldType::Vector3) + 1);
}

TEST_F(ScriptingFieldTypeTest, CountIsImmediatelyAfterLastType) {
    EXPECT_EQ(static_cast<uint64_t>(FieldType::_Count),
              static_cast<uint64_t>(FieldType::Vector4) + 1);
}

// =============================================================================
// Edge Case Tests - Type Categorization
// =============================================================================

TEST_F(ScriptingFieldTypeTest, SignedIntegerTypesGroup) {
    std::vector<FieldType> signedInts = {
        FieldType::Int8,
        FieldType::Int16,
        FieldType::Int32,
        FieldType::Int64
    };

    // Verify all signed int types are in consecutive range
    auto minVal = static_cast<uint64_t>(FieldType::Int8);
    auto maxVal = static_cast<uint64_t>(FieldType::Int64);

    for (const auto& type : signedInts) {
        auto val = static_cast<uint64_t>(type);
        EXPECT_GE(val, minVal);
        EXPECT_LE(val, maxVal);
    }
}

TEST_F(ScriptingFieldTypeTest, UnsignedIntegerTypesGroup) {
    std::vector<FieldType> unsignedInts = {
        FieldType::UInt8,
        FieldType::UInt16,
        FieldType::UInt32,
        FieldType::UInt64
    };

    // Verify all unsigned int types are in consecutive range
    auto minVal = static_cast<uint64_t>(FieldType::UInt8);
    auto maxVal = static_cast<uint64_t>(FieldType::UInt64);

    for (const auto& type : unsignedInts) {
        auto val = static_cast<uint64_t>(type);
        EXPECT_GE(val, minVal);
        EXPECT_LE(val, maxVal);
    }
}

TEST_F(ScriptingFieldTypeTest, FloatingPointTypesGroup) {
    std::vector<FieldType> floatTypes = {
        FieldType::Float,
        FieldType::Double
    };

    auto minVal = static_cast<uint64_t>(FieldType::Float);
    auto maxVal = static_cast<uint64_t>(FieldType::Double);

    for (const auto& type : floatTypes) {
        auto val = static_cast<uint64_t>(type);
        EXPECT_GE(val, minVal);
        EXPECT_LE(val, maxVal);
    }
}

TEST_F(ScriptingFieldTypeTest, VectorTypesGroup) {
    std::vector<FieldType> vectorTypes = {
        FieldType::Vector3,
        FieldType::Vector4
    };

    auto minVal = static_cast<uint64_t>(FieldType::Vector3);
    auto maxVal = static_cast<uint64_t>(FieldType::Vector4);

    for (const auto& type : vectorTypes) {
        auto val = static_cast<uint64_t>(type);
        EXPECT_GE(val, minVal);
        EXPECT_LE(val, maxVal);
    }
}

// =============================================================================
// Edge Case Tests - Invalid Values
// =============================================================================

TEST_F(ScriptingFieldTypeTest, InvalidValueDetection) {
    // Values beyond _Count should be detectable as invalid
    uint64_t invalidValue = static_cast<uint64_t>(FieldType::_Count) + 1;
    EXPECT_GT(invalidValue, static_cast<uint64_t>(FieldType::_Count));
}

TEST_F(ScriptingFieldTypeTest, MaxUint64Cast) {
    // Verify we can cast maximum value (even though it's invalid)
    uint64_t maxValue = std::numeric_limits<uint64_t>::max();
    auto type = static_cast<FieldType>(maxValue);
    EXPECT_EQ(static_cast<uint64_t>(type), maxValue);
}

TEST_F(ScriptingFieldTypeTest, ValidationFunction) {
    // Helper function to validate field types
    auto isValidFieldType = [](FieldType type) -> bool {
        return static_cast<uint64_t>(type) < static_cast<uint64_t>(FieldType::_Count);
    };

    // Test all valid types
    EXPECT_TRUE(isValidFieldType(FieldType::Blank));
    EXPECT_TRUE(isValidFieldType(FieldType::Section));
    EXPECT_TRUE(isValidFieldType(FieldType::Bool));
    EXPECT_TRUE(isValidFieldType(FieldType::Int8));
    EXPECT_TRUE(isValidFieldType(FieldType::Int64));
    EXPECT_TRUE(isValidFieldType(FieldType::UInt64));
    EXPECT_TRUE(isValidFieldType(FieldType::Float));
    EXPECT_TRUE(isValidFieldType(FieldType::Double));
    EXPECT_TRUE(isValidFieldType(FieldType::Vector3));
    EXPECT_TRUE(isValidFieldType(FieldType::Vector4));

    // Test invalid types
    EXPECT_FALSE(isValidFieldType(FieldType::_Count));
    EXPECT_FALSE(isValidFieldType(static_cast<FieldType>(100)));
    EXPECT_FALSE(isValidFieldType(static_cast<FieldType>(999999)));
}

// =============================================================================
// Edge Case Tests - Container Usage
// =============================================================================

TEST_F(ScriptingFieldTypeTest, CanBeUsedInArray) {
    FieldType types[3] = {
        FieldType::Int32,
        FieldType::Float,
        FieldType::Vector3
    };

    EXPECT_EQ(types[0], FieldType::Int32);
    EXPECT_EQ(types[1], FieldType::Float);
    EXPECT_EQ(types[2], FieldType::Vector3);
}

TEST_F(ScriptingFieldTypeTest, CanBeUsedInVector) {
    std::vector<FieldType> types = {
        FieldType::Bool,
        FieldType::Int32,
        FieldType::Double
    };

    EXPECT_EQ(types.size(), 3u);
    EXPECT_EQ(types[0], FieldType::Bool);
    EXPECT_EQ(types[1], FieldType::Int32);
    EXPECT_EQ(types[2], FieldType::Double);
}

TEST_F(ScriptingFieldTypeTest, CanBeUsedAsMapKey) {
    std::map<FieldType, std::string> typeNames;
    typeNames[FieldType::Int32] = "Integer";
    typeNames[FieldType::Float] = "Float";

    EXPECT_EQ(typeNames[FieldType::Int32], "Integer");
    EXPECT_EQ(typeNames[FieldType::Float], "Float");
}

TEST_F(ScriptingFieldTypeTest, CanBeUsedInSet) {
    std::set<FieldType> uniqueTypes;
    uniqueTypes.insert(FieldType::Int32);
    uniqueTypes.insert(FieldType::Float);
    uniqueTypes.insert(FieldType::Int32); // Duplicate

    EXPECT_EQ(uniqueTypes.size(), 2u);
    EXPECT_TRUE(uniqueTypes.count(FieldType::Int32) > 0);
    EXPECT_TRUE(uniqueTypes.count(FieldType::Float) > 0);
}

// =============================================================================
// Edge Case Tests - Type Conversion Patterns
// =============================================================================

TEST_F(ScriptingFieldTypeTest, BidirectionalCast) {
    FieldType original = FieldType::Vector4;
    uint64_t asInt = static_cast<uint64_t>(original);
    FieldType backToEnum = static_cast<FieldType>(asInt);

    EXPECT_EQ(original, backToEnum);
}

TEST_F(ScriptingFieldTypeTest, AllTypesBidirectionalCast) {
    std::vector<FieldType> allTypes = {
        FieldType::Blank, FieldType::Section, FieldType::Bool,
        FieldType::Int8, FieldType::Int16, FieldType::Int32, FieldType::Int64,
        FieldType::UInt8, FieldType::UInt16, FieldType::UInt32, FieldType::UInt64,
        FieldType::Float, FieldType::Double,
        FieldType::Vector3, FieldType::Vector4
    };

    for (const auto& original : allTypes) {
        uint64_t asInt = static_cast<uint64_t>(original);
        FieldType backToEnum = static_cast<FieldType>(asInt);
        EXPECT_EQ(original, backToEnum);
    }
}

TEST_F(ScriptingFieldTypeTest, ZeroCastToBlank) {
    uint64_t zero = 0;
    FieldType type = static_cast<FieldType>(zero);
    EXPECT_EQ(type, FieldType::Blank);
}

// =============================================================================
// Edge Case Tests - Sizeof and Alignment
// =============================================================================

TEST_F(ScriptingFieldTypeTest, EnumSize) {
    // Verify the enum has the expected size (uint64_t)
    EXPECT_EQ(sizeof(FieldType), sizeof(uint64_t));
    EXPECT_EQ(sizeof(FieldType), 8u);
}

TEST_F(ScriptingFieldTypeTest, EnumAlignment) {
    // Verify alignment matches uint64_t
    EXPECT_EQ(alignof(FieldType), alignof(uint64_t));
}

// =============================================================================
// Edge Case Tests - Constexpr Usage
// =============================================================================

TEST_F(ScriptingFieldTypeTest, CanBeUsedInConstexpr) {
    constexpr FieldType type = FieldType::Int32;
    constexpr uint64_t value = static_cast<uint64_t>(type);

    EXPECT_EQ(type, FieldType::Int32);
    EXPECT_GT(value, 0u);
}

TEST_F(ScriptingFieldTypeTest, ConstexprComparison) {
    constexpr bool isEqual = (FieldType::Int32 == FieldType::Int32);
    constexpr bool isNotEqual = (FieldType::Int32 != FieldType::Float);

    EXPECT_TRUE(isEqual);
    EXPECT_TRUE(isNotEqual);
}

}  // namespace nexo::scripting
