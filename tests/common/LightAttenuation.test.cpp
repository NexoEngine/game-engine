//// LightAttenuation.test.cpp /////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for computeAttenuationFromDistance function
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "math/Light.hpp"

namespace nexo::math {

// =============================================================================
// AttenuationData Struct Tests
// =============================================================================

class AttenuationDataTest : public ::testing::Test {};

TEST_F(AttenuationDataTest, StructHasDistanceField) {
    AttenuationData data{7.0f, 1.0f, 0.7f, 1.8f};
    EXPECT_FLOAT_EQ(data.distance, 7.0f);
}

TEST_F(AttenuationDataTest, StructHasConstantField) {
    AttenuationData data{7.0f, 1.0f, 0.7f, 1.8f};
    EXPECT_FLOAT_EQ(data.constant, 1.0f);
}

TEST_F(AttenuationDataTest, StructHasLinearField) {
    AttenuationData data{7.0f, 1.0f, 0.7f, 1.8f};
    EXPECT_FLOAT_EQ(data.linear, 0.7f);
}

TEST_F(AttenuationDataTest, StructHasQuadraticField) {
    AttenuationData data{7.0f, 1.0f, 0.7f, 1.8f};
    EXPECT_FLOAT_EQ(data.quadratic, 1.8f);
}

// =============================================================================
// Attenuation Table Tests
// =============================================================================

class AttenuationTableTest : public ::testing::Test {};

TEST_F(AttenuationTableTest, TableHas12Entries) {
    EXPECT_EQ(s_attenuationCount, 12);
}

TEST_F(AttenuationTableTest, FirstEntryDistance) {
    EXPECT_FLOAT_EQ(s_attenuationTable[0].distance, 7.0f);
}

TEST_F(AttenuationTableTest, LastEntryDistance) {
    EXPECT_FLOAT_EQ(s_attenuationTable[11].distance, 3250.0f);
}

TEST_F(AttenuationTableTest, AllConstantsAreOne) {
    for (int i = 0; i < s_attenuationCount; i++) {
        EXPECT_FLOAT_EQ(s_attenuationTable[i].constant, 1.0f)
            << "Entry " << i << " has non-1.0 constant";
    }
}

TEST_F(AttenuationTableTest, DistancesAreIncreasing) {
    for (int i = 1; i < s_attenuationCount; i++) {
        EXPECT_GT(s_attenuationTable[i].distance, s_attenuationTable[i-1].distance)
            << "Distance not increasing at index " << i;
    }
}

TEST_F(AttenuationTableTest, LinearDecreases) {
    for (int i = 1; i < s_attenuationCount; i++) {
        EXPECT_LE(s_attenuationTable[i].linear, s_attenuationTable[i-1].linear)
            << "Linear not decreasing at index " << i;
    }
}

TEST_F(AttenuationTableTest, QuadraticDecreases) {
    for (int i = 1; i < s_attenuationCount; i++) {
        EXPECT_LE(s_attenuationTable[i].quadratic, s_attenuationTable[i-1].quadratic)
            << "Quadratic not decreasing at index " << i;
    }
}

// =============================================================================
// computeAttenuationFromDistance Tests - Exact Table Values
// =============================================================================

class AttenuationExactValuesTest : public ::testing::Test {};

TEST_F(AttenuationExactValuesTest, Distance7Returns0_70And1_8) {
    auto [linear, quadratic] = computeAttenuationFromDistance(7.0f);
    EXPECT_FLOAT_EQ(linear, 0.70f);
    EXPECT_FLOAT_EQ(quadratic, 1.8f);
}

TEST_F(AttenuationExactValuesTest, Distance13Returns0_35And0_44) {
    auto [linear, quadratic] = computeAttenuationFromDistance(13.0f);
    EXPECT_FLOAT_EQ(linear, 0.35f);
    EXPECT_FLOAT_EQ(quadratic, 0.44f);
}

TEST_F(AttenuationExactValuesTest, Distance100Returns0_045And0_0075) {
    auto [linear, quadratic] = computeAttenuationFromDistance(100.0f);
    EXPECT_FLOAT_EQ(linear, 0.045f);
    EXPECT_FLOAT_EQ(quadratic, 0.0075f);
}

TEST_F(AttenuationExactValuesTest, Distance3250Returns0_0014And0_000007) {
    auto [linear, quadratic] = computeAttenuationFromDistance(3250.0f);
    EXPECT_FLOAT_EQ(linear, 0.0014f);
    EXPECT_FLOAT_EQ(quadratic, 0.000007f);
}

// =============================================================================
// computeAttenuationFromDistance Tests - Clamping
// =============================================================================

class AttenuationClampingTest : public ::testing::Test {};

TEST_F(AttenuationClampingTest, DistanceBelowMinClampsToMin) {
    auto [linear, quadratic] = computeAttenuationFromDistance(0.0f);
    // Should return first table entry
    EXPECT_FLOAT_EQ(linear, 0.70f);
    EXPECT_FLOAT_EQ(quadratic, 1.8f);
}

TEST_F(AttenuationClampingTest, NegativeDistanceClampsToMin) {
    auto [linear, quadratic] = computeAttenuationFromDistance(-10.0f);
    EXPECT_FLOAT_EQ(linear, 0.70f);
    EXPECT_FLOAT_EQ(quadratic, 1.8f);
}

TEST_F(AttenuationClampingTest, VerySmallDistanceClampsToMin) {
    auto [linear, quadratic] = computeAttenuationFromDistance(1.0f);
    EXPECT_FLOAT_EQ(linear, 0.70f);
    EXPECT_FLOAT_EQ(quadratic, 1.8f);
}

TEST_F(AttenuationClampingTest, DistanceAboveMaxClampsToMax) {
    auto [linear, quadratic] = computeAttenuationFromDistance(10000.0f);
    // Should return last table entry
    EXPECT_FLOAT_EQ(linear, 0.0014f);
    EXPECT_FLOAT_EQ(quadratic, 0.000007f);
}

TEST_F(AttenuationClampingTest, VeryLargeDistanceClampsToMax) {
    auto [linear, quadratic] = computeAttenuationFromDistance(1000000.0f);
    EXPECT_FLOAT_EQ(linear, 0.0014f);
    EXPECT_FLOAT_EQ(quadratic, 0.000007f);
}

// =============================================================================
// computeAttenuationFromDistance Tests - Interpolation
// =============================================================================

class AttenuationInterpolationTest : public ::testing::Test {};

TEST_F(AttenuationInterpolationTest, MidpointBetween7And13) {
    // Distance 10 is between 7 and 13
    auto [linear, quadratic] = computeAttenuationFromDistance(10.0f);

    // linear should be between 0.70 and 0.35
    EXPECT_GT(linear, 0.35f);
    EXPECT_LT(linear, 0.70f);

    // quadratic should be between 1.8 and 0.44
    EXPECT_GT(quadratic, 0.44f);
    EXPECT_LT(quadratic, 1.8f);
}

TEST_F(AttenuationInterpolationTest, CloserToLowerBound) {
    // Distance 8 is closer to 7 than to 13
    auto [linear, quadratic] = computeAttenuationFromDistance(8.0f);

    // Should be closer to 0.70 than 0.35
    EXPECT_GT(linear, 0.5f);
}

TEST_F(AttenuationInterpolationTest, CloserToUpperBound) {
    // Distance 12 is closer to 13 than to 7
    auto [linear, quadratic] = computeAttenuationFromDistance(12.0f);

    // Should be closer to 0.35 than 0.70
    EXPECT_LT(linear, 0.5f);
}

TEST_F(AttenuationInterpolationTest, InterpolateBetween50And65) {
    // Distance 57.5 is midpoint between 50 and 65
    auto [linear, quadratic] = computeAttenuationFromDistance(57.5f);

    // linear should be between 0.09 and 0.07
    EXPECT_GT(linear, 0.07f);
    EXPECT_LT(linear, 0.09f);

    // quadratic should be between 0.032 and 0.017
    EXPECT_GT(quadratic, 0.017f);
    EXPECT_LT(quadratic, 0.032f);
}

// =============================================================================
// computeAttenuationFromDistance Tests - Return Type
// =============================================================================

class AttenuationReturnTypeTest : public ::testing::Test {};

TEST_F(AttenuationReturnTypeTest, ReturnsPair) {
    auto result = computeAttenuationFromDistance(50.0f);
    // Verify we can access as a pair
    EXPECT_FLOAT_EQ(result.first, 0.09f);
    EXPECT_FLOAT_EQ(result.second, 0.032f);
}

TEST_F(AttenuationReturnTypeTest, StructuredBindingWorks) {
    auto [linear, quadratic] = computeAttenuationFromDistance(50.0f);
    EXPECT_FLOAT_EQ(linear, 0.09f);
    EXPECT_FLOAT_EQ(quadratic, 0.032f);
}

// =============================================================================
// Edge Cases
// =============================================================================

class AttenuationEdgeCasesTest : public ::testing::Test {};

TEST_F(AttenuationEdgeCasesTest, ExactlyAtFirstEntry) {
    auto [linear, quadratic] = computeAttenuationFromDistance(7.0f);
    EXPECT_FLOAT_EQ(linear, 0.70f);
    EXPECT_FLOAT_EQ(quadratic, 1.8f);
}

TEST_F(AttenuationEdgeCasesTest, ExactlyAtLastEntry) {
    auto [linear, quadratic] = computeAttenuationFromDistance(3250.0f);
    EXPECT_FLOAT_EQ(linear, 0.0014f);
    EXPECT_FLOAT_EQ(quadratic, 0.000007f);
}

TEST_F(AttenuationEdgeCasesTest, JustAboveFirstEntry) {
    auto [linear, quadratic] = computeAttenuationFromDistance(7.001f);
    // Should interpolate slightly
    EXPECT_LE(linear, 0.70f);
    EXPECT_LE(quadratic, 1.8f);
}

TEST_F(AttenuationEdgeCasesTest, JustBelowLastEntry) {
    auto [linear, quadratic] = computeAttenuationFromDistance(3249.999f);
    // Should interpolate slightly
    EXPECT_GE(linear, 0.0014f);
    EXPECT_GE(quadratic, 0.000007f);
}

}  // namespace nexo::math
