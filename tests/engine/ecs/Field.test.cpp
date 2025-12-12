//// Field.test.cpp ///////////////////////////////////////////////////////////
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
//  Description: Comprehensive test file for ECS Field struct
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ecs/TypeErasedComponent/Field.hpp"
#include <string>
#include <vector>

namespace nexo::ecs {

class ECSFieldTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// =============================================================================
// Type Traits Tests
// =============================================================================

TEST_F(ECSFieldTest, IsAggregate) {
    EXPECT_TRUE(std::is_aggregate_v<Field>);
}

TEST_F(ECSFieldTest, IsStandardLayout) {
    EXPECT_TRUE(std::is_standard_layout_v<Field>);
}

TEST_F(ECSFieldTest, HasExpectedSize) {
    // Field contains: std::string (24 bytes) + FieldType (8 bytes) + 2 uint64_t (16 bytes)
    // Minimum size is 48 bytes (may be larger due to alignment/padding)
    EXPECT_GE(sizeof(Field), 48u);
}

// =============================================================================
// Default Construction Tests
// =============================================================================

TEST_F(ECSFieldTest, DefaultConstruction) {
    Field field{};

    EXPECT_TRUE(field.name.empty());
    // Note: type, size, and offset are uninitialized in default construction
}

TEST_F(ECSFieldTest, DefaultConstructionWithInitializer) {
    Field field = {};

    EXPECT_TRUE(field.name.empty());
    EXPECT_EQ(field.type, FieldType::Blank);
    EXPECT_EQ(field.size, 0u);
    EXPECT_EQ(field.offset, 0u);
}

// =============================================================================
// Member Access Tests
// =============================================================================

TEST_F(ECSFieldTest, HasNameMember) {
    Field field{};
    field.name = "testField";
    EXPECT_EQ(field.name, "testField");
}

TEST_F(ECSFieldTest, HasTypeMember) {
    Field field{};
    field.type = FieldType::Int32;
    EXPECT_EQ(field.type, FieldType::Int32);
}

TEST_F(ECSFieldTest, HasSizeMember) {
    Field field{};
    field.size = 4;
    EXPECT_EQ(field.size, 4u);
}

TEST_F(ECSFieldTest, HasOffsetMember) {
    Field field{};
    field.offset = 16;
    EXPECT_EQ(field.offset, 16u);
}

// =============================================================================
// Initialization Tests
// =============================================================================

TEST_F(ECSFieldTest, DesignatedInitialization) {
    Field field{
        .name = "position",
        .type = FieldType::Float,
        .size = sizeof(float),
        .offset = 0
    };

    EXPECT_EQ(field.name, "position");
    EXPECT_EQ(field.type, FieldType::Float);
    EXPECT_EQ(field.size, sizeof(float));
    EXPECT_EQ(field.offset, 0u);
}

TEST_F(ECSFieldTest, AggregateInitialization) {
    Field field{"velocity", FieldType::Vector3, 12, 4};

    EXPECT_EQ(field.name, "velocity");
    EXPECT_EQ(field.type, FieldType::Vector3);
    EXPECT_EQ(field.size, 12u);
    EXPECT_EQ(field.offset, 4u);
}

TEST_F(ECSFieldTest, InitializationWithEmptyName) {
    Field field{"", FieldType::Bool, 1, 0};

    EXPECT_TRUE(field.name.empty());
    EXPECT_EQ(field.type, FieldType::Bool);
}

TEST_F(ECSFieldTest, InitializationWithLongName) {
    std::string longName(1000, 'x');
    Field field{longName, FieldType::Double, 8, 0};

    EXPECT_EQ(field.name.size(), 1000u);
    EXPECT_EQ(field.name, longName);
}

// =============================================================================
// Copy Construction Tests
// =============================================================================

TEST_F(ECSFieldTest, CopyConstruction) {
    Field original{
        .name = "health",
        .type = FieldType::Int32,
        .size = 4,
        .offset = 0
    };

    Field copy = original;

    EXPECT_EQ(copy.name, original.name);
    EXPECT_EQ(copy.type, original.type);
    EXPECT_EQ(copy.size, original.size);
    EXPECT_EQ(copy.offset, original.offset);
}

TEST_F(ECSFieldTest, CopyConstructionIndependence) {
    Field original{"damage", FieldType::Float, 4, 8};
    Field copy = original;

    // Modify copy
    copy.name = "armor";
    copy.type = FieldType::UInt32;
    copy.size = 4;
    copy.offset = 12;

    // Original should be unchanged
    EXPECT_EQ(original.name, "damage");
    EXPECT_EQ(original.type, FieldType::Float);
    EXPECT_EQ(original.size, 4u);
    EXPECT_EQ(original.offset, 8u);
}

// =============================================================================
// Copy Assignment Tests
// =============================================================================

TEST_F(ECSFieldTest, CopyAssignment) {
    Field original{
        .name = "rotation",
        .type = FieldType::Vector4,
        .size = 16,
        .offset = 0
    };

    Field copy{};
    copy = original;

    EXPECT_EQ(copy.name, original.name);
    EXPECT_EQ(copy.type, original.type);
    EXPECT_EQ(copy.size, original.size);
    EXPECT_EQ(copy.offset, original.offset);
}

TEST_F(ECSFieldTest, CopyAssignmentChaining) {
    Field field1{"x", FieldType::Float, 4, 0};
    Field field2{};
    Field field3{};

    field3 = field2 = field1;

    EXPECT_EQ(field3.name, "x");
    EXPECT_EQ(field3.type, FieldType::Float);
    EXPECT_EQ(field2.name, "x");
    EXPECT_EQ(field2.type, FieldType::Float);
}

// =============================================================================
// Move Semantics Tests
// =============================================================================

TEST_F(ECSFieldTest, MoveConstruction) {
    Field original{"transform", FieldType::Vector3, 12, 0};
    Field moved = std::move(original);

    EXPECT_EQ(moved.name, "transform");
    EXPECT_EQ(moved.type, FieldType::Vector3);
    EXPECT_EQ(moved.size, 12u);
    EXPECT_EQ(moved.offset, 0u);
}

TEST_F(ECSFieldTest, MoveAssignment) {
    Field original{"scale", FieldType::Vector3, 12, 16};
    Field moved{};
    moved = std::move(original);

    EXPECT_EQ(moved.name, "scale");
    EXPECT_EQ(moved.type, FieldType::Vector3);
    EXPECT_EQ(moved.size, 12u);
    EXPECT_EQ(moved.offset, 16u);
}

// =============================================================================
// Field with Different Types Tests
// =============================================================================

TEST_F(ECSFieldTest, FieldWithBlankType) {
    Field field{
        .name = "",
        .type = FieldType::Blank,
        .size = 0,
        .offset = 0
    };

    EXPECT_EQ(field.type, FieldType::Blank);
    EXPECT_EQ(field.size, 0u);
}

TEST_F(ECSFieldTest, FieldWithSectionType) {
    Field field{
        .name = "Transform Section",
        .type = FieldType::Section,
        .size = 0,
        .offset = 0
    };

    EXPECT_EQ(field.name, "Transform Section");
    EXPECT_EQ(field.type, FieldType::Section);
}

TEST_F(ECSFieldTest, BoolField) {
    Field field{"isActive", FieldType::Bool, sizeof(bool), 0};

    EXPECT_EQ(field.type, FieldType::Bool);
    EXPECT_EQ(field.size, sizeof(bool));
}

TEST_F(ECSFieldTest, Int8Field) {
    Field field{"byte_value", FieldType::Int8, sizeof(int8_t), 0};

    EXPECT_EQ(field.type, FieldType::Int8);
    EXPECT_EQ(field.size, 1u);
}

TEST_F(ECSFieldTest, Int16Field) {
    Field field{"short_value", FieldType::Int16, sizeof(int16_t), 0};

    EXPECT_EQ(field.type, FieldType::Int16);
    EXPECT_EQ(field.size, 2u);
}

TEST_F(ECSFieldTest, Int32Field) {
    Field field{"int_value", FieldType::Int32, sizeof(int32_t), 0};

    EXPECT_EQ(field.type, FieldType::Int32);
    EXPECT_EQ(field.size, 4u);
}

TEST_F(ECSFieldTest, Int64Field) {
    Field field{"long_value", FieldType::Int64, sizeof(int64_t), 0};

    EXPECT_EQ(field.type, FieldType::Int64);
    EXPECT_EQ(field.size, 8u);
}

TEST_F(ECSFieldTest, UInt8Field) {
    Field field{"ubyte_value", FieldType::UInt8, sizeof(uint8_t), 0};

    EXPECT_EQ(field.type, FieldType::UInt8);
    EXPECT_EQ(field.size, 1u);
}

TEST_F(ECSFieldTest, UInt16Field) {
    Field field{"ushort_value", FieldType::UInt16, sizeof(uint16_t), 0};

    EXPECT_EQ(field.type, FieldType::UInt16);
    EXPECT_EQ(field.size, 2u);
}

TEST_F(ECSFieldTest, UInt32Field) {
    Field field{"uint_value", FieldType::UInt32, sizeof(uint32_t), 0};

    EXPECT_EQ(field.type, FieldType::UInt32);
    EXPECT_EQ(field.size, 4u);
}

TEST_F(ECSFieldTest, UInt64Field) {
    Field field{"ulong_value", FieldType::UInt64, sizeof(uint64_t), 0};

    EXPECT_EQ(field.type, FieldType::UInt64);
    EXPECT_EQ(field.size, 8u);
}

TEST_F(ECSFieldTest, FloatField) {
    Field field{"float_value", FieldType::Float, sizeof(float), 0};

    EXPECT_EQ(field.type, FieldType::Float);
    EXPECT_EQ(field.size, 4u);
}

TEST_F(ECSFieldTest, DoubleField) {
    Field field{"double_value", FieldType::Double, sizeof(double), 0};

    EXPECT_EQ(field.type, FieldType::Double);
    EXPECT_EQ(field.size, 8u);
}

TEST_F(ECSFieldTest, Vector3Field) {
    Field field{"position", FieldType::Vector3, 12, 0};

    EXPECT_EQ(field.name, "position");
    EXPECT_EQ(field.type, FieldType::Vector3);
    EXPECT_EQ(field.size, 12u);
}

TEST_F(ECSFieldTest, Vector4Field) {
    Field field{"color", FieldType::Vector4, 16, 0};

    EXPECT_EQ(field.name, "color");
    EXPECT_EQ(field.type, FieldType::Vector4);
    EXPECT_EQ(field.size, 16u);
}

// =============================================================================
// Offset Tests
// =============================================================================

TEST_F(ECSFieldTest, ZeroOffset) {
    Field field{"first_field", FieldType::Int32, 4, 0};
    EXPECT_EQ(field.offset, 0u);
}

TEST_F(ECSFieldTest, NonZeroOffset) {
    Field field{"second_field", FieldType::Float, 4, 8};
    EXPECT_EQ(field.offset, 8u);
}

TEST_F(ECSFieldTest, LargeOffset) {
    Field field{"far_field", FieldType::Double, 8, 1024};
    EXPECT_EQ(field.offset, 1024u);
}

TEST_F(ECSFieldTest, ConsecutiveFieldOffsets) {
    Field field1{"x", FieldType::Float, 4, 0};
    Field field2{"y", FieldType::Float, 4, 4};
    Field field3{"z", FieldType::Float, 4, 8};

    EXPECT_EQ(field1.offset + field1.size, field2.offset);
    EXPECT_EQ(field2.offset + field2.size, field3.offset);
}

TEST_F(ECSFieldTest, OffsetWithPadding) {
    // Simulate alignment padding
    Field field1{"byte", FieldType::Int8, 1, 0};
    Field field2{"aligned_int", FieldType::Int32, 4, 4};  // Offset 4 due to alignment

    EXPECT_LT(field1.offset + field1.size, field2.offset);
}

// =============================================================================
// Size Tests
// =============================================================================

TEST_F(ECSFieldTest, ZeroSize) {
    Field field{"section", FieldType::Section, 0, 0};
    EXPECT_EQ(field.size, 0u);
}

TEST_F(ECSFieldTest, SmallSize) {
    Field field{"flag", FieldType::Bool, 1, 0};
    EXPECT_EQ(field.size, 1u);
}

TEST_F(ECSFieldTest, MediumSize) {
    Field field{"value", FieldType::Int32, 4, 0};
    EXPECT_EQ(field.size, 4u);
}

TEST_F(ECSFieldTest, LargeSize) {
    Field field{"matrix", FieldType::Vector4, 64, 0};
    EXPECT_EQ(field.size, 64u);
}

// =============================================================================
// Array and Container Tests
// =============================================================================

TEST_F(ECSFieldTest, ArrayOfFields) {
    Field fields[3] = {
        {"x", FieldType::Float, 4, 0},
        {"y", FieldType::Float, 4, 4},
        {"z", FieldType::Float, 4, 8}
    };

    EXPECT_EQ(fields[0].name, "x");
    EXPECT_EQ(fields[1].name, "y");
    EXPECT_EQ(fields[2].name, "z");
    EXPECT_EQ(fields[0].offset, 0u);
    EXPECT_EQ(fields[1].offset, 4u);
    EXPECT_EQ(fields[2].offset, 8u);
}

TEST_F(ECSFieldTest, VectorOfFields) {
    std::vector<Field> fields = {
        {"health", FieldType::Int32, 4, 0},
        {"damage", FieldType::Float, 4, 4},
        {"position", FieldType::Vector3, 12, 8}
    };

    EXPECT_EQ(fields.size(), 3u);
    EXPECT_EQ(fields[0].name, "health");
    EXPECT_EQ(fields[1].name, "damage");
    EXPECT_EQ(fields[2].name, "position");
}

TEST_F(ECSFieldTest, VectorPushBack) {
    std::vector<Field> fields;
    fields.push_back({"speed", FieldType::Float, 4, 0});
    fields.push_back({"direction", FieldType::Vector3, 12, 4});

    EXPECT_EQ(fields.size(), 2u);
    EXPECT_EQ(fields[0].name, "speed");
    EXPECT_EQ(fields[1].name, "direction");
}

TEST_F(ECSFieldTest, VectorEmplaceBack) {
    std::vector<Field> fields;
    fields.emplace_back("mass", FieldType::Double, 8, 0);
    fields.emplace_back("velocity", FieldType::Vector3, 12, 8);

    EXPECT_EQ(fields.size(), 2u);
    EXPECT_EQ(fields[0].name, "mass");
    EXPECT_EQ(fields[1].name, "velocity");
}

// =============================================================================
// String Handling Tests
// =============================================================================

TEST_F(ECSFieldTest, EmptyNameString) {
    Field field{"", FieldType::Int32, 4, 0};
    EXPECT_TRUE(field.name.empty());
    EXPECT_EQ(field.name.length(), 0u);
}

TEST_F(ECSFieldTest, ShortNameString) {
    Field field{"x", FieldType::Float, 4, 0};
    EXPECT_EQ(field.name, "x");
    EXPECT_EQ(field.name.length(), 1u);
}

TEST_F(ECSFieldTest, LongNameString) {
    std::string longName(500, 'a');
    Field field{longName, FieldType::Double, 8, 0};
    EXPECT_EQ(field.name.length(), 500u);
    EXPECT_EQ(field.name, longName);
}

TEST_F(ECSFieldTest, NameWithSpaces) {
    Field field{"my field name", FieldType::Int32, 4, 0};
    EXPECT_EQ(field.name, "my field name");
}

TEST_F(ECSFieldTest, NameWithSpecialCharacters) {
    Field field{"field_name_123!@#", FieldType::Float, 4, 0};
    EXPECT_EQ(field.name, "field_name_123!@#");
}

TEST_F(ECSFieldTest, NameWithUnicode) {
    Field field{"フィールド", FieldType::Int32, 4, 0};
    EXPECT_EQ(field.name, "フィールド");
}

// =============================================================================
// Complex Scenarios Tests
// =============================================================================

TEST_F(ECSFieldTest, ComponentFieldLayout) {
    // Simulate a Transform component with multiple fields
    std::vector<Field> transformFields = {
        {"position", FieldType::Vector3, 12, 0},
        {"rotation", FieldType::Vector4, 16, 12},
        {"scale", FieldType::Vector3, 12, 28}
    };

    EXPECT_EQ(transformFields.size(), 3u);

    // Verify layout
    EXPECT_EQ(transformFields[0].offset, 0u);
    EXPECT_EQ(transformFields[1].offset, 12u);
    EXPECT_EQ(transformFields[2].offset, 28u);

    // Total size
    uint64_t totalSize = transformFields[2].offset + transformFields[2].size;
    EXPECT_EQ(totalSize, 40u);
}

TEST_F(ECSFieldTest, ComponentWithSection) {
    std::vector<Field> fields = {
        {"--- Transform ---", FieldType::Section, 0, 0},
        {"position", FieldType::Vector3, 12, 0},
        {"rotation", FieldType::Vector3, 12, 12},
        {"--- Physics ---", FieldType::Section, 0, 24},
        {"velocity", FieldType::Vector3, 12, 24},
        {"mass", FieldType::Float, 4, 36}
    };

    EXPECT_EQ(fields.size(), 6u);
    EXPECT_EQ(fields[0].type, FieldType::Section);
    EXPECT_EQ(fields[3].type, FieldType::Section);
}

TEST_F(ECSFieldTest, MixedTypeComponent) {
    std::vector<Field> fields = {
        {"id", FieldType::UInt64, 8, 0},
        {"isActive", FieldType::Bool, 1, 8},
        {"level", FieldType::Int16, 2, 10},
        {"health", FieldType::Float, 4, 12},
        {"position", FieldType::Vector3, 12, 16}
    };

    EXPECT_EQ(fields.size(), 5u);

    // Verify each field type
    EXPECT_EQ(fields[0].type, FieldType::UInt64);
    EXPECT_EQ(fields[1].type, FieldType::Bool);
    EXPECT_EQ(fields[2].type, FieldType::Int16);
    EXPECT_EQ(fields[3].type, FieldType::Float);
    EXPECT_EQ(fields[4].type, FieldType::Vector3);
}

}  // namespace nexo::ecs
