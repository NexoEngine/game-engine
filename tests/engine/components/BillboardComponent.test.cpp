//// BillboardComponent.test.cpp //////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for BillboardComponent and BillboardType enum
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "components/BillboardMesh.hpp"
#include <glm/glm.hpp>
#include <set>
#include <vector>

namespace nexo::components {

// =============================================================================
// BillboardType Enum Tests
// =============================================================================

class BillboardTypeEnumTest : public ::testing::Test {};

TEST_F(BillboardTypeEnumTest, IsEnum) {
    EXPECT_TRUE(std::is_enum_v<BillboardType>);
}

TEST_F(BillboardTypeEnumTest, FullEnumValueExists) {
    EXPECT_EQ(static_cast<int>(BillboardType::FULL), 0);
}

TEST_F(BillboardTypeEnumTest, AxisYEnumValueExists) {
    EXPECT_EQ(static_cast<int>(BillboardType::AXIS_Y), 1);
}

TEST_F(BillboardTypeEnumTest, AxisCustomEnumValueExists) {
    EXPECT_EQ(static_cast<int>(BillboardType::AXIS_CUSTOM), 2);
}

TEST_F(BillboardTypeEnumTest, EnumValuesAreDistinct) {
    EXPECT_NE(BillboardType::FULL, BillboardType::AXIS_Y);
    EXPECT_NE(BillboardType::FULL, BillboardType::AXIS_CUSTOM);
    EXPECT_NE(BillboardType::AXIS_Y, BillboardType::AXIS_CUSTOM);
}

TEST_F(BillboardTypeEnumTest, AllValuesUnique) {
    std::vector<BillboardType> types = {
        BillboardType::FULL,
        BillboardType::AXIS_Y,
        BillboardType::AXIS_CUSTOM
    };

    std::set<int> uniqueValues;
    for (auto type : types) {
        auto [it, inserted] = uniqueValues.insert(static_cast<int>(type));
        EXPECT_TRUE(inserted) << "Duplicate value found for BillboardType";
    }

    EXPECT_EQ(uniqueValues.size(), types.size());
}

TEST_F(BillboardTypeEnumTest, CanBeStoredInVariable) {
    BillboardType type = BillboardType::AXIS_Y;
    EXPECT_EQ(type, BillboardType::AXIS_Y);
}

TEST_F(BillboardTypeEnumTest, CanBeCopied) {
    BillboardType original = BillboardType::AXIS_CUSTOM;
    BillboardType copy = original;
    EXPECT_EQ(original, copy);
}

TEST_F(BillboardTypeEnumTest, CanBeUsedInSwitch) {
    BillboardType type = BillboardType::FULL;
    bool matched = false;

    switch (type) {
        case BillboardType::FULL:
            matched = true;
            break;
        case BillboardType::AXIS_Y:
        case BillboardType::AXIS_CUSTOM:
            break;
    }

    EXPECT_TRUE(matched);
}

// =============================================================================
// BillboardComponent Default Values Tests
// =============================================================================

class BillboardComponentDefaultsTest : public ::testing::Test {};

TEST_F(BillboardComponentDefaultsTest, DefaultTypeIsFull) {
    BillboardComponent component;
    EXPECT_EQ(component.type, BillboardType::FULL);
}

TEST_F(BillboardComponentDefaultsTest, DefaultAxisIsYUp) {
    BillboardComponent component;
    EXPECT_FLOAT_EQ(component.axis.x, 0.0f);
    EXPECT_FLOAT_EQ(component.axis.y, 1.0f);
    EXPECT_FLOAT_EQ(component.axis.z, 0.0f);
}

TEST_F(BillboardComponentDefaultsTest, DefaultVaoIsNull) {
    BillboardComponent component;
    EXPECT_EQ(component.vao, nullptr);
}

// =============================================================================
// BillboardComponent Custom Values Tests
// =============================================================================

class BillboardComponentCustomTest : public ::testing::Test {};

TEST_F(BillboardComponentCustomTest, SetTypeToAxisY) {
    BillboardComponent component;
    component.type = BillboardType::AXIS_Y;
    EXPECT_EQ(component.type, BillboardType::AXIS_Y);
}

TEST_F(BillboardComponentCustomTest, SetTypeToAxisCustom) {
    BillboardComponent component;
    component.type = BillboardType::AXIS_CUSTOM;
    EXPECT_EQ(component.type, BillboardType::AXIS_CUSTOM);
}

TEST_F(BillboardComponentCustomTest, SetCustomAxisX) {
    BillboardComponent component;
    component.axis = {1.0f, 0.0f, 0.0f};
    EXPECT_FLOAT_EQ(component.axis.x, 1.0f);
    EXPECT_FLOAT_EQ(component.axis.y, 0.0f);
    EXPECT_FLOAT_EQ(component.axis.z, 0.0f);
}

TEST_F(BillboardComponentCustomTest, SetCustomAxisZ) {
    BillboardComponent component;
    component.axis = {0.0f, 0.0f, 1.0f};
    EXPECT_FLOAT_EQ(component.axis.x, 0.0f);
    EXPECT_FLOAT_EQ(component.axis.y, 0.0f);
    EXPECT_FLOAT_EQ(component.axis.z, 1.0f);
}

TEST_F(BillboardComponentCustomTest, SetCustomAxisDiagonal) {
    BillboardComponent component;
    // Normalized diagonal
    float v = 0.577350269f; // 1/sqrt(3)
    component.axis = {v, v, v};
    EXPECT_FLOAT_EQ(component.axis.x, v);
    EXPECT_FLOAT_EQ(component.axis.y, v);
    EXPECT_FLOAT_EQ(component.axis.z, v);
}

// =============================================================================
// BillboardComponent Copy Tests
// =============================================================================

class BillboardComponentCopyTest : public ::testing::Test {};

TEST_F(BillboardComponentCopyTest, CopyConstructorCopiesType) {
    BillboardComponent original;
    original.type = BillboardType::AXIS_CUSTOM;

    BillboardComponent copy = original;
    EXPECT_EQ(copy.type, BillboardType::AXIS_CUSTOM);
}

TEST_F(BillboardComponentCopyTest, CopyConstructorCopiesAxis) {
    BillboardComponent original;
    original.axis = {0.5f, 0.5f, 0.707f};

    BillboardComponent copy = original;
    EXPECT_FLOAT_EQ(copy.axis.x, 0.5f);
    EXPECT_FLOAT_EQ(copy.axis.y, 0.5f);
    EXPECT_FLOAT_EQ(copy.axis.z, 0.707f);
}

TEST_F(BillboardComponentCopyTest, AssignmentOperatorCopiesType) {
    BillboardComponent original;
    original.type = BillboardType::AXIS_Y;

    BillboardComponent other;
    other = original;
    EXPECT_EQ(other.type, BillboardType::AXIS_Y);
}

TEST_F(BillboardComponentCopyTest, AssignmentOperatorCopiesAxis) {
    BillboardComponent original;
    original.axis = {1.0f, 0.0f, 0.0f};

    BillboardComponent other;
    other = original;
    EXPECT_FLOAT_EQ(other.axis.x, 1.0f);
    EXPECT_FLOAT_EQ(other.axis.y, 0.0f);
    EXPECT_FLOAT_EQ(other.axis.z, 0.0f);
}

TEST_F(BillboardComponentCopyTest, IndependentCopies) {
    BillboardComponent original;
    original.type = BillboardType::FULL;
    original.axis = {1.0f, 0.0f, 0.0f};

    BillboardComponent copy = original;

    // Modify original
    original.type = BillboardType::AXIS_Y;
    original.axis = {0.0f, 1.0f, 0.0f};

    // Copy should remain unchanged
    EXPECT_EQ(copy.type, BillboardType::FULL);
    EXPECT_FLOAT_EQ(copy.axis.x, 1.0f);
    EXPECT_FLOAT_EQ(copy.axis.y, 0.0f);
}

// =============================================================================
// BillboardComponent Configuration Preset Tests
// =============================================================================

class BillboardComponentPresetsTest : public ::testing::Test {};

TEST_F(BillboardComponentPresetsTest, ParticleBillboard) {
    BillboardComponent billboard;
    billboard.type = BillboardType::FULL;
    // Particles always face camera completely

    EXPECT_EQ(billboard.type, BillboardType::FULL);
}

TEST_F(BillboardComponentPresetsTest, TreeBillboard) {
    BillboardComponent billboard;
    billboard.type = BillboardType::AXIS_Y;
    billboard.axis = glm::vec3(0.0f, 1.0f, 0.0f);
    // Trees rotate only around Y axis

    EXPECT_EQ(billboard.type, BillboardType::AXIS_Y);
    EXPECT_FLOAT_EQ(billboard.axis.y, 1.0f);
}

TEST_F(BillboardComponentPresetsTest, CustomAxisBillboard) {
    BillboardComponent billboard;
    billboard.type = BillboardType::AXIS_CUSTOM;
    billboard.axis = glm::normalize(glm::vec3(1.0f, 2.0f, 0.0f));

    EXPECT_EQ(billboard.type, BillboardType::AXIS_CUSTOM);
    // Verify axis is set (normalized values)
    EXPECT_GT(billboard.axis.x, 0.0f);
    EXPECT_GT(billboard.axis.y, 0.0f);
}

// =============================================================================
// Type Traits Tests
// =============================================================================

class BillboardComponentTraitsTest : public ::testing::Test {};

TEST_F(BillboardComponentTraitsTest, IsDefaultConstructible) {
    EXPECT_TRUE(std::is_default_constructible_v<BillboardComponent>);
}

TEST_F(BillboardComponentTraitsTest, IsCopyConstructible) {
    EXPECT_TRUE(std::is_copy_constructible_v<BillboardComponent>);
}

TEST_F(BillboardComponentTraitsTest, IsCopyAssignable) {
    EXPECT_TRUE(std::is_copy_assignable_v<BillboardComponent>);
}

TEST_F(BillboardComponentTraitsTest, IsMoveConstructible) {
    EXPECT_TRUE(std::is_move_constructible_v<BillboardComponent>);
}

TEST_F(BillboardComponentTraitsTest, IsMoveAssignable) {
    EXPECT_TRUE(std::is_move_assignable_v<BillboardComponent>);
}

// =============================================================================
// Additional Component Tests
// =============================================================================

class BillboardComponentAdditionalTest : public ::testing::Test {};

TEST_F(BillboardComponentAdditionalTest, ModifyTypeAndAxisIndependently) {
    BillboardComponent billboard;

    billboard.type = BillboardType::AXIS_CUSTOM;
    EXPECT_EQ(billboard.type, BillboardType::AXIS_CUSTOM);
    EXPECT_FLOAT_EQ(billboard.axis.y, 1.0f); // Axis should still be default

    billboard.axis = glm::vec3(1.0f, 0.0f, 0.0f);
    EXPECT_EQ(billboard.type, BillboardType::AXIS_CUSTOM); // Type unchanged
    EXPECT_FLOAT_EQ(billboard.axis.x, 1.0f);
}

TEST_F(BillboardComponentAdditionalTest, AxisCanBeNormalized) {
    BillboardComponent billboard;
    billboard.axis = glm::normalize(glm::vec3(1.0f, 2.0f, 3.0f));

    float length = glm::length(billboard.axis);
    EXPECT_NEAR(length, 1.0f, 0.0001f);
}

TEST_F(BillboardComponentAdditionalTest, SupportsNegativeAxisValues) {
    BillboardComponent billboard;
    billboard.axis = glm::vec3(-1.0f, 0.0f, 0.0f);

    EXPECT_FLOAT_EQ(billboard.axis.x, -1.0f);
    EXPECT_FLOAT_EQ(billboard.axis.y, 0.0f);
    EXPECT_FLOAT_EQ(billboard.axis.z, 0.0f);
}

TEST_F(BillboardComponentAdditionalTest, AxisZUp) {
    BillboardComponent billboard;
    billboard.type = BillboardType::AXIS_CUSTOM;
    billboard.axis = glm::vec3(0.0f, 0.0f, 1.0f);

    EXPECT_EQ(billboard.type, BillboardType::AXIS_CUSTOM);
    EXPECT_FLOAT_EQ(billboard.axis.z, 1.0f);
}

TEST_F(BillboardComponentAdditionalTest, AxisXUp) {
    BillboardComponent billboard;
    billboard.type = BillboardType::AXIS_CUSTOM;
    billboard.axis = glm::vec3(1.0f, 0.0f, 0.0f);

    EXPECT_EQ(billboard.type, BillboardType::AXIS_CUSTOM);
    EXPECT_FLOAT_EQ(billboard.axis.x, 1.0f);
}

}  // namespace nexo::components
