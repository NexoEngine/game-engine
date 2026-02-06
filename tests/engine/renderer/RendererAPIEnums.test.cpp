//// RendererAPIEnums.test.cpp /////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for RendererAPI enums (CulledFace, WindingOrder)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/RendererAPI.hpp"

namespace nexo::renderer {

// =============================================================================
// CulledFace Enum Tests
// =============================================================================

class CulledFaceEnumTest : public ::testing::Test {};

TEST_F(CulledFaceEnumTest, BackValueIs0) {
    EXPECT_EQ(static_cast<int>(CulledFace::BACK), 0);
}

TEST_F(CulledFaceEnumTest, FrontValueIs1) {
    EXPECT_EQ(static_cast<int>(CulledFace::FRONT), 1);
}

TEST_F(CulledFaceEnumTest, FrontAndBackValueIs2) {
    EXPECT_EQ(static_cast<int>(CulledFace::FRONT_AND_BACK), 2);
}

TEST_F(CulledFaceEnumTest, AllValuesDistinct) {
    EXPECT_NE(CulledFace::BACK, CulledFace::FRONT);
    EXPECT_NE(CulledFace::BACK, CulledFace::FRONT_AND_BACK);
    EXPECT_NE(CulledFace::FRONT, CulledFace::FRONT_AND_BACK);
}

TEST_F(CulledFaceEnumTest, CanBeUsedInSwitch) {
    CulledFace face = CulledFace::BACK;
    int result = 0;

    switch (face) {
        case CulledFace::BACK: result = 1; break;
        case CulledFace::FRONT: result = 2; break;
        case CulledFace::FRONT_AND_BACK: result = 3; break;
    }

    EXPECT_EQ(result, 1);
}

// =============================================================================
// WindingOrder Enum Tests
// =============================================================================

class WindingOrderEnumTest : public ::testing::Test {};

TEST_F(WindingOrderEnumTest, CWValueIs0) {
    EXPECT_EQ(static_cast<int>(WindingOrder::CW), 0);
}

TEST_F(WindingOrderEnumTest, CCWValueIs1) {
    EXPECT_EQ(static_cast<int>(WindingOrder::CCW), 1);
}

TEST_F(WindingOrderEnumTest, ValuesDistinct) {
    EXPECT_NE(WindingOrder::CW, WindingOrder::CCW);
}

TEST_F(WindingOrderEnumTest, CanBeUsedInSwitch) {
    WindingOrder order = WindingOrder::CCW;
    int result = 0;

    switch (order) {
        case WindingOrder::CW: result = 1; break;
        case WindingOrder::CCW: result = 2; break;
    }

    EXPECT_EQ(result, 2);
}

// =============================================================================
// Enum Assignment Tests
// =============================================================================

class EnumAssignmentTest : public ::testing::Test {};

TEST_F(EnumAssignmentTest, CulledFaceAssignment) {
    CulledFace face = CulledFace::BACK;
    EXPECT_EQ(face, CulledFace::BACK);

    face = CulledFace::FRONT;
    EXPECT_EQ(face, CulledFace::FRONT);

    face = CulledFace::FRONT_AND_BACK;
    EXPECT_EQ(face, CulledFace::FRONT_AND_BACK);
}

TEST_F(EnumAssignmentTest, WindingOrderAssignment) {
    WindingOrder order = WindingOrder::CW;
    EXPECT_EQ(order, WindingOrder::CW);

    order = WindingOrder::CCW;
    EXPECT_EQ(order, WindingOrder::CCW);
}

// =============================================================================
// Enum Comparison Tests
// =============================================================================

class EnumComparisonTest : public ::testing::Test {};

TEST_F(EnumComparisonTest, CulledFaceEquality) {
    CulledFace face1 = CulledFace::BACK;
    CulledFace face2 = CulledFace::BACK;
    EXPECT_TRUE(face1 == face2);
}

TEST_F(EnumComparisonTest, CulledFaceInequality) {
    CulledFace face1 = CulledFace::BACK;
    CulledFace face2 = CulledFace::FRONT;
    EXPECT_TRUE(face1 != face2);
}

TEST_F(EnumComparisonTest, WindingOrderEquality) {
    WindingOrder order1 = WindingOrder::CCW;
    WindingOrder order2 = WindingOrder::CCW;
    EXPECT_TRUE(order1 == order2);
}

TEST_F(EnumComparisonTest, WindingOrderInequality) {
    WindingOrder order1 = WindingOrder::CW;
    WindingOrder order2 = WindingOrder::CCW;
    EXPECT_TRUE(order1 != order2);
}

}  // namespace nexo::renderer
