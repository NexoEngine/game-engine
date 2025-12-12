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

}  // namespace nexo::scripting
