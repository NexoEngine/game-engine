//// FieldType.test.cpp ///////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Comprehensive test file for ECS FieldType enum
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ecs/TypeErasedComponent/FieldType.hpp"
#include <vector>
#include <set>
#include <type_traits>

namespace nexo::ecs {

class ECSFieldTypeTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// =============================================================================
// Type Traits Tests
// =============================================================================

TEST_F(ECSFieldTypeTest, IsEnum) {
    EXPECT_TRUE(std::is_enum_v<FieldType>);
}

TEST_F(ECSFieldTypeTest, IsEnumClass) {
    // Test that it's a scoped enum (enum class)
    EXPECT_TRUE(std::is_enum_v<FieldType>);
    // Cannot implicitly convert to int
    EXPECT_FALSE((std::is_convertible_v<FieldType, int>));
}

TEST_F(ECSFieldTypeTest, UnderlyingTypeIsUint64) {
    EXPECT_TRUE((std::is_same_v<std::underlying_type_t<FieldType>, uint64_t>));
}

TEST_F(ECSFieldTypeTest, SizeOfFieldType) {
    EXPECT_EQ(sizeof(FieldType), sizeof(uint64_t));
}

// =============================================================================
// Enum Values Tests
// =============================================================================

TEST_F(ECSFieldTypeTest, BlankValueIsZero) {
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Blank), 0u);
}

TEST_F(ECSFieldTypeTest, SectionValueIsOne) {
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Section), 1u);
}

TEST_F(ECSFieldTypeTest, AllValuesAreDistinct) {
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
        EXPECT_TRUE(inserted) << "Duplicate value found for FieldType: "
                              << static_cast<uint64_t>(type);
    }

    EXPECT_EQ(uniqueValues.size(), types.size());
    EXPECT_EQ(uniqueValues.size(), 16u);
}

TEST_F(ECSFieldTypeTest, SequentialValues) {
    // Test that values are sequential from 0 to _Count - 1
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Blank), 0u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Section), 1u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Bool), 2u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Int8), 3u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Int16), 4u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Int32), 5u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Int64), 6u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::UInt8), 7u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::UInt16), 8u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::UInt32), 9u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::UInt64), 10u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Float), 11u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Double), 12u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Vector3), 13u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Vector4), 14u);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::_Count), 15u);
}

// =============================================================================
// _Count Value Tests
// =============================================================================

TEST_F(ECSFieldTypeTest, CountEqualsNumberOfActualTypes) {
    // _Count should equal 15 (number of actual field types)
    EXPECT_EQ(static_cast<uint64_t>(FieldType::_Count), 15u);
}

TEST_F(ECSFieldTypeTest, CountIsLastValue) {
    // All other values should be less than _Count
    EXPECT_LT(static_cast<uint64_t>(FieldType::Blank),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Section),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Bool),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Int8),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Int16),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Int32),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Int64),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::UInt8),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::UInt16),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::UInt32),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::UInt64),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Float),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Double),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Vector3),
              static_cast<uint64_t>(FieldType::_Count));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Vector4),
              static_cast<uint64_t>(FieldType::_Count));
}

// =============================================================================
// Category Tests - Special Types
// =============================================================================

TEST_F(ECSFieldTypeTest, SpecialTypesExist) {
    // Verify special types are defined and accessible
    EXPECT_NO_THROW([[maybe_unused]] auto blank = FieldType::Blank);
    EXPECT_NO_THROW([[maybe_unused]] auto section = FieldType::Section);
}

TEST_F(ECSFieldTypeTest, BlankTypeIsFirst) {
    // Blank should be the first value (0)
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Blank), 0u);
}

// =============================================================================
// Category Tests - Primitive Types
// =============================================================================

TEST_F(ECSFieldTypeTest, BooleanTypeExists) {
    EXPECT_NO_THROW([[maybe_unused]] auto b = FieldType::Bool);
    EXPECT_EQ(static_cast<uint64_t>(FieldType::Bool), 2u);
}

TEST_F(ECSFieldTypeTest, SignedIntegerTypesExist) {
    EXPECT_NO_THROW([[maybe_unused]] auto i8 = FieldType::Int8);
    EXPECT_NO_THROW([[maybe_unused]] auto i16 = FieldType::Int16);
    EXPECT_NO_THROW([[maybe_unused]] auto i32 = FieldType::Int32);
    EXPECT_NO_THROW([[maybe_unused]] auto i64 = FieldType::Int64);
}

