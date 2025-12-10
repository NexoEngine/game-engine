//// BillboardComponent.test.cpp //////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for BillboardComponent and BillboardType enum
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "components/BillboardMesh.hpp"

namespace nexo::components {

// =============================================================================
// BillboardType Enum Tests
// =============================================================================

class BillboardTypeEnumTest : public ::testing::Test {};

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

}  // namespace nexo::components
