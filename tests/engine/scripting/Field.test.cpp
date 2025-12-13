//// Field.test.cpp ///////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for scripting Field struct
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "scripting/native/ui/Field.hpp"

namespace nexo::scripting {

class ScriptingFieldTest : public ::testing::Test {};

// =============================================================================
// Type Traits Tests
// =============================================================================

TEST_F(ScriptingFieldTest, IsAggregate) {
    EXPECT_TRUE(std::is_aggregate_v<Field>);
}

TEST_F(ScriptingFieldTest, IsTriviallyCopyable) {
    EXPECT_TRUE(std::is_trivially_copyable_v<Field>);
}

TEST_F(ScriptingFieldTest, IsTriviallyDestructible) {
    EXPECT_TRUE(std::is_trivially_destructible_v<Field>);
}

TEST_F(ScriptingFieldTest, IsStandardLayout) {
    EXPECT_TRUE(std::is_standard_layout_v<Field>);
}

// =============================================================================
// Member Tests
// =============================================================================

TEST_F(ScriptingFieldTest, HasNameMember) {
    Field field{};
    field.name = reinterpret_cast<IntPtr>(0x12345678);
    EXPECT_EQ(field.name, reinterpret_cast<IntPtr>(0x12345678));
}

TEST_F(ScriptingFieldTest, HasTypeMember) {
    Field field{};
    field.type = FieldType::Int32;
    EXPECT_EQ(field.type, FieldType::Int32);
}

TEST_F(ScriptingFieldTest, HasSizeMember) {
    Field field{};
    field.size = 4;
    EXPECT_EQ(field.size, 4u);
}

TEST_F(ScriptingFieldTest, HasOffsetMember) {
    Field field{};
    field.offset = 16;
    EXPECT_EQ(field.offset, 16u);
}

// =============================================================================
// Initialization Tests
// =============================================================================

TEST_F(ScriptingFieldTest, AggregateInitialization) {
    char testName[] = "TestField";
    Field field{
        .name = static_cast<IntPtr>(testName),
        .type = FieldType::Float,
        .size = sizeof(float),
        .offset = 0
    };

    EXPECT_EQ(field.name, static_cast<IntPtr>(testName));
    EXPECT_EQ(field.type, FieldType::Float);
    EXPECT_EQ(field.size, sizeof(float));
    EXPECT_EQ(field.offset, 0u);
}

TEST_F(ScriptingFieldTest, BraceInitialization) {
    char name[] = "test";
    Field field{static_cast<IntPtr>(name), FieldType::Bool, 1, 8};

    EXPECT_EQ(field.type, FieldType::Bool);
    EXPECT_EQ(field.size, 1u);
    EXPECT_EQ(field.offset, 8u);
}

// =============================================================================
// Copy Tests
// =============================================================================

TEST_F(ScriptingFieldTest, CopyConstruction) {
    char name[] = "Original";
    Field original{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Double,
        .size = sizeof(double),
        .offset = 32
    };

    Field copy = original;

    EXPECT_EQ(copy.name, original.name);
    EXPECT_EQ(copy.type, original.type);
    EXPECT_EQ(copy.size, original.size);
    EXPECT_EQ(copy.offset, original.offset);
}

TEST_F(ScriptingFieldTest, CopyAssignment) {
    char name[] = "Original";
    Field original{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Vector3,
        .size = 12,
        .offset = 0
    };

    Field copy{};
    copy = original;

    EXPECT_EQ(copy.name, original.name);
    EXPECT_EQ(copy.type, original.type);
    EXPECT_EQ(copy.size, original.size);
    EXPECT_EQ(copy.offset, original.offset);
}

// =============================================================================
// Type-specific Tests
// =============================================================================

TEST_F(ScriptingFieldTest, FieldWithBlankType) {
    Field field{
        .name = nullptr,
        .type = FieldType::Blank,
        .size = 0,
        .offset = 0
    };

    EXPECT_EQ(field.type, FieldType::Blank);
    EXPECT_EQ(field.size, 0u);
}

TEST_F(ScriptingFieldTest, FieldWithSectionType) {
    char sectionName[] = "Transform Section";
    Field field{
        .name = static_cast<IntPtr>(sectionName),
        .type = FieldType::Section,
        .size = 0,
        .offset = 0
    };

    EXPECT_EQ(field.type, FieldType::Section);
}

TEST_F(ScriptingFieldTest, Int8Field) {
    char name[] = "int8Field";
    Field field{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Int8,
        .size = sizeof(int8_t),
        .offset = 0
    };

    EXPECT_EQ(field.size, 1u);
}

TEST_F(ScriptingFieldTest, Int64Field) {
    char name[] = "int64Field";
    Field field{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Int64,
        .size = sizeof(int64_t),
        .offset = 8
    };

    EXPECT_EQ(field.size, 8u);
}

TEST_F(ScriptingFieldTest, Vector3Field) {
    char name[] = "position";
    Field field{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Vector3,
        .size = 12,  // 3 floats
        .offset = 0
    };

    EXPECT_EQ(field.type, FieldType::Vector3);
    EXPECT_EQ(field.size, 12u);
}

TEST_F(ScriptingFieldTest, Vector4Field) {
    char name[] = "color";
    Field field{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Vector4,
        .size = 16,  // 4 floats
        .offset = 12
    };

    EXPECT_EQ(field.type, FieldType::Vector4);
    EXPECT_EQ(field.size, 16u);
}

// =============================================================================
// Offset Tests
// =============================================================================

TEST_F(ScriptingFieldTest, ConsecutiveFieldOffsets) {
    // Simulate a component with multiple fields
    char x[] = "x";
    char y[] = "y";
    char z[] = "z";

    Field field1{
        .name = static_cast<IntPtr>(x),
        .type = FieldType::Float,
        .size = 4,
        .offset = 0
    };

    Field field2{
        .name = static_cast<IntPtr>(y),
        .type = FieldType::Float,
        .size = 4,
        .offset = 4
    };

    Field field3{
        .name = static_cast<IntPtr>(z),
        .type = FieldType::Float,
        .size = 4,
        .offset = 8
    };

    EXPECT_EQ(field1.offset + field1.size, field2.offset);
    EXPECT_EQ(field2.offset + field2.size, field3.offset);
}

TEST_F(ScriptingFieldTest, LargeOffset) {
    char name[] = "farField";
    Field field{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Double,
        .size = 8,
        .offset = 1024
    };

    EXPECT_EQ(field.offset, 1024u);
}

// =============================================================================
// Array of Fields Tests
// =============================================================================

TEST_F(ScriptingFieldTest, ArrayOfFields) {
    char x[] = "x";
    char y[] = "y";
    char z[] = "z";

    Field fields[3] = {
        {static_cast<IntPtr>(x), FieldType::Float, 4, 0},
        {static_cast<IntPtr>(y), FieldType::Float, 4, 4},
        {static_cast<IntPtr>(z), FieldType::Float, 4, 8}
    };

    EXPECT_EQ(fields[0].type, FieldType::Float);
    EXPECT_EQ(fields[1].type, FieldType::Float);
    EXPECT_EQ(fields[2].type, FieldType::Float);
    EXPECT_EQ(fields[2].offset, 8u);
}

// =============================================================================
// Edge Case Tests - Null and Zero Values
// =============================================================================

TEST_F(ScriptingFieldTest, NullNamePointer) {
    Field field{
        .name = nullptr,
        .type = FieldType::Int32,
        .size = 4,
        .offset = 0
    };

    EXPECT_EQ(field.name, nullptr);
    EXPECT_EQ(field.type, FieldType::Int32);
}

TEST_F(ScriptingFieldTest, ZeroSize) {
    char name[] = "emptyField";
    Field field{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Blank,
        .size = 0,
        .offset = 0
    };

    EXPECT_EQ(field.size, 0u);
}

TEST_F(ScriptingFieldTest, ZeroOffset) {
    char name[] = "firstField";
    Field field{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Double,
        .size = 8,
        .offset = 0
    };

    EXPECT_EQ(field.offset, 0u);
}

TEST_F(ScriptingFieldTest, AllZeroValues) {
    Field field{
        .name = nullptr,
        .type = FieldType::Blank,
        .size = 0,
        .offset = 0
    };

    EXPECT_EQ(field.name, nullptr);
    EXPECT_EQ(field.type, FieldType::Blank);
    EXPECT_EQ(field.size, 0u);
    EXPECT_EQ(field.offset, 0u);
}

// =============================================================================
// Edge Case Tests - Maximum Values
// =============================================================================

TEST_F(ScriptingFieldTest, MaximumSize) {
    char name[] = "hugeField";
    UInt64 maxSize = std::numeric_limits<UInt64>::max();
    Field field{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Int64,
        .size = maxSize,
        .offset = 0
    };

    EXPECT_EQ(field.size, maxSize);
}

TEST_F(ScriptingFieldTest, MaximumOffset) {
    char name[] = "farAwayField";
    UInt64 maxOffset = std::numeric_limits<UInt64>::max();
    Field field{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Int32,
        .size = 4,
        .offset = maxOffset
    };

    EXPECT_EQ(field.offset, maxOffset);
}

TEST_F(ScriptingFieldTest, VeryLargeOffset) {
    char name[] = "distantField";
    UInt64 largeOffset = 0xFFFFFFFFFFFFull; // Near max
    Field field{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Float,
        .size = 4,
        .offset = largeOffset
    };

    EXPECT_EQ(field.offset, largeOffset);
}

// =============================================================================
// Edge Case Tests - All Field Types
// =============================================================================

TEST_F(ScriptingFieldTest, AllPrimitiveTypes) {
    std::vector<std::pair<FieldType, UInt64>> typesSizes = {
        {FieldType::Bool, 1},
        {FieldType::Int8, 1},
        {FieldType::Int16, 2},
        {FieldType::Int32, 4},
        {FieldType::Int64, 8},
        {FieldType::UInt8, 1},
        {FieldType::UInt16, 2},
        {FieldType::UInt32, 4},
        {FieldType::UInt64, 8},
        {FieldType::Float, 4},
        {FieldType::Double, 8}
    };

    for (const auto& [type, expectedSize] : typesSizes) {
        Field field{
            .name = nullptr,
            .type = type,
            .size = expectedSize,
            .offset = 0
        };

        EXPECT_EQ(field.type, type);
        EXPECT_EQ(field.size, expectedSize);
    }
}

TEST_F(ScriptingFieldTest, AllIntegerTypes) {
    char name[] = "intField";

    // Test all signed integer types
    Field int8Field{static_cast<IntPtr>(name), FieldType::Int8, 1, 0};
    Field int16Field{static_cast<IntPtr>(name), FieldType::Int16, 2, 0};
    Field int32Field{static_cast<IntPtr>(name), FieldType::Int32, 4, 0};
    Field int64Field{static_cast<IntPtr>(name), FieldType::Int64, 8, 0};

    EXPECT_EQ(int8Field.size, 1u);
    EXPECT_EQ(int16Field.size, 2u);
    EXPECT_EQ(int32Field.size, 4u);
    EXPECT_EQ(int64Field.size, 8u);

    // Test all unsigned integer types
    Field uint8Field{static_cast<IntPtr>(name), FieldType::UInt8, 1, 0};
    Field uint16Field{static_cast<IntPtr>(name), FieldType::UInt16, 2, 0};
    Field uint32Field{static_cast<IntPtr>(name), FieldType::UInt32, 4, 0};
    Field uint64Field{static_cast<IntPtr>(name), FieldType::UInt64, 8, 0};

    EXPECT_EQ(uint8Field.size, 1u);
    EXPECT_EQ(uint16Field.size, 2u);
    EXPECT_EQ(uint32Field.size, 4u);
    EXPECT_EQ(uint64Field.size, 8u);
}

// =============================================================================
// Edge Case Tests - Alignment and Padding
// =============================================================================

TEST_F(ScriptingFieldTest, UnalignedOffset) {
    // Test that fields can have unaligned offsets (if needed)
    char name[] = "unaligned";
    Field field{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Int64,
        .size = 8,
        .offset = 3  // Unaligned for 8-byte type
    };

    EXPECT_EQ(field.offset, 3u);
}

TEST_F(ScriptingFieldTest, AlignedOffsets) {
    // Test properly aligned offsets for different types
    char name[] = "aligned";

    Field field8{static_cast<IntPtr>(name), FieldType::Int64, 8, 8};
    EXPECT_EQ(field8.offset % 8, 0u);

    Field field4{static_cast<IntPtr>(name), FieldType::Int32, 4, 4};
    EXPECT_EQ(field4.offset % 4, 0u);

    Field field2{static_cast<IntPtr>(name), FieldType::Int16, 2, 2};
    EXPECT_EQ(field2.offset % 2, 0u);
}

TEST_F(ScriptingFieldTest, PaddedStructure) {
    // Simulate a padded structure with gaps between fields
    char x[] = "x";
    char y[] = "y";
    char z[] = "z";

    Field field1{static_cast<IntPtr>(x), FieldType::Int8, 1, 0};
    // 3 bytes padding
    Field field2{static_cast<IntPtr>(y), FieldType::Int32, 4, 4};
    // No padding
    Field field3{static_cast<IntPtr>(z), FieldType::Int64, 8, 8};

    EXPECT_EQ(field1.offset, 0u);
    EXPECT_EQ(field2.offset, 4u);
    EXPECT_EQ(field3.offset, 8u);
}

// =============================================================================
// Edge Case Tests - Complex Structures
// =============================================================================

TEST_F(ScriptingFieldTest, NestedStructureSimulation) {
    // Simulate fields that might represent a nested structure
    char posX[] = "position.x";
    char posY[] = "position.y";
    char posZ[] = "position.z";

    Field xField{static_cast<IntPtr>(posX), FieldType::Float, 4, 0};
    Field yField{static_cast<IntPtr>(posY), FieldType::Float, 4, 4};
    Field zField{static_cast<IntPtr>(posZ), FieldType::Float, 4, 8};

    EXPECT_EQ(xField.offset, 0u);
    EXPECT_EQ(yField.offset, 4u);
    EXPECT_EQ(zField.offset, 8u);

    // Total size would be 12 bytes (Vector3)
    UInt64 totalSize = zField.offset + zField.size;
    EXPECT_EQ(totalSize, 12u);
}

TEST_F(ScriptingFieldTest, MixedTypesStructure) {
    char name1[] = "isActive";
    char name2[] = "count";
    char name3[] = "velocity";
    char name4[] = "color";

    Field boolField{static_cast<IntPtr>(name1), FieldType::Bool, 1, 0};
    Field intField{static_cast<IntPtr>(name2), FieldType::Int32, 4, 4};
    Field vec3Field{static_cast<IntPtr>(name3), FieldType::Vector3, 12, 8};
    Field vec4Field{static_cast<IntPtr>(name4), FieldType::Vector4, 16, 20};

    EXPECT_EQ(boolField.type, FieldType::Bool);
    EXPECT_EQ(intField.type, FieldType::Int32);
    EXPECT_EQ(vec3Field.type, FieldType::Vector3);
    EXPECT_EQ(vec4Field.type, FieldType::Vector4);

    UInt64 totalSize = vec4Field.offset + vec4Field.size;
    EXPECT_EQ(totalSize, 36u);
}

// =============================================================================
// Edge Case Tests - Default Initialization
// =============================================================================

TEST_F(ScriptingFieldTest, DefaultInitialization) {
    Field field{};

    // Default initialized field should have indeterminate values
    // but we can test that it doesn't crash
    EXPECT_NO_THROW({
        [[maybe_unused]] auto n = field.name;
        [[maybe_unused]] auto t = field.type;
        [[maybe_unused]] auto s = field.size;
        [[maybe_unused]] auto o = field.offset;
    });
}

TEST_F(ScriptingFieldTest, PartialInitialization) {
    // Initialize only some members
    Field field{.type = FieldType::Float};

    EXPECT_EQ(field.type, FieldType::Float);
    // Other members have default values (0/nullptr for POD types)
}

// =============================================================================
// Edge Case Tests - Vector of Fields
// =============================================================================

TEST_F(ScriptingFieldTest, VectorOfFields) {
    std::vector<Field> fields;

    char name1[] = "field1";
    char name2[] = "field2";

    fields.push_back({static_cast<IntPtr>(name1), FieldType::Int32, 4, 0});
    fields.push_back({static_cast<IntPtr>(name2), FieldType::Float, 4, 4});

    EXPECT_EQ(fields.size(), 2u);
    EXPECT_EQ(fields[0].type, FieldType::Int32);
    EXPECT_EQ(fields[1].type, FieldType::Float);
}

TEST_F(ScriptingFieldTest, LargeArrayOfFields) {
    constexpr size_t fieldCount = 100;
    std::vector<Field> fields;
    fields.reserve(fieldCount);

    for (size_t i = 0; i < fieldCount; ++i) {
        fields.push_back({
            nullptr,
            FieldType::Int32,
            4,
            static_cast<UInt64>(i * 4)
        });
    }

    EXPECT_EQ(fields.size(), fieldCount);
    EXPECT_EQ(fields[0].offset, 0u);
    EXPECT_EQ(fields[99].offset, 396u);
}

// =============================================================================
// Edge Case Tests - Memory Layout
// =============================================================================

TEST_F(ScriptingFieldTest, FieldSize) {
    // Verify Field struct size
    // Should be: IntPtr (8) + FieldType (8) + UInt64 (8) + UInt64 (8) = 32 bytes
    EXPECT_EQ(sizeof(Field), 32u);
}

TEST_F(ScriptingFieldTest, FieldAlignment) {
    // Verify Field struct alignment
    EXPECT_EQ(alignof(Field), 8u);
}

TEST_F(ScriptingFieldTest, MemberOffsets) {
    // Verify member offsets within the struct
    Field field{};

    auto baseAddr = reinterpret_cast<uintptr_t>(&field);
    auto nameAddr = reinterpret_cast<uintptr_t>(&field.name);
    auto typeAddr = reinterpret_cast<uintptr_t>(&field.type);
    auto sizeAddr = reinterpret_cast<uintptr_t>(&field.size);
    auto offsetAddr = reinterpret_cast<uintptr_t>(&field.offset);

    EXPECT_EQ(nameAddr - baseAddr, 0u);
    EXPECT_EQ(typeAddr - baseAddr, 8u);
    EXPECT_EQ(sizeAddr - baseAddr, 16u);
    EXPECT_EQ(offsetAddr - baseAddr, 24u);
}

// =============================================================================
// Edge Case Tests - Comparison and Equality
// =============================================================================

TEST_F(ScriptingFieldTest, FieldEquality) {
    char name[] = "testField";

    Field field1{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Int32,
        .size = 4,
        .offset = 0
    };

    Field field2{
        .name = static_cast<IntPtr>(name),
        .type = FieldType::Int32,
        .size = 4,
        .offset = 0
    };

    // Manual comparison (no operator== defined)
    EXPECT_EQ(field1.name, field2.name);
    EXPECT_EQ(field1.type, field2.type);
    EXPECT_EQ(field1.size, field2.size);
    EXPECT_EQ(field1.offset, field2.offset);
}

TEST_F(ScriptingFieldTest, FieldInequality) {
    char name1[] = "field1";
    char name2[] = "field2";

    Field field1{static_cast<IntPtr>(name1), FieldType::Int32, 4, 0};
    Field field2{static_cast<IntPtr>(name2), FieldType::Float, 4, 4};

    EXPECT_NE(field1.name, field2.name);
    EXPECT_NE(field1.type, field2.type);
    EXPECT_NE(field1.offset, field2.offset);
}

// =============================================================================
// Edge Case Tests - Special Field Types
// =============================================================================

TEST_F(ScriptingFieldTest, MultipleBlankFields) {
    Field blank1{nullptr, FieldType::Blank, 0, 0};
    Field blank2{nullptr, FieldType::Blank, 0, 0};

    EXPECT_EQ(blank1.type, FieldType::Blank);
    EXPECT_EQ(blank2.type, FieldType::Blank);
}

TEST_F(ScriptingFieldTest, MultipleSectionFields) {
    char section1[] = "Section 1";
    char section2[] = "Section 2";

    Field sec1{static_cast<IntPtr>(section1), FieldType::Section, 0, 0};
    Field sec2{static_cast<IntPtr>(section2), FieldType::Section, 0, 0};

    EXPECT_EQ(sec1.type, FieldType::Section);
    EXPECT_EQ(sec2.type, FieldType::Section);
    EXPECT_NE(sec1.name, sec2.name);
}

// =============================================================================
// Edge Case Tests - Invalid Type Combinations
// =============================================================================

TEST_F(ScriptingFieldTest, InvalidFieldTypeValue) {
    // Test with invalid FieldType value
    Field field{
        nullptr,
        static_cast<FieldType>(999),  // Invalid type
        0,
        0
    };

    // Should still compile and work, just has invalid type
    auto typeVal = static_cast<uint64_t>(field.type);
    EXPECT_EQ(typeVal, 999u);
}

TEST_F(ScriptingFieldTest, MismatchedSizeAndType) {
    // Float with wrong size
    char name[] = "wrongSize";
    Field field{
        static_cast<IntPtr>(name),
        FieldType::Float,
        100,  // Wrong size for float
        0
    };

    EXPECT_EQ(field.type, FieldType::Float);
    EXPECT_EQ(field.size, 100u);  // Still stored, even if wrong
}

}  // namespace nexo::scripting