TEST_F(ECSFieldTypeTest, UnsignedIntegerTypesExist) {
    EXPECT_NO_THROW([[maybe_unused]] auto u8 = FieldType::UInt8);
    EXPECT_NO_THROW([[maybe_unused]] auto u16 = FieldType::UInt16);
    EXPECT_NO_THROW([[maybe_unused]] auto u32 = FieldType::UInt32);
    EXPECT_NO_THROW([[maybe_unused]] auto u64 = FieldType::UInt64);
}

TEST_F(ECSFieldTypeTest, FloatingPointTypesExist) {
    EXPECT_NO_THROW([[maybe_unused]] auto f = FieldType::Float);
    EXPECT_NO_THROW([[maybe_unused]] auto d = FieldType::Double);
}

TEST_F(ECSFieldTypeTest, IntegerTypesAreOrdered) {
    // Test that signed integers are in size order
    EXPECT_LT(static_cast<uint64_t>(FieldType::Int8),
              static_cast<uint64_t>(FieldType::Int16));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Int16),
              static_cast<uint64_t>(FieldType::Int32));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Int32),
              static_cast<uint64_t>(FieldType::Int64));

    // Test that unsigned integers are in size order
    EXPECT_LT(static_cast<uint64_t>(FieldType::UInt8),
              static_cast<uint64_t>(FieldType::UInt16));
    EXPECT_LT(static_cast<uint64_t>(FieldType::UInt16),
              static_cast<uint64_t>(FieldType::UInt32));
    EXPECT_LT(static_cast<uint64_t>(FieldType::UInt32),
              static_cast<uint64_t>(FieldType::UInt64));
}

// =============================================================================
// Category Tests - Widget Types
// =============================================================================

TEST_F(ECSFieldTypeTest, VectorTypesExist) {
    EXPECT_NO_THROW([[maybe_unused]] auto v3 = FieldType::Vector3);
    EXPECT_NO_THROW([[maybe_unused]] auto v4 = FieldType::Vector4);
}

TEST_F(ECSFieldTypeTest, VectorTypesAreOrdered) {
    EXPECT_LT(static_cast<uint64_t>(FieldType::Vector3),
              static_cast<uint64_t>(FieldType::Vector4));
}

TEST_F(ECSFieldTypeTest, VectorTypesAreAfterPrimitives) {
    EXPECT_GT(static_cast<uint64_t>(FieldType::Vector3),
              static_cast<uint64_t>(FieldType::Double));
    EXPECT_GT(static_cast<uint64_t>(FieldType::Vector4),
              static_cast<uint64_t>(FieldType::Double));
}

// =============================================================================
// Comparison Tests
// =============================================================================

TEST_F(ECSFieldTypeTest, EqualityComparison) {
    EXPECT_EQ(FieldType::Blank, FieldType::Blank);
    EXPECT_EQ(FieldType::Int32, FieldType::Int32);
    EXPECT_EQ(FieldType::Vector3, FieldType::Vector3);
}

TEST_F(ECSFieldTypeTest, InequalityComparison) {
    EXPECT_NE(FieldType::Blank, FieldType::Bool);
    EXPECT_NE(FieldType::Int32, FieldType::UInt32);
    EXPECT_NE(FieldType::Float, FieldType::Double);
    EXPECT_NE(FieldType::Vector3, FieldType::Vector4);
}

TEST_F(ECSFieldTypeTest, OrderingComparison) {
    EXPECT_LT(static_cast<uint64_t>(FieldType::Blank),
              static_cast<uint64_t>(FieldType::Section));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Bool),
              static_cast<uint64_t>(FieldType::Int8));
    EXPECT_LT(static_cast<uint64_t>(FieldType::Vector4),
              static_cast<uint64_t>(FieldType::_Count));
}

// =============================================================================
// Storage and Usage Tests
// =============================================================================

TEST_F(ECSFieldTypeTest, CanBeStoredInVariable) {
    FieldType type = FieldType::Int32;
    EXPECT_EQ(type, FieldType::Int32);
}

TEST_F(ECSFieldTypeTest, CanBeCopied) {
    FieldType original = FieldType::Float;
    FieldType copy = original;
    EXPECT_EQ(original, copy);
}

TEST_F(ECSFieldTypeTest, CanBeAssigned) {
    FieldType type = FieldType::Int8;
    type = FieldType::Double;
    EXPECT_EQ(type, FieldType::Double);
}

