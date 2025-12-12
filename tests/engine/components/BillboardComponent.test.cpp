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

// =============================================================================
// Field Assignment Tests
// =============================================================================

class BillboardComponentFieldAssignmentTest : public ::testing::Test {};

TEST_F(BillboardComponentFieldAssignmentTest, AssignTypeMultipleTimes) {
    BillboardComponent billboard;

    billboard.type = BillboardType::FULL;
    EXPECT_EQ(billboard.type, BillboardType::FULL);

    billboard.type = BillboardType::AXIS_Y;
    EXPECT_EQ(billboard.type, BillboardType::AXIS_Y);

    billboard.type = BillboardType::AXIS_CUSTOM;
    EXPECT_EQ(billboard.type, BillboardType::AXIS_CUSTOM);

    billboard.type = BillboardType::FULL;
    EXPECT_EQ(billboard.type, BillboardType::FULL);
}

TEST_F(BillboardComponentFieldAssignmentTest, AssignAxisVectorComponents) {
    BillboardComponent billboard;

    billboard.axis.x = 0.5f;
    EXPECT_FLOAT_EQ(billboard.axis.x, 0.5f);

    billboard.axis.y = 0.3f;
    EXPECT_FLOAT_EQ(billboard.axis.y, 0.3f);

    billboard.axis.z = 0.8f;
    EXPECT_FLOAT_EQ(billboard.axis.z, 0.8f);
}

TEST_F(BillboardComponentFieldAssignmentTest, AssignAxisUsingConstructor) {
    BillboardComponent billboard;
    billboard.axis = glm::vec3(0.2f, 0.4f, 0.6f);

    EXPECT_FLOAT_EQ(billboard.axis.x, 0.2f);
    EXPECT_FLOAT_EQ(billboard.axis.y, 0.4f);
    EXPECT_FLOAT_EQ(billboard.axis.z, 0.6f);
}

TEST_F(BillboardComponentFieldAssignmentTest, AssignVaoToNullptr) {
    BillboardComponent billboard;
    billboard.vao = nullptr;
    EXPECT_EQ(billboard.vao, nullptr);
}

TEST_F(BillboardComponentFieldAssignmentTest, ReassignVaoToNullptr) {
    BillboardComponent billboard;
    billboard.vao = nullptr;
    EXPECT_EQ(billboard.vao, nullptr);

    billboard.vao = nullptr;
    EXPECT_EQ(billboard.vao, nullptr);
}

TEST_F(BillboardComponentFieldAssignmentTest, AssignAllFieldsSimultaneously) {
    BillboardComponent billboard;
    billboard.type = BillboardType::AXIS_CUSTOM;
    billboard.axis = glm::vec3(0.577f, 0.577f, 0.577f);
    billboard.vao = nullptr;

    EXPECT_EQ(billboard.type, BillboardType::AXIS_CUSTOM);
    EXPECT_FLOAT_EQ(billboard.axis.x, 0.577f);
    EXPECT_FLOAT_EQ(billboard.axis.y, 0.577f);
    EXPECT_FLOAT_EQ(billboard.axis.z, 0.577f);
    EXPECT_EQ(billboard.vao, nullptr);
}

TEST_F(BillboardComponentFieldAssignmentTest, AxisAssignmentDoesNotAffectType) {
    BillboardComponent billboard;
    billboard.type = BillboardType::FULL;

    billboard.axis = glm::vec3(1.0f, 0.0f, 0.0f);

    EXPECT_EQ(billboard.type, BillboardType::FULL);
}

TEST_F(BillboardComponentFieldAssignmentTest, TypeAssignmentDoesNotAffectAxis) {
    BillboardComponent billboard;
    billboard.axis = glm::vec3(0.5f, 0.5f, 0.0f);

    billboard.type = BillboardType::AXIS_CUSTOM;

    EXPECT_FLOAT_EQ(billboard.axis.x, 0.5f);
    EXPECT_FLOAT_EQ(billboard.axis.y, 0.5f);
    EXPECT_FLOAT_EQ(billboard.axis.z, 0.0f);
}

// =============================================================================
// Edge Cases and Boundary Tests
// =============================================================================

class BillboardComponentEdgeCasesTest : public ::testing::Test {};

TEST_F(BillboardComponentEdgeCasesTest, ZeroAxisVector) {
    BillboardComponent billboard;
    billboard.axis = glm::vec3(0.0f, 0.0f, 0.0f);

    EXPECT_FLOAT_EQ(billboard.axis.x, 0.0f);
    EXPECT_FLOAT_EQ(billboard.axis.y, 0.0f);
    EXPECT_FLOAT_EQ(billboard.axis.z, 0.0f);
}

