//// TypeErasedComponent.test.cpp //////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for TypeErasedComponent types (FieldType, Field,
//               ComponentDescription)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ecs/TypeErasedComponent/FieldType.hpp"
#include "ecs/TypeErasedComponent/Field.hpp"
#include "ecs/TypeErasedComponent/ComponentDescription.hpp"
#include <type_traits>
#include <vector>

namespace nexo::ecs {

// =============================================================================
// FieldType Tests
// =============================================================================

class FieldTypeTest : public ::testing::Test {};

TEST_F(FieldTypeTest, IsEnumClass) {
    static_assert(std::is_enum_v<FieldType>);
    // Enum class should not be implicitly convertible to int
    static_assert(!std::is_convertible_v<FieldType, int>);
    SUCCEED();
}

TEST_F(FieldTypeTest, HasBlankValue) {
    FieldType type = FieldType::Blank;
    EXPECT_EQ(type, FieldType::Blank);
}

TEST_F(FieldTypeTest, HasSectionValue) {
    FieldType type = FieldType::Section;
    EXPECT_EQ(type, FieldType::Section);
}

TEST_F(FieldTypeTest, HasBoolValue) {
    FieldType type = FieldType::Bool;
    EXPECT_EQ(type, FieldType::Bool);
}

TEST_F(FieldTypeTest, HasIntegerTypes) {
    EXPECT_NE(FieldType::Int8, FieldType::Blank);
    EXPECT_NE(FieldType::Int16, FieldType::Blank);
    EXPECT_NE(FieldType::Int32, FieldType::Blank);
    EXPECT_NE(FieldType::Int64, FieldType::Blank);
}

TEST_F(FieldTypeTest, HasUnsignedIntegerTypes) {
    EXPECT_NE(FieldType::UInt8, FieldType::Blank);
    EXPECT_NE(FieldType::UInt16, FieldType::Blank);
    EXPECT_NE(FieldType::UInt32, FieldType::Blank);
    EXPECT_NE(FieldType::UInt64, FieldType::Blank);
}

TEST_F(FieldTypeTest, HasFloatingPointTypes) {
    EXPECT_NE(FieldType::Float, FieldType::Blank);
    EXPECT_NE(FieldType::Double, FieldType::Blank);
}

TEST_F(FieldTypeTest, HasVectorWidgetTypes) {
    EXPECT_NE(FieldType::Vector3, FieldType::Blank);
    EXPECT_NE(FieldType::Vector4, FieldType::Blank);
}

TEST_F(FieldTypeTest, HasCountValue) {
    // _Count should be the last value for validation
    FieldType type = FieldType::_Count;
    EXPECT_EQ(type, FieldType::_Count);
}

TEST_F(FieldTypeTest, AllValuesAreDistinct) {
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

    // Check all values are distinct
    for (size_t i = 0; i < types.size(); ++i) {
        for (size_t j = i + 1; j < types.size(); ++j) {
            EXPECT_NE(types[i], types[j]) << "Types at index " << i << " and " << j << " are not distinct";
        }
    }
}

TEST_F(FieldTypeTest, CountReflectsNumberOfTypes) {
    // _Count should equal the number of other enum values
    auto count = static_cast<uint64_t>(FieldType::_Count);
    EXPECT_EQ(count, 15u);  // 15 types before _Count
}

TEST_F(FieldTypeTest, CanBeUsedInSwitch) {
    FieldType type = FieldType::Int32;
    bool handled = false;

    switch (type) {
        case FieldType::Blank:
        case FieldType::Section:
        case FieldType::Bool:
        case FieldType::Int8:
        case FieldType::Int16:
        case FieldType::Int32:
            handled = true;
            break;
        case FieldType::Int64:
        case FieldType::UInt8:
        case FieldType::UInt16:
        case FieldType::UInt32:
        case FieldType::UInt64:
        case FieldType::Float:
        case FieldType::Double:
        case FieldType::Vector3:
        case FieldType::Vector4:
        case FieldType::_Count:
            break;
    }

    EXPECT_TRUE(handled);
}

TEST_F(FieldTypeTest, UnderlyingTypeIsUint64) {
    static_assert(std::is_same_v<std::underlying_type_t<FieldType>, uint64_t>);
    SUCCEED();
}

TEST_F(FieldTypeTest, CanBeCopied) {
    FieldType original = FieldType::Vector3;
    FieldType copy = original;
    EXPECT_EQ(copy, FieldType::Vector3);
}

TEST_F(FieldTypeTest, CanBeAssigned) {
    FieldType type = FieldType::Bool;
    type = FieldType::Float;
    EXPECT_EQ(type, FieldType::Float);
}

// =============================================================================
// Field Tests
// =============================================================================

class FieldTest : public ::testing::Test {};

TEST_F(FieldTest, IsAggregate) {
    static_assert(std::is_aggregate_v<Field>);
    SUCCEED();
}

TEST_F(FieldTest, CanBeDefaultConstructed) {
    Field field{};
    EXPECT_TRUE(field.name.empty());
    EXPECT_EQ(field.type, FieldType::Blank);
    EXPECT_EQ(field.size, 0u);
    EXPECT_EQ(field.offset, 0u);
}

TEST_F(FieldTest, CanBeInitializedWithValues) {
    Field field{"position", FieldType::Vector3, 12, 0};

    EXPECT_EQ(field.name, "position");
    EXPECT_EQ(field.type, FieldType::Vector3);
    EXPECT_EQ(field.size, 12u);
    EXPECT_EQ(field.offset, 0u);
}

TEST_F(FieldTest, CanBeCopied) {
    Field original{"health", FieldType::Int32, 4, 16};
    Field copy = original;

    EXPECT_EQ(copy.name, "health");
    EXPECT_EQ(copy.type, FieldType::Int32);
    EXPECT_EQ(copy.size, 4u);
    EXPECT_EQ(copy.offset, 16u);
}

TEST_F(FieldTest, CanBeMoved) {
    Field original{"velocity", FieldType::Vector3, 12, 24};
    Field moved = std::move(original);

    EXPECT_EQ(moved.name, "velocity");
    EXPECT_EQ(moved.type, FieldType::Vector3);
    EXPECT_EQ(moved.size, 12u);
    EXPECT_EQ(moved.offset, 24u);
}

TEST_F(FieldTest, MembersCanBeModified) {
    Field field{};

    field.name = "rotation";
    field.type = FieldType::Vector4;
    field.size = 16;
    field.offset = 36;

    EXPECT_EQ(field.name, "rotation");
    EXPECT_EQ(field.type, FieldType::Vector4);
    EXPECT_EQ(field.size, 16u);
    EXPECT_EQ(field.offset, 36u);
}

TEST_F(FieldTest, CanBeStoredInVector) {
    std::vector<Field> fields;
    fields.push_back({"x", FieldType::Float, 4, 0});
    fields.push_back({"y", FieldType::Float, 4, 4});
    fields.push_back({"z", FieldType::Float, 4, 8});

    EXPECT_EQ(fields.size(), 3u);
    EXPECT_EQ(fields[0].name, "x");
    EXPECT_EQ(fields[1].name, "y");
    EXPECT_EQ(fields[2].name, "z");
}

TEST_F(FieldTest, EmplacesCorrectly) {
    std::vector<Field> fields;
    fields.emplace_back(Field{"enabled", FieldType::Bool, 1, 0});

    EXPECT_EQ(fields.size(), 1u);
    EXPECT_EQ(fields[0].name, "enabled");
    EXPECT_EQ(fields[0].type, FieldType::Bool);
}

TEST_F(FieldTest, SupportsAllFieldTypes) {
    std::vector<Field> fields = {
        {"blank", FieldType::Blank, 0, 0},
        {"section", FieldType::Section, 0, 0},
        {"flag", FieldType::Bool, 1, 0},
        {"tiny", FieldType::Int8, 1, 1},
        {"small", FieldType::Int16, 2, 2},
        {"medium", FieldType::Int32, 4, 4},
        {"large", FieldType::Int64, 8, 8},
        {"utiny", FieldType::UInt8, 1, 16},
        {"usmall", FieldType::UInt16, 2, 17},
        {"umedium", FieldType::UInt32, 4, 19},
        {"ularge", FieldType::UInt64, 8, 23},
        {"single", FieldType::Float, 4, 31},
        {"precision", FieldType::Double, 8, 35},
        {"pos", FieldType::Vector3, 12, 43},
        {"color", FieldType::Vector4, 16, 55}
    };

    EXPECT_EQ(fields.size(), 15u);
}

// =============================================================================
// ComponentDescription Tests
// =============================================================================

class ComponentDescriptionTest : public ::testing::Test {};

TEST_F(ComponentDescriptionTest, IsAggregate) {
    static_assert(std::is_aggregate_v<ComponentDescription>);
    SUCCEED();
}

TEST_F(ComponentDescriptionTest, CanBeDefaultConstructed) {
    ComponentDescription desc{};
    EXPECT_TRUE(desc.name.empty());
    EXPECT_TRUE(desc.fields.empty());
}

TEST_F(ComponentDescriptionTest, CanBeInitializedWithName) {
    ComponentDescription desc{"Transform", {}};
    EXPECT_EQ(desc.name, "Transform");
    EXPECT_TRUE(desc.fields.empty());
}

TEST_F(ComponentDescriptionTest, CanBeInitializedWithFields) {
    ComponentDescription desc{
        "Transform",
        {
            {"position", FieldType::Vector3, 12, 0},
            {"rotation", FieldType::Vector4, 16, 12},
            {"scale", FieldType::Vector3, 12, 28}
        }
    };

    EXPECT_EQ(desc.name, "Transform");
    EXPECT_EQ(desc.fields.size(), 3u);
    EXPECT_EQ(desc.fields[0].name, "position");
    EXPECT_EQ(desc.fields[1].name, "rotation");
    EXPECT_EQ(desc.fields[2].name, "scale");
}

TEST_F(ComponentDescriptionTest, CanBeCopied) {
    ComponentDescription original{
        "Light",
        {
            {"color", FieldType::Vector4, 16, 0},
            {"intensity", FieldType::Float, 4, 16}
        }
    };

    ComponentDescription copy = original;

    EXPECT_EQ(copy.name, "Light");
    EXPECT_EQ(copy.fields.size(), 2u);
    EXPECT_EQ(copy.fields[0].name, "color");
}

TEST_F(ComponentDescriptionTest, CanBeMoved) {
    ComponentDescription original{
        "Camera",
        {
            {"fov", FieldType::Float, 4, 0},
            {"near", FieldType::Float, 4, 4},
            {"far", FieldType::Float, 4, 8}
        }
    };

    ComponentDescription moved = std::move(original);

    EXPECT_EQ(moved.name, "Camera");
    EXPECT_EQ(moved.fields.size(), 3u);
}

TEST_F(ComponentDescriptionTest, FieldsCanBeAddedAfterConstruction) {
    ComponentDescription desc{"Collider", {}};

    desc.fields.push_back({"radius", FieldType::Float, 4, 0});
    desc.fields.push_back({"isTrigger", FieldType::Bool, 1, 4});

    EXPECT_EQ(desc.fields.size(), 2u);
}

TEST_F(ComponentDescriptionTest, FieldsCanBeCleared) {
    ComponentDescription desc{
        "Test",
        {
            {"a", FieldType::Int32, 4, 0},
            {"b", FieldType::Int32, 4, 4}
        }
    };

    desc.fields.clear();

    EXPECT_TRUE(desc.fields.empty());
    EXPECT_EQ(desc.name, "Test");  // Name should remain
}

TEST_F(ComponentDescriptionTest, CanBeStoredInVector) {
    std::vector<ComponentDescription> components;

    components.push_back({"Transform", {{"pos", FieldType::Vector3, 12, 0}}});
    components.push_back({"Velocity", {{"vel", FieldType::Vector3, 12, 0}}});

    EXPECT_EQ(components.size(), 2u);
    EXPECT_EQ(components[0].name, "Transform");
    EXPECT_EQ(components[1].name, "Velocity");
}

TEST_F(ComponentDescriptionTest, SupportsEmptyFieldsList) {
    ComponentDescription desc{"EmptyComponent", {}};

    EXPECT_EQ(desc.name, "EmptyComponent");
    EXPECT_EQ(desc.fields.size(), 0u);
}

TEST_F(ComponentDescriptionTest, SupportsManyFields) {
    ComponentDescription desc{"BigComponent", {}};

    // Add 100 fields
    for (int i = 0; i < 100; ++i) {
        desc.fields.push_back({
            "field" + std::to_string(i),
            FieldType::Float,
            4,
            static_cast<uint64_t>(i * 4)
        });
    }

    EXPECT_EQ(desc.fields.size(), 100u);
    EXPECT_EQ(desc.fields[50].name, "field50");
    EXPECT_EQ(desc.fields[50].offset, 200u);
}

// =============================================================================
// Cross-Type Integration Tests
// =============================================================================

class TypeErasedComponentIntegrationTest : public ::testing::Test {};

TEST_F(TypeErasedComponentIntegrationTest, FieldContainsFieldType) {
    Field field{"test", FieldType::Vector3, 12, 0};
    EXPECT_EQ(field.type, FieldType::Vector3);
}

TEST_F(TypeErasedComponentIntegrationTest, ComponentDescriptionContainsFields) {
    ComponentDescription desc{
        "TestComponent",
        {
            {"a", FieldType::Int32, 4, 0}
        }
    };

    EXPECT_FALSE(desc.fields.empty());
    EXPECT_EQ(desc.fields[0].type, FieldType::Int32);
}

TEST_F(TypeErasedComponentIntegrationTest, CanDescribeComplexComponent) {
    // Describe a realistic component like a material
    ComponentDescription material{
        "Material",
        {
            {"name", FieldType::Section, 0, 0},
            {"albedo", FieldType::Vector4, 16, 0},
            {"metallic", FieldType::Float, 4, 16},
            {"roughness", FieldType::Float, 4, 20},
            {"ao", FieldType::Float, 4, 24},
            {"emission", FieldType::Vector3, 12, 28},
            {"useNormalMap", FieldType::Bool, 1, 40}
        }
    };

    EXPECT_EQ(material.name, "Material");
    EXPECT_EQ(material.fields.size(), 7u);

    // Verify section field
    EXPECT_EQ(material.fields[0].type, FieldType::Section);

    // Verify last field offset makes sense
    EXPECT_EQ(material.fields[6].offset, 40u);
}

}  // namespace nexo::ecs