TEST_F(ECSFieldTypeTest, CanBeUsedInSwitch) {
    FieldType type = FieldType::Vector3;
    bool matched = false;

    switch (type) {
        case FieldType::Blank:
            break;
        case FieldType::Section:
            break;
        case FieldType::Bool:
            break;
        case FieldType::Int8:
            break;
        case FieldType::Int16:
            break;
        case FieldType::Int32:
            break;
        case FieldType::Int64:
            break;
        case FieldType::UInt8:
            break;
        case FieldType::UInt16:
            break;
        case FieldType::UInt32:
            break;
        case FieldType::UInt64:
            break;
        case FieldType::Float:
            break;
        case FieldType::Double:
            break;
        case FieldType::Vector3:
            matched = true;
            break;
        case FieldType::Vector4:
            break;
        case FieldType::_Count:
            break;
    }

    EXPECT_TRUE(matched);
}

TEST_F(ECSFieldTypeTest, CanBeStoredInVector) {
    std::vector<FieldType> types = {
        FieldType::Int32,
        FieldType::Float,
        FieldType::Vector3
    };

    EXPECT_EQ(types.size(), 3u);
    EXPECT_EQ(types[0], FieldType::Int32);
    EXPECT_EQ(types[1], FieldType::Float);
    EXPECT_EQ(types[2], FieldType::Vector3);
}

TEST_F(ECSFieldTypeTest, CanBeUsedAsMapKey) {
    std::map<FieldType, std::string> typeNames;
    typeNames[FieldType::Int32] = "Int32";
    typeNames[FieldType::Float] = "Float";
    typeNames[FieldType::Vector3] = "Vector3";

    EXPECT_EQ(typeNames.size(), 3u);
    EXPECT_EQ(typeNames[FieldType::Int32], "Int32");
    EXPECT_EQ(typeNames[FieldType::Float], "Float");
    EXPECT_EQ(typeNames[FieldType::Vector3], "Vector3");
}

// =============================================================================
// Type Safety Tests
// =============================================================================

TEST_F(ECSFieldTypeTest, RequiresExplicitCast) {
    // Verify it's an enum class (scoped enum)
    EXPECT_TRUE(std::is_enum_v<FieldType>);

    // Verify it requires explicit cast to underlying type
    EXPECT_FALSE((std::is_convertible_v<FieldType, uint64_t>));
}

TEST_F(ECSFieldTypeTest, ExplicitCastToUint64Works) {
    auto value = static_cast<uint64_t>(FieldType::Int64);
    EXPECT_GE(value, 0u);
    EXPECT_LT(value, static_cast<uint64_t>(FieldType::_Count));
}

TEST_F(ECSFieldTypeTest, ExplicitCastFromUint64Works) {
    uint64_t value = 5;
    auto type = static_cast<FieldType>(value);
    EXPECT_EQ(type, FieldType::Int32);
}

// =============================================================================
// Validation Tests
// =============================================================================

TEST_F(ECSFieldTypeTest, IsValidType) {
    // Helper function to check if a value is a valid FieldType
    auto isValid = [](FieldType type) {
        return static_cast<uint64_t>(type) < static_cast<uint64_t>(FieldType::_Count);
    };

    EXPECT_TRUE(isValid(FieldType::Blank));
    EXPECT_TRUE(isValid(FieldType::Section));
    EXPECT_TRUE(isValid(FieldType::Bool));
    EXPECT_TRUE(isValid(FieldType::Int32));
    EXPECT_TRUE(isValid(FieldType::Float));
    EXPECT_TRUE(isValid(FieldType::Vector3));
    EXPECT_TRUE(isValid(FieldType::Vector4));
    EXPECT_FALSE(isValid(FieldType::_Count));
}

TEST_F(ECSFieldTypeTest, CountCanBeUsedForArraySize) {
    // _Count can be used to size arrays
    constexpr size_t arraySize = static_cast<size_t>(FieldType::_Count);
    int counters[arraySize] = {};

    EXPECT_EQ(arraySize, 15u);

    // Initialize array
    for (size_t i = 0; i < arraySize; ++i) {
        counters[i] = static_cast<int>(i);
    }

    EXPECT_EQ(counters[0], 0);
    EXPECT_EQ(counters[14], 14);
}

}  // namespace nexo::ecs