TEST_F(BillboardComponentEdgeCasesTest, VerySmallAxisValues) {
    BillboardComponent billboard;
    billboard.axis = glm::vec3(0.0001f, 0.0002f, 0.0003f);

    EXPECT_FLOAT_EQ(billboard.axis.x, 0.0001f);
    EXPECT_FLOAT_EQ(billboard.axis.y, 0.0002f);
    EXPECT_FLOAT_EQ(billboard.axis.z, 0.0003f);
}

TEST_F(BillboardComponentEdgeCasesTest, VeryLargeAxisValues) {
    BillboardComponent billboard;
    billboard.axis = glm::vec3(1000.0f, 2000.0f, 3000.0f);

    EXPECT_FLOAT_EQ(billboard.axis.x, 1000.0f);
    EXPECT_FLOAT_EQ(billboard.axis.y, 2000.0f);
    EXPECT_FLOAT_EQ(billboard.axis.z, 3000.0f);
}

TEST_F(BillboardComponentEdgeCasesTest, AllNegativeAxisValues) {
    BillboardComponent billboard;
    billboard.axis = glm::vec3(-1.0f, -2.0f, -3.0f);

    EXPECT_FLOAT_EQ(billboard.axis.x, -1.0f);
    EXPECT_FLOAT_EQ(billboard.axis.y, -2.0f);
    EXPECT_FLOAT_EQ(billboard.axis.z, -3.0f);
}

TEST_F(BillboardComponentEdgeCasesTest, MixedSignAxisValues) {
    BillboardComponent billboard;
    billboard.axis = glm::vec3(-1.0f, 2.0f, -3.0f);

    EXPECT_FLOAT_EQ(billboard.axis.x, -1.0f);
    EXPECT_FLOAT_EQ(billboard.axis.y, 2.0f);
    EXPECT_FLOAT_EQ(billboard.axis.z, -3.0f);
}

TEST_F(BillboardComponentEdgeCasesTest, FloatingPointPrecisionAxis) {
    BillboardComponent billboard;
    billboard.axis = glm::vec3(0.123456789f, 0.987654321f, 0.555555555f);

    EXPECT_NEAR(billboard.axis.x, 0.123456789f, 0.000001f);
    EXPECT_NEAR(billboard.axis.y, 0.987654321f, 0.000001f);
    EXPECT_NEAR(billboard.axis.z, 0.555555555f, 0.000001f);
}

TEST_F(BillboardComponentEdgeCasesTest, AxisNearZero) {
    BillboardComponent billboard;
    billboard.axis = glm::vec3(1e-10f, 1e-10f, 1e-10f);

    EXPECT_FLOAT_EQ(billboard.axis.x, 1e-10f);
    EXPECT_FLOAT_EQ(billboard.axis.y, 1e-10f);
    EXPECT_FLOAT_EQ(billboard.axis.z, 1e-10f);
}

TEST_F(BillboardComponentEdgeCasesTest, NormalizedUnitAxisX) {
    BillboardComponent billboard;
    billboard.axis = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));

    float length = glm::length(billboard.axis);
    EXPECT_NEAR(length, 1.0f, 0.0001f);
    EXPECT_NEAR(billboard.axis.x, 1.0f, 0.0001f);
}

TEST_F(BillboardComponentEdgeCasesTest, NormalizedUnitAxisY) {
    BillboardComponent billboard;
    billboard.axis = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

    float length = glm::length(billboard.axis);
    EXPECT_NEAR(length, 1.0f, 0.0001f);
    EXPECT_NEAR(billboard.axis.y, 1.0f, 0.0001f);
}

TEST_F(BillboardComponentEdgeCasesTest, NormalizedUnitAxisZ) {
    BillboardComponent billboard;
    billboard.axis = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));

    float length = glm::length(billboard.axis);
    EXPECT_NEAR(length, 1.0f, 0.0001f);
    EXPECT_NEAR(billboard.axis.z, 1.0f, 0.0001f);
}

TEST_F(BillboardComponentEdgeCasesTest, NormalizedArbitraryAxis) {
    BillboardComponent billboard;
    glm::vec3 arbitrary(3.0f, 4.0f, 12.0f);
    billboard.axis = glm::normalize(arbitrary);

    float length = glm::length(billboard.axis);
    EXPECT_NEAR(length, 1.0f, 0.0001f);
}

TEST_F(BillboardComponentEdgeCasesTest, UnnormalizedLongVector) {
    BillboardComponent billboard;
    billboard.axis = glm::vec3(100.0f, 200.0f, 300.0f);

    EXPECT_FLOAT_EQ(billboard.axis.x, 100.0f);
    EXPECT_FLOAT_EQ(billboard.axis.y, 200.0f);
    EXPECT_FLOAT_EQ(billboard.axis.z, 300.0f);
}

