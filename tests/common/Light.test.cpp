//// Light.test.cpp ///////////////////////////////////////////////////////////////
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
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Test file for the light utils functions
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <cmath>
#include <utility>
#include "math/Light.hpp"
#include "../utils/comparison.hpp"

TEST(ComputeAttenuation, BelowMinimumDistance) {
    // For a distance below the smallest value in the table (7.0f),
    // the function should return the values from the first row.
    float distance = 5.0f;
    auto [linear, quadratic] = nexo::math::computeAttenuationFromDistance(distance);
    // Expected from first row: linear = 0.70, quadratic = 1.8
    EXPECT_FLOAT_NEAR(linear, 0.70f, 0.01f);
    EXPECT_FLOAT_NEAR(quadratic, 1.8f, 0.01f);
}

TEST(ComputeAttenuation, AboveMaximumDistance) {
    // For a distance above the largest table value (3250.0f),
    // the function should return the last row's values.
    float distance = 5000.0f;
    auto [linear, quadratic] = nexo::math::computeAttenuationFromDistance(distance);
    // Expected from last row: linear = 0.0014, quadratic = 0.000007
    EXPECT_FLOAT_NEAR(linear, 0.0014f, 0.0001f);
    EXPECT_FLOAT_NEAR(quadratic, 0.000007f, 0.000001f);
}

TEST(ComputeAttenuation, ExactTableEntry) {
    // For a distance exactly equal to a table entry,
    // the function should return that row's values.
    // For example, distance = 32.0f (4th row): linear = 0.14, quadratic = 0.07.
    float distance = 32.0f;
    auto [linear, quadratic] = nexo::math::computeAttenuationFromDistance(distance);
    EXPECT_FLOAT_NEAR(linear, 0.14f, 0.001f);
    EXPECT_FLOAT_NEAR(quadratic, 0.07f, 0.0001f);
}

TEST(ComputeAttenuation, InterpolateBetweenRow0AndRow1) {
    // For a distance between the first two rows.
    // Row0: at 7.0f, linear=0.70, quadratic=1.8
    // Row1: at 13.0f, linear=0.35, quadratic=0.44
    // Use distance = 10.0f: t = (10-7)/(13-7) = 0.5.
    // Expected: linear = 0.70 + 0.5*(0.35-0.70) = 0.525
    //           quadratic = 1.8 + 0.5*(0.44-1.8) = 1.12
    float distance = 10.0f;
    float t = (distance - 7.0f) / (13.0f - 7.0f); // Should be 0.5
    float expectedLinear = 0.70f + t * (0.35f - 0.70f);
    float expectedQuadratic = 1.8f + t * (0.44f - 1.8f);

    auto [linear, quadratic] = nexo::math::computeAttenuationFromDistance(distance);
    EXPECT_FLOAT_NEAR(linear, expectedLinear, 0.001f);
    EXPECT_FLOAT_NEAR(quadratic, expectedQuadratic, 0.0001f);
}

TEST(ComputeAttenuation, InterpolateBetweenRow3AndRow4) {
    // For a distance between row 3 (32.0f: linear=0.14, quadratic=0.07)
    // and row 4 (50.0f: linear=0.09, quadratic=0.032).
    // Use distance = 40.0f:
    //   t = (40-32) / (50-32) = 8/18 ≈ 0.4444.
    float distance = 40.0f;
    float t = (distance - 32.0f) / (50.0f - 32.0f);
    float expectedLinear = 0.14f + t * (0.09f - 0.14f);
    float expectedQuadratic = 0.07f + t * (0.032f - 0.07f);

    auto [linear, quadratic] = nexo::math::computeAttenuationFromDistance(distance);
    EXPECT_FLOAT_NEAR(linear, expectedLinear, 0.001f);
    EXPECT_FLOAT_NEAR(quadratic, expectedQuadratic, 0.0001f);
}
