//// WorldState.test.cpp //////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        13/12/2025
//  Description: Test file for WorldState struct
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "WorldState.hpp"
#include <limits>

namespace nexo {

// =============================================================================
// Default Values Tests
// =============================================================================

class WorldStateDefaultsTest : public ::testing::Test {};

TEST_F(WorldStateDefaultsTest, DefaultDeltaTimeIsZero) {
    WorldState state;
    EXPECT_DOUBLE_EQ(state.time.deltaTime, 0.0);
}

TEST_F(WorldStateDefaultsTest, DefaultTotalTimeIsZero) {
    WorldState state;
    EXPECT_DOUBLE_EQ(state.time.totalTime, 0.0);
}

TEST_F(WorldStateDefaultsTest, DefaultFrameCountIsZero) {
    WorldState state;
    EXPECT_EQ(state.stats.frameCount, 0);
}

TEST_F(WorldStateDefaultsTest, AllDefaultValuesAreZero) {
    WorldState state;
    EXPECT_DOUBLE_EQ(state.time.deltaTime, 0.0);
    EXPECT_DOUBLE_EQ(state.time.totalTime, 0.0);
    EXPECT_EQ(state.stats.frameCount, 0);
}

// =============================================================================
// WorldTime Modifications Tests
// =============================================================================

class WorldTimeModificationsTest : public ::testing::Test {};

TEST_F(WorldTimeModificationsTest, CanSetDeltaTime) {
    WorldState state;
    state.time.deltaTime = 0.016;
    EXPECT_DOUBLE_EQ(state.time.deltaTime, 0.016);
}

TEST_F(WorldTimeModificationsTest, CanSetTotalTime) {
    WorldState state;
    state.time.totalTime = 123.456;
    EXPECT_DOUBLE_EQ(state.time.totalTime, 123.456);
}

TEST_F(WorldTimeModificationsTest, CanModifyBothTimeValues) {
    WorldState state;
    state.time.deltaTime = 0.033;
    state.time.totalTime = 99.99;
    EXPECT_DOUBLE_EQ(state.time.deltaTime, 0.033);
    EXPECT_DOUBLE_EQ(state.time.totalTime, 99.99);
}

TEST_F(WorldTimeModificationsTest, CanReadAndModifyDeltaTime) {
    WorldState state;
    state.time.deltaTime = 0.02;
    double dt = state.time.deltaTime;
    EXPECT_DOUBLE_EQ(dt, 0.02);
    state.time.deltaTime = dt * 2.0;
    EXPECT_DOUBLE_EQ(state.time.deltaTime, 0.04);
}

TEST_F(WorldTimeModificationsTest, CanAccumulateTotalTime) {
    WorldState state;
    state.time.totalTime = 10.0;
    state.time.totalTime += 0.016;
    EXPECT_DOUBLE_EQ(state.time.totalTime, 10.016);
}

// =============================================================================
// WorldStats Modifications Tests
// =============================================================================

class WorldStatsModificationsTest : public ::testing::Test {};

TEST_F(WorldStatsModificationsTest, CanSetFrameCount) {
    WorldState state;
    state.stats.frameCount = 42;
    EXPECT_EQ(state.stats.frameCount, 42);
}

TEST_F(WorldStatsModificationsTest, CanIncrementFrameCount) {
    WorldState state;
    state.stats.frameCount = 10;
    state.stats.frameCount++;
    EXPECT_EQ(state.stats.frameCount, 11);
}

TEST_F(WorldStatsModificationsTest, CanIncrementFrameCountMultipleTimes) {
    WorldState state;
    for (int i = 0; i < 100; ++i) {
        state.stats.frameCount++;
    }
    EXPECT_EQ(state.stats.frameCount, 100);
}

TEST_F(WorldStatsModificationsTest, CanAddToFrameCount) {
    WorldState state;
    state.stats.frameCount = 50;
    state.stats.frameCount += 25;
    EXPECT_EQ(state.stats.frameCount, 75);
}

// =============================================================================
// Value Boundaries Tests
// =============================================================================

class WorldStateBoundariesTest : public ::testing::Test {};

TEST_F(WorldStateBoundariesTest, CanHaveNegativeDeltaTime) {
    WorldState state;
    state.time.deltaTime = -0.5;
    EXPECT_DOUBLE_EQ(state.time.deltaTime, -0.5);
}

TEST_F(WorldStateBoundariesTest, CanHaveVeryLargeTotalTime) {
    WorldState state;
    state.time.totalTime = 1e10;
    EXPECT_DOUBLE_EQ(state.time.totalTime, 1e10);
}

TEST_F(WorldStateBoundariesTest, CanHaveNegativeFrameCount) {
    WorldState state;
    state.stats.frameCount = -100;
    EXPECT_EQ(state.stats.frameCount, -100);
}

TEST_F(WorldStateBoundariesTest, CanHaveMaxIntFrameCount) {
    WorldState state;
    state.stats.frameCount = std::numeric_limits<int>::max();
    EXPECT_EQ(state.stats.frameCount, std::numeric_limits<int>::max());
}

TEST_F(WorldStateBoundariesTest, CanHaveMinIntFrameCount) {
    WorldState state;
    state.stats.frameCount = std::numeric_limits<int>::min();
    EXPECT_EQ(state.stats.frameCount, std::numeric_limits<int>::min());
}

TEST_F(WorldStateBoundariesTest, CanHaveVerySmallDeltaTime) {
    WorldState state;
    state.time.deltaTime = 1e-10;
    EXPECT_DOUBLE_EQ(state.time.deltaTime, 1e-10);
}

// =============================================================================
// Copy Semantics Tests
// =============================================================================

class WorldStateCopyTest : public ::testing::Test {};

TEST_F(WorldStateCopyTest, CanCopyWorldState) {
    WorldState original;
    original.time.deltaTime = 0.016;
    original.time.totalTime = 100.0;
    original.stats.frameCount = 500;

    WorldState copy = original;

    EXPECT_DOUBLE_EQ(copy.time.deltaTime, 0.016);
    EXPECT_DOUBLE_EQ(copy.time.totalTime, 100.0);
    EXPECT_EQ(copy.stats.frameCount, 500);
}

TEST_F(WorldStateCopyTest, CopiesAreIndependent) {
    WorldState original;
    original.time.deltaTime = 0.016;
    original.time.totalTime = 100.0;
    original.stats.frameCount = 500;

    WorldState copy = original;
    copy.time.deltaTime = 0.033;
    copy.time.totalTime = 200.0;
    copy.stats.frameCount = 1000;

    EXPECT_DOUBLE_EQ(original.time.deltaTime, 0.016);
    EXPECT_DOUBLE_EQ(original.time.totalTime, 100.0);
    EXPECT_EQ(original.stats.frameCount, 500);
}

TEST_F(WorldStateCopyTest, CanAssignWorldState) {
    WorldState state1;
    state1.time.deltaTime = 0.016;
    state1.time.totalTime = 50.0;
    state1.stats.frameCount = 250;

    WorldState state2;
    state2 = state1;

    EXPECT_DOUBLE_EQ(state2.time.deltaTime, 0.016);
    EXPECT_DOUBLE_EQ(state2.time.totalTime, 50.0);
    EXPECT_EQ(state2.stats.frameCount, 250);
}

TEST_F(WorldStateCopyTest, CanCopyWorldTime) {
    WorldState state1;
    state1.time.deltaTime = 0.02;
    state1.time.totalTime = 75.0;

    WorldState state2;
    state2.time = state1.time;

    EXPECT_DOUBLE_EQ(state2.time.deltaTime, 0.02);
    EXPECT_DOUBLE_EQ(state2.time.totalTime, 75.0);
}

TEST_F(WorldStateCopyTest, CanCopyWorldStats) {
    WorldState state1;
    state1.stats.frameCount = 999;

    WorldState state2;
    state2.stats = state1.stats;

    EXPECT_EQ(state2.stats.frameCount, 999);
}

// =============================================================================
// Aggregate Initialization Tests
// =============================================================================

class WorldStateAggregateInitTest : public ::testing::Test {};

TEST_F(WorldStateAggregateInitTest, CanInitializeWithBraces) {
    WorldState state = {{0.016, 100.0}, {500}};
    EXPECT_DOUBLE_EQ(state.time.deltaTime, 0.016);
    EXPECT_DOUBLE_EQ(state.time.totalTime, 100.0);
    EXPECT_EQ(state.stats.frameCount, 500);
}

TEST_F(WorldStateAggregateInitTest, CanInitializeWorldTime) {
    WorldState state;
    state.time = {0.033, 250.0};
    EXPECT_DOUBLE_EQ(state.time.deltaTime, 0.033);
    EXPECT_DOUBLE_EQ(state.time.totalTime, 250.0);
}

TEST_F(WorldStateAggregateInitTest, CanInitializeWorldStats) {
    WorldState state;
    state.stats = {777};
    EXPECT_EQ(state.stats.frameCount, 777);
}

TEST_F(WorldStateAggregateInitTest, CanPartiallyInitialize) {
    WorldState state = {{0.016}};
    EXPECT_DOUBLE_EQ(state.time.deltaTime, 0.016);
    EXPECT_DOUBLE_EQ(state.time.totalTime, 0.0);
    EXPECT_EQ(state.stats.frameCount, 0);
}

}  // namespace nexo