// =============================================================================
// Move Semantics Tests
// =============================================================================

class BillboardComponentMoveTest : public ::testing::Test {};

TEST_F(BillboardComponentMoveTest, MoveConstructorTransfersType) {
    BillboardComponent original;
    original.type = BillboardType::AXIS_CUSTOM;

    BillboardComponent moved(std::move(original));
    EXPECT_EQ(moved.type, BillboardType::AXIS_CUSTOM);
}

TEST_F(BillboardComponentMoveTest, MoveConstructorTransfersAxis) {
    BillboardComponent original;
    original.axis = glm::vec3(0.7f, 0.8f, 0.9f);

    BillboardComponent moved(std::move(original));
    EXPECT_FLOAT_EQ(moved.axis.x, 0.7f);
    EXPECT_FLOAT_EQ(moved.axis.y, 0.8f);
    EXPECT_FLOAT_EQ(moved.axis.z, 0.9f);
}

TEST_F(BillboardComponentMoveTest, MoveConstructorTransfersVao) {
    BillboardComponent original;
    original.vao = nullptr;

    BillboardComponent moved(std::move(original));
    EXPECT_EQ(moved.vao, nullptr);
}

TEST_F(BillboardComponentMoveTest, MoveAssignmentOperatorTransfersType) {
    BillboardComponent original;
    original.type = BillboardType::AXIS_Y;

    BillboardComponent other;
    other = std::move(original);
    EXPECT_EQ(other.type, BillboardType::AXIS_Y);
}

TEST_F(BillboardComponentMoveTest, MoveAssignmentOperatorTransfersAxis) {
    BillboardComponent original;
    original.axis = glm::vec3(0.1f, 0.2f, 0.3f);

    BillboardComponent other;
    other = std::move(original);
    EXPECT_FLOAT_EQ(other.axis.x, 0.1f);
    EXPECT_FLOAT_EQ(other.axis.y, 0.2f);
    EXPECT_FLOAT_EQ(other.axis.z, 0.3f);
}

TEST_F(BillboardComponentMoveTest, MoveAssignmentOperatorTransfersVao) {
    BillboardComponent original;
    original.vao = nullptr;

    BillboardComponent other;
    other = std::move(original);
    EXPECT_EQ(other.vao, nullptr);
}

TEST_F(BillboardComponentMoveTest, MoveAssignmentOverwritesExistingData) {
    BillboardComponent original;
    original.type = BillboardType::AXIS_CUSTOM;
    original.axis = glm::vec3(1.0f, 2.0f, 3.0f);

    BillboardComponent other;
    other.type = BillboardType::FULL;
    other.axis = glm::vec3(0.0f, 0.0f, 0.0f);

    other = std::move(original);

    EXPECT_EQ(other.type, BillboardType::AXIS_CUSTOM);
    EXPECT_FLOAT_EQ(other.axis.x, 1.0f);
    EXPECT_FLOAT_EQ(other.axis.y, 2.0f);
    EXPECT_FLOAT_EQ(other.axis.z, 3.0f);
}

// =============================================================================
// Complex State Tests
// =============================================================================

class BillboardComponentComplexStateTest : public ::testing::Test {};

TEST_F(BillboardComponentComplexStateTest, FullBillboardCompleteSetup) {
    BillboardComponent billboard;
    billboard.type = BillboardType::FULL;
    billboard.axis = glm::vec3(0.0f, 1.0f, 0.0f);  // Axis ignored for FULL
    billboard.vao = nullptr;

    EXPECT_EQ(billboard.type, BillboardType::FULL);
    EXPECT_EQ(billboard.vao, nullptr);
}

TEST_F(BillboardComponentComplexStateTest, AxisYBillboardCompleteSetup) {
    BillboardComponent billboard;
    billboard.type = BillboardType::AXIS_Y;
    billboard.axis = glm::vec3(0.0f, 1.0f, 0.0f);
    billboard.vao = nullptr;

    EXPECT_EQ(billboard.type, BillboardType::AXIS_Y);
    EXPECT_FLOAT_EQ(billboard.axis.y, 1.0f);
    EXPECT_EQ(billboard.vao, nullptr);
}

TEST_F(BillboardComponentComplexStateTest, AxisCustomBillboardCompleteSetup) {
    BillboardComponent billboard;
    billboard.type = BillboardType::AXIS_CUSTOM;
    billboard.axis = glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f));
    billboard.vao = nullptr;

    EXPECT_EQ(billboard.type, BillboardType::AXIS_CUSTOM);
    float length = glm::length(billboard.axis);
    EXPECT_NEAR(length, 1.0f, 0.0001f);
    EXPECT_EQ(billboard.vao, nullptr);
}

