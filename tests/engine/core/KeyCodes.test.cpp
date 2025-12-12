//// KeyCodes.test.cpp /////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for KeyCodes definitions
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "core/event/KeyCodes.hpp"
#include <set>

namespace nexo::event {

class KeyCodesTest : public ::testing::Test {};

// =============================================================================
// Key Code Value Tests
// =============================================================================

TEST_F(KeyCodesTest, SpaceKeyValue) {
    EXPECT_EQ(NEXO_KEY_SPACE, 32);
}

TEST_F(KeyCodesTest, NumberKeyValues) {
    EXPECT_EQ(NEXO_KEY_1, 49);
    EXPECT_EQ(NEXO_KEY_2, 50);
    EXPECT_EQ(NEXO_KEY_3, 51);
}

TEST_F(KeyCodesTest, LetterKeyValues) {
    EXPECT_EQ(NEXO_KEY_Q, 65);
    EXPECT_EQ(NEXO_KEY_D, 68);
    EXPECT_EQ(NEXO_KEY_E, 69);
    EXPECT_EQ(NEXO_KEY_I, 73);
    EXPECT_EQ(NEXO_KEY_J, 74);
    EXPECT_EQ(NEXO_KEY_K, 75);
    EXPECT_EQ(NEXO_KEY_L, 76);
    EXPECT_EQ(NEXO_KEY_A, 81);
    EXPECT_EQ(NEXO_KEY_S, 83);
    EXPECT_EQ(NEXO_KEY_Z, 87);
}

TEST_F(KeyCodesTest, TabKeyValue) {
    EXPECT_EQ(NEXO_KEY_TAB, 258);
}

TEST_F(KeyCodesTest, ArrowKeyValues) {
    EXPECT_EQ(NEXO_KEY_RIGHT, 262);
    EXPECT_EQ(NEXO_KEY_LEFT, 263);
    EXPECT_EQ(NEXO_KEY_DOWN, 264);
    EXPECT_EQ(NEXO_KEY_UP, 265);
}

TEST_F(KeyCodesTest, ShiftKeyValue) {
    EXPECT_EQ(NEXO_KEY_SHIFT, 340);
}

// =============================================================================
// Mouse Button Tests
// =============================================================================

TEST_F(KeyCodesTest, MouseLeftButtonValue) {
    EXPECT_EQ(NEXO_MOUSE_LEFT, 0);
}

TEST_F(KeyCodesTest, MouseRightButtonValue) {
    EXPECT_EQ(NEXO_MOUSE_RIGHT, 1);
}

// =============================================================================
// Uniqueness Tests
// =============================================================================

TEST_F(KeyCodesTest, AllKeyCodesAreUnique) {
    std::set<int> keyCodes = {
        NEXO_KEY_SPACE,
        NEXO_KEY_1, NEXO_KEY_2, NEXO_KEY_3,
        NEXO_KEY_Q, NEXO_KEY_D, NEXO_KEY_E, NEXO_KEY_I,
        NEXO_KEY_J, NEXO_KEY_K, NEXO_KEY_L, NEXO_KEY_A,
        NEXO_KEY_S, NEXO_KEY_Z,
        NEXO_KEY_TAB,
        NEXO_KEY_RIGHT, NEXO_KEY_LEFT, NEXO_KEY_DOWN, NEXO_KEY_UP,
        NEXO_KEY_SHIFT
    };
    // 20 key codes should all be unique
    EXPECT_EQ(keyCodes.size(), 20u);
}

TEST_F(KeyCodesTest, MouseButtonsAreUnique) {
    EXPECT_NE(NEXO_MOUSE_LEFT, NEXO_MOUSE_RIGHT);
}

TEST_F(KeyCodesTest, MouseButtonsDontConflictWithKeys) {
    // Mouse buttons should not conflict with keyboard keys
    std::set<int> keyCodes = {
        NEXO_KEY_SPACE, NEXO_KEY_1, NEXO_KEY_2, NEXO_KEY_3,
        NEXO_KEY_Q, NEXO_KEY_D, NEXO_KEY_E, NEXO_KEY_I,
        NEXO_KEY_J, NEXO_KEY_K, NEXO_KEY_L, NEXO_KEY_A,
        NEXO_KEY_S, NEXO_KEY_Z, NEXO_KEY_TAB,
        NEXO_KEY_RIGHT, NEXO_KEY_LEFT, NEXO_KEY_DOWN, NEXO_KEY_UP,
        NEXO_KEY_SHIFT
    };
    EXPECT_EQ(keyCodes.count(NEXO_MOUSE_LEFT), 0u);
    EXPECT_EQ(keyCodes.count(NEXO_MOUSE_RIGHT), 0u);
}

// =============================================================================
// Range Tests
// =============================================================================

TEST_F(KeyCodesTest, ArrowKeysAreContiguous) {
    // Arrow keys should be in a contiguous range
    EXPECT_EQ(NEXO_KEY_LEFT - NEXO_KEY_RIGHT, 1);
    EXPECT_EQ(NEXO_KEY_DOWN - NEXO_KEY_LEFT, 1);
    EXPECT_EQ(NEXO_KEY_UP - NEXO_KEY_DOWN, 1);
}

TEST_F(KeyCodesTest, NumberKeysAreContiguous) {
    EXPECT_EQ(NEXO_KEY_2 - NEXO_KEY_1, 1);
    EXPECT_EQ(NEXO_KEY_3 - NEXO_KEY_2, 1);
}

TEST_F(KeyCodesTest, KeyCodesArePositive) {
    EXPECT_GT(NEXO_KEY_SPACE, 0);
    EXPECT_GT(NEXO_KEY_1, 0);
    EXPECT_GT(NEXO_KEY_TAB, 0);
    EXPECT_GT(NEXO_KEY_SHIFT, 0);
}

TEST_F(KeyCodesTest, MouseButtonsAreNonNegative) {
    EXPECT_GE(NEXO_MOUSE_LEFT, 0);
    EXPECT_GE(NEXO_MOUSE_RIGHT, 0);
}

}  // namespace nexo::event
