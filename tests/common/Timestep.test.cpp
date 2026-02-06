//// Timestep.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for Timestep class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "Timestep.hpp"

namespace nexo {

class TimestepTest : public ::testing::Test {};

// =============================================================================
// Constructor Tests
// =============================================================================

TEST_F(TimestepTest, DefaultConstructorZeroTime) {
    Timestep ts;
    EXPECT_DOUBLE_EQ(ts.getSeconds(), 0.0);
}

TEST_F(TimestepTest, ConstructorWithPositiveTime) {
    Timestep ts(1.5);
    EXPECT_DOUBLE_EQ(ts.getSeconds(), 1.5);
}

TEST_F(TimestepTest, ConstructorWithZeroTime) {
    Timestep ts(0.0);
    EXPECT_DOUBLE_EQ(ts.getSeconds(), 0.0);
}

TEST_F(TimestepTest, ConstructorWithSmallTime) {
    Timestep ts(0.001);
    EXPECT_DOUBLE_EQ(ts.getSeconds(), 0.001);
}

TEST_F(TimestepTest, ConstructorWithLargeTime) {
    Timestep ts(3600.0);
    EXPECT_DOUBLE_EQ(ts.getSeconds(), 3600.0);
}

TEST_F(TimestepTest, ImplicitConversionFromDouble) {
    // explicit(false) allows implicit conversion
    Timestep ts = 2.5;
    EXPECT_DOUBLE_EQ(ts.getSeconds(), 2.5);
}

// =============================================================================
// getSeconds Tests
// =============================================================================

TEST_F(TimestepTest, GetSecondsReturnsCorrectValue) {
    Timestep ts(5.0);
    EXPECT_DOUBLE_EQ(ts.getSeconds(), 5.0);
}

TEST_F(TimestepTest, GetSecondsWithFractionalValue) {
    Timestep ts(0.016667);  // ~60 FPS frame time
    EXPECT_NEAR(ts.getSeconds(), 0.016667, 0.0000001);
}

// =============================================================================
// getMilliseconds Tests
// =============================================================================

TEST_F(TimestepTest, GetMillisecondsConvertsCorrectly) {
    Timestep ts(1.0);
    EXPECT_DOUBLE_EQ(ts.getMilliseconds(), 1000.0);
}

TEST_F(TimestepTest, GetMillisecondsWithZero) {
    Timestep ts(0.0);
    EXPECT_DOUBLE_EQ(ts.getMilliseconds(), 0.0);
}

TEST_F(TimestepTest, GetMillisecondsWithFractionalSeconds) {
    Timestep ts(0.5);
    EXPECT_DOUBLE_EQ(ts.getMilliseconds(), 500.0);
}

TEST_F(TimestepTest, GetMillisecondsSmallValue) {
    Timestep ts(0.001);
    EXPECT_DOUBLE_EQ(ts.getMilliseconds(), 1.0);
}

TEST_F(TimestepTest, GetMillisecondsTypicalFrameTime) {
    Timestep ts(0.016667);  // ~60 FPS
    EXPECT_NEAR(ts.getMilliseconds(), 16.667, 0.001);
}

TEST_F(TimestepTest, GetMilliseconds30FPS) {
    Timestep ts(0.033333);  // ~30 FPS
    EXPECT_NEAR(ts.getMilliseconds(), 33.333, 0.001);
}

// =============================================================================
// Explicit Float Conversion Tests
// =============================================================================

TEST_F(TimestepTest, ExplicitFloatConversion) {
    Timestep ts(2.5);
    float value = static_cast<float>(ts);
    EXPECT_FLOAT_EQ(value, 2.5f);
}

TEST_F(TimestepTest, ExplicitFloatConversionZero) {
    Timestep ts(0.0);
    float value = static_cast<float>(ts);
    EXPECT_FLOAT_EQ(value, 0.0f);
}

TEST_F(TimestepTest, ExplicitFloatConversionSmallValue) {
    Timestep ts(0.001);
    float value = static_cast<float>(ts);
    EXPECT_FLOAT_EQ(value, 0.001f);
}

// =============================================================================
// Explicit Double Conversion Tests
// =============================================================================

TEST_F(TimestepTest, ExplicitDoubleConversion) {
    Timestep ts(2.5);
    double value = static_cast<double>(ts);
    EXPECT_DOUBLE_EQ(value, 2.5);
}

TEST_F(TimestepTest, ExplicitDoubleConversionZero) {
    Timestep ts(0.0);
    double value = static_cast<double>(ts);
    EXPECT_DOUBLE_EQ(value, 0.0);
}

TEST_F(TimestepTest, ExplicitDoubleConversionPrecision) {
    Timestep ts(0.123456789);
    double value = static_cast<double>(ts);
    EXPECT_DOUBLE_EQ(value, 0.123456789);
}

// =============================================================================
// Precision Tests
// =============================================================================

TEST_F(TimestepTest, PrecisionMaintained) {
    double original = 0.0166666666666667;  // ~60 FPS with high precision
    Timestep ts(original);
    EXPECT_NEAR(ts.getSeconds(), original, 1e-15);
}

TEST_F(TimestepTest, MillisecondsPrecision) {
    Timestep ts(0.001234567);
    double ms = ts.getMilliseconds();
    EXPECT_NEAR(ms, 1.234567, 0.0000001);
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST_F(TimestepTest, VerySmallTimestep) {
    Timestep ts(0.0000001);  // 100 nanoseconds
    EXPECT_NEAR(ts.getSeconds(), 0.0000001, 1e-10);
    EXPECT_NEAR(ts.getMilliseconds(), 0.0001, 1e-7);
}

TEST_F(TimestepTest, OneHourTimestep) {
    Timestep ts(3600.0);
    EXPECT_DOUBLE_EQ(ts.getSeconds(), 3600.0);
    EXPECT_DOUBLE_EQ(ts.getMilliseconds(), 3600000.0);
}

// =============================================================================
// Typical Game Frame Times
// =============================================================================

TEST_F(TimestepTest, Frame60FPS) {
    Timestep ts(1.0 / 60.0);
    EXPECT_NEAR(ts.getMilliseconds(), 16.6667, 0.001);
}

TEST_F(TimestepTest, Frame120FPS) {
    Timestep ts(1.0 / 120.0);
    EXPECT_NEAR(ts.getMilliseconds(), 8.3333, 0.001);
}

TEST_F(TimestepTest, Frame30FPS) {
    Timestep ts(1.0 / 30.0);
    EXPECT_NEAR(ts.getMilliseconds(), 33.3333, 0.001);
}

TEST_F(TimestepTest, Frame144FPS) {
    Timestep ts(1.0 / 144.0);
    EXPECT_NEAR(ts.getMilliseconds(), 6.944, 0.001);
}

}  // namespace nexo