TEST_F(BillboardComponentComplexStateTest, MultipleStateChanges) {
    BillboardComponent billboard;

    // State 1: FULL
    billboard.type = BillboardType::FULL;
    EXPECT_EQ(billboard.type, BillboardType::FULL);

    // State 2: AXIS_Y
    billboard.type = BillboardType::AXIS_Y;
    billboard.axis = glm::vec3(0.0f, 1.0f, 0.0f);
    EXPECT_EQ(billboard.type, BillboardType::AXIS_Y);
    EXPECT_FLOAT_EQ(billboard.axis.y, 1.0f);

    // State 3: AXIS_CUSTOM
    billboard.type = BillboardType::AXIS_CUSTOM;
    billboard.axis = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f));
    EXPECT_EQ(billboard.type, BillboardType::AXIS_CUSTOM);

    // State 4: Back to FULL
    billboard.type = BillboardType::FULL;
    EXPECT_EQ(billboard.type, BillboardType::FULL);
}

// =============================================================================
// Struct Size and Alignment Tests
// =============================================================================

class BillboardComponentStructTest : public ::testing::Test {};

TEST_F(BillboardComponentStructTest, StructIsNotEmpty) {
    EXPECT_GT(sizeof(BillboardComponent), 0u);
}

TEST_F(BillboardComponentStructTest, StructContainsExpectedMembers) {
    // This test ensures the struct has the expected layout
    BillboardComponent billboard;

    // Verify we can access all members
    billboard.type = BillboardType::FULL;
    billboard.axis = glm::vec3(0.0f);
    billboard.vao = nullptr;

    EXPECT_EQ(billboard.type, BillboardType::FULL);
    EXPECT_FLOAT_EQ(billboard.axis.x, 0.0f);
    EXPECT_EQ(billboard.vao, nullptr);
}

// =============================================================================
// Integration Tests
// =============================================================================

class BillboardComponentIntegrationTest : public ::testing::Test {};

TEST_F(BillboardComponentIntegrationTest, ParticleSystemSetup) {
    // Typical particle system billboard setup
    BillboardComponent particle;
    particle.type = BillboardType::FULL;
    particle.vao = nullptr;

    EXPECT_EQ(particle.type, BillboardType::FULL);
    EXPECT_EQ(particle.vao, nullptr);
}

TEST_F(BillboardComponentIntegrationTest, TreeBillboardSetup) {
    // Typical tree billboard setup (Y-axis aligned)
    BillboardComponent tree;
    tree.type = BillboardType::AXIS_Y;
    tree.axis = glm::vec3(0.0f, 1.0f, 0.0f);
    tree.vao = nullptr;

    EXPECT_EQ(tree.type, BillboardType::AXIS_Y);
    EXPECT_FLOAT_EQ(tree.axis.y, 1.0f);
}

TEST_F(BillboardComponentIntegrationTest, HealthBarBillboardSetup) {
    // Health bar that rotates around Y but not X/Z
    BillboardComponent healthBar;
    healthBar.type = BillboardType::AXIS_Y;
    healthBar.axis = glm::vec3(0.0f, 1.0f, 0.0f);
    healthBar.vao = nullptr;

    EXPECT_EQ(healthBar.type, BillboardType::AXIS_Y);
}

TEST_F(BillboardComponentIntegrationTest, SlopedSurfaceBillboard) {
    // Billboard constrained to a sloped surface normal
    BillboardComponent slope;
    slope.type = BillboardType::AXIS_CUSTOM;
    slope.axis = glm::normalize(glm::vec3(0.0f, 0.707f, 0.707f));  // 45 degree slope
    slope.vao = nullptr;

    EXPECT_EQ(slope.type, BillboardType::AXIS_CUSTOM);
    float length = glm::length(slope.axis);
    EXPECT_NEAR(length, 1.0f, 0.0001f);
}

TEST_F(BillboardComponentIntegrationTest, MultipleComponentsIndependent) {
    BillboardComponent billboard1;
    BillboardComponent billboard2;
    BillboardComponent billboard3;

    billboard1.type = BillboardType::FULL;
    billboard2.type = BillboardType::AXIS_Y;
    billboard3.type = BillboardType::AXIS_CUSTOM;
    billboard3.axis = glm::vec3(1.0f, 0.0f, 0.0f);

    EXPECT_EQ(billboard1.type, BillboardType::FULL);
    EXPECT_EQ(billboard2.type, BillboardType::AXIS_Y);
    EXPECT_EQ(billboard3.type, BillboardType::AXIS_CUSTOM);
    EXPECT_FLOAT_EQ(billboard3.axis.x, 1.0f);
}

}  // namespace nexo::components
