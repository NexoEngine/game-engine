//// KeyCodes.test.cpp /////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for KeyCodes definitions
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "core/event/KeyCodes.hpp"
#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

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

// =============================================================================
// Comprehensive Key Code Value Tests
// =============================================================================

TEST_F(KeyCodesTest, AllNumberKeysHaveCorrectValues) {
    // Testing all number keys 0-9 that exist
    EXPECT_EQ(NEXO_KEY_1, 49);
    EXPECT_EQ(NEXO_KEY_2, 50);
    EXPECT_EQ(NEXO_KEY_3, 51);
}

TEST_F(KeyCodesTest, AllLetterKeysHaveCorrectValues) {
    // Testing all letter keys that are defined
    EXPECT_EQ(NEXO_KEY_A, 81);  // AZERTY layout
    EXPECT_EQ(NEXO_KEY_D, 68);
    EXPECT_EQ(NEXO_KEY_E, 69);
    EXPECT_EQ(NEXO_KEY_I, 73);
    EXPECT_EQ(NEXO_KEY_J, 74);
    EXPECT_EQ(NEXO_KEY_K, 75);
    EXPECT_EQ(NEXO_KEY_L, 76);
    EXPECT_EQ(NEXO_KEY_Q, 65);
    EXPECT_EQ(NEXO_KEY_S, 83);
    EXPECT_EQ(NEXO_KEY_Z, 87);
}

TEST_F(KeyCodesTest, AllModifierKeysHaveCorrectValues) {
    // Only SHIFT is defined
    EXPECT_EQ(NEXO_KEY_SHIFT, 340);
}

TEST_F(KeyCodesTest, AllSpecialKeysHaveCorrectValues) {
    EXPECT_EQ(NEXO_KEY_SPACE, 32);
    EXPECT_EQ(NEXO_KEY_TAB, 258);
}

TEST_F(KeyCodesTest, AllArrowKeysHaveCorrectValues) {
    EXPECT_EQ(NEXO_KEY_RIGHT, 262);
    EXPECT_EQ(NEXO_KEY_LEFT, 263);
    EXPECT_EQ(NEXO_KEY_DOWN, 264);
    EXPECT_EQ(NEXO_KEY_UP, 265);
}

TEST_F(KeyCodesTest, AllMouseButtonsHaveCorrectValues) {
    EXPECT_EQ(NEXO_MOUSE_LEFT, 0);
    EXPECT_EQ(NEXO_MOUSE_RIGHT, 1);
}

// =============================================================================
// Extended Uniqueness Tests
// =============================================================================

TEST_F(KeyCodesTest, AllDefinedKeyCodesAreGloballyUnique) {
    std::set<int> allCodes = {
        NEXO_KEY_SPACE,
        NEXO_KEY_1, NEXO_KEY_2, NEXO_KEY_3,
        NEXO_KEY_Q, NEXO_KEY_A, NEXO_KEY_Z,
        NEXO_KEY_D, NEXO_KEY_E,
        NEXO_KEY_I, NEXO_KEY_J, NEXO_KEY_K, NEXO_KEY_L,
        NEXO_KEY_S,
        NEXO_KEY_TAB,
        NEXO_KEY_RIGHT, NEXO_KEY_LEFT, NEXO_KEY_DOWN, NEXO_KEY_UP,
        NEXO_KEY_SHIFT,
        NEXO_MOUSE_LEFT, NEXO_MOUSE_RIGHT
    };
    // All 22 codes should be unique
    EXPECT_EQ(allCodes.size(), 22u);
}

TEST_F(KeyCodesTest, LetterKeysAreAllDifferent) {
    std::set<int> letterKeys = {
        NEXO_KEY_Q, NEXO_KEY_A, NEXO_KEY_Z,
        NEXO_KEY_D, NEXO_KEY_E,
        NEXO_KEY_I, NEXO_KEY_J, NEXO_KEY_K, NEXO_KEY_L,
        NEXO_KEY_S
    };
    EXPECT_EQ(letterKeys.size(), 10u);
}

TEST_F(KeyCodesTest, NumberKeysAreAllDifferent) {
    std::set<int> numberKeys = {
        NEXO_KEY_1, NEXO_KEY_2, NEXO_KEY_3
    };
    EXPECT_EQ(numberKeys.size(), 3u);
}

TEST_F(KeyCodesTest, ArrowKeysAreAllDifferent) {
    std::set<int> arrowKeys = {
        NEXO_KEY_RIGHT, NEXO_KEY_LEFT, NEXO_KEY_DOWN, NEXO_KEY_UP
    };
    EXPECT_EQ(arrowKeys.size(), 4u);
}

TEST_F(KeyCodesTest, SpecialKeysAreAllDifferent) {
    std::set<int> specialKeys = {
        NEXO_KEY_SPACE, NEXO_KEY_TAB, NEXO_KEY_SHIFT
    };
    EXPECT_EQ(specialKeys.size(), 3u);
}

// =============================================================================
// Key Code Range and Boundary Tests
// =============================================================================

TEST_F(KeyCodesTest, PrintableASCIIKeysInValidRange) {
    // Space and number keys use printable ASCII range (32-126)
    EXPECT_GE(NEXO_KEY_SPACE, 32);
    EXPECT_LE(NEXO_KEY_SPACE, 126);
    EXPECT_GE(NEXO_KEY_1, 32);
    EXPECT_LE(NEXO_KEY_1, 126);
    EXPECT_GE(NEXO_KEY_2, 32);
    EXPECT_LE(NEXO_KEY_2, 126);
    EXPECT_GE(NEXO_KEY_3, 32);
    EXPECT_LE(NEXO_KEY_3, 126);
}

TEST_F(KeyCodesTest, LetterKeysInValidASCIIRange) {
    // Letter keys should be in uppercase ASCII range (65-90)
    EXPECT_GE(NEXO_KEY_Q, 65);
    EXPECT_LE(NEXO_KEY_Q, 90);
    EXPECT_GE(NEXO_KEY_A, 65);
    EXPECT_LE(NEXO_KEY_A, 90);
    EXPECT_GE(NEXO_KEY_Z, 65);
    EXPECT_LE(NEXO_KEY_Z, 90);
    EXPECT_GE(NEXO_KEY_D, 65);
    EXPECT_LE(NEXO_KEY_D, 90);
    EXPECT_GE(NEXO_KEY_E, 65);
    EXPECT_LE(NEXO_KEY_E, 90);
}

TEST_F(KeyCodesTest, ExtendedKeysInHigherRange) {
    // Extended keys (Tab, arrows, modifiers) use higher values (>256)
    EXPECT_GT(NEXO_KEY_TAB, 256);
    EXPECT_GT(NEXO_KEY_RIGHT, 256);
    EXPECT_GT(NEXO_KEY_LEFT, 256);
    EXPECT_GT(NEXO_KEY_DOWN, 256);
    EXPECT_GT(NEXO_KEY_UP, 256);
    EXPECT_GT(NEXO_KEY_SHIFT, 256);
}

TEST_F(KeyCodesTest, KeyCodesDoNotOverlapBetweenRanges) {
    // ASCII keys should be < 256
    EXPECT_LT(NEXO_KEY_SPACE, 256);
    EXPECT_LT(NEXO_KEY_1, 256);
    EXPECT_LT(NEXO_KEY_Q, 256);

    // Extended keys should be >= 256
    EXPECT_GE(NEXO_KEY_TAB, 256);
    EXPECT_GE(NEXO_KEY_SHIFT, 256);
}

TEST_F(KeyCodesTest, MouseButtonsInLowRange) {
    // Mouse buttons should be in low range (0-7 typically)
    EXPECT_LT(NEXO_MOUSE_LEFT, 8);
    EXPECT_LT(NEXO_MOUSE_RIGHT, 8);
}

// =============================================================================
// Contiguity and Ordering Tests
// =============================================================================

TEST_F(KeyCodesTest, NumberKeysAreSequential) {
    // Number keys 1-3 should be sequential in ASCII
    EXPECT_EQ(NEXO_KEY_2 - NEXO_KEY_1, 1);
    EXPECT_EQ(NEXO_KEY_3 - NEXO_KEY_2, 1);
}

TEST_F(KeyCodesTest, ArrowKeysFormContiguousBlock) {
    // Arrow keys should form a contiguous block
    EXPECT_EQ(NEXO_KEY_LEFT - NEXO_KEY_RIGHT, 1);
    EXPECT_EQ(NEXO_KEY_DOWN - NEXO_KEY_LEFT, 1);
    EXPECT_EQ(NEXO_KEY_UP - NEXO_KEY_DOWN, 1);
}

TEST_F(KeyCodesTest, ArrowKeysInCorrectOrder) {
    // Verify ordering: RIGHT < LEFT < DOWN < UP
    EXPECT_LT(NEXO_KEY_RIGHT, NEXO_KEY_LEFT);
    EXPECT_LT(NEXO_KEY_LEFT, NEXO_KEY_DOWN);
    EXPECT_LT(NEXO_KEY_DOWN, NEXO_KEY_UP);
}

TEST_F(KeyCodesTest, MouseButtonsInOrder) {
    EXPECT_LT(NEXO_MOUSE_LEFT, NEXO_MOUSE_RIGHT);
    EXPECT_EQ(NEXO_MOUSE_RIGHT - NEXO_MOUSE_LEFT, 1);
}

// =============================================================================
// Key Code Category Tests
// =============================================================================

TEST_F(KeyCodesTest, CanIdentifyPrintableKeys) {
    // Keys that are printable ASCII characters
    auto isPrintableASCII = [](int code) {
        return code >= 32 && code <= 126;
    };

    EXPECT_TRUE(isPrintableASCII(NEXO_KEY_SPACE));
    EXPECT_TRUE(isPrintableASCII(NEXO_KEY_1));
    EXPECT_TRUE(isPrintableASCII(NEXO_KEY_2));
    EXPECT_TRUE(isPrintableASCII(NEXO_KEY_3));
    EXPECT_TRUE(isPrintableASCII(NEXO_KEY_Q));
    EXPECT_TRUE(isPrintableASCII(NEXO_KEY_A));
}

TEST_F(KeyCodesTest, CanIdentifyExtendedKeys) {
    // Keys that are not printable ASCII (special keys)
    auto isExtendedKey = [](int code) {
        return code > 256;
    };

    EXPECT_TRUE(isExtendedKey(NEXO_KEY_TAB));
    EXPECT_TRUE(isExtendedKey(NEXO_KEY_RIGHT));
    EXPECT_TRUE(isExtendedKey(NEXO_KEY_LEFT));
    EXPECT_TRUE(isExtendedKey(NEXO_KEY_DOWN));
    EXPECT_TRUE(isExtendedKey(NEXO_KEY_UP));
    EXPECT_TRUE(isExtendedKey(NEXO_KEY_SHIFT));
}

TEST_F(KeyCodesTest, CanIdentifyLetterKeys) {
    // Letters are in uppercase ASCII range
    auto isLetterKey = [](int code) {
        return code >= 65 && code <= 90;
    };

    EXPECT_TRUE(isLetterKey(NEXO_KEY_Q));
    EXPECT_TRUE(isLetterKey(NEXO_KEY_A));
    EXPECT_TRUE(isLetterKey(NEXO_KEY_Z));
    EXPECT_TRUE(isLetterKey(NEXO_KEY_D));
    EXPECT_TRUE(isLetterKey(NEXO_KEY_E));
    EXPECT_TRUE(isLetterKey(NEXO_KEY_I));
    EXPECT_TRUE(isLetterKey(NEXO_KEY_J));
    EXPECT_TRUE(isLetterKey(NEXO_KEY_K));
    EXPECT_TRUE(isLetterKey(NEXO_KEY_L));
    EXPECT_TRUE(isLetterKey(NEXO_KEY_S));
}

TEST_F(KeyCodesTest, CanIdentifyNumberKeys) {
    // Number keys are in ASCII range 48-57
    auto isNumberKey = [](int code) {
        return code >= 48 && code <= 57;
    };

    EXPECT_TRUE(isNumberKey(NEXO_KEY_1));
    EXPECT_TRUE(isNumberKey(NEXO_KEY_2));
    EXPECT_TRUE(isNumberKey(NEXO_KEY_3));
}

// =============================================================================
// Macro Consistency Tests
// =============================================================================

TEST_F(KeyCodesTest, MacrosExpandToConstants) {
    // Ensure macros expand to constant values (not expressions)
    constexpr int space = NEXO_KEY_SPACE;
    constexpr int shift = NEXO_KEY_SHIFT;
    constexpr int mouse_left = NEXO_MOUSE_LEFT;

    EXPECT_EQ(space, 32);
    EXPECT_EQ(shift, 340);
    EXPECT_EQ(mouse_left, 0);
}

TEST_F(KeyCodesTest, MacrosCanBeUsedInArrays) {
    // Test that macros can be used as array indices
    int keyStates[512] = {0};
    keyStates[NEXO_KEY_SPACE] = 1;
    keyStates[NEXO_KEY_TAB] = 2;
    keyStates[NEXO_KEY_SHIFT] = 3;

    EXPECT_EQ(keyStates[NEXO_KEY_SPACE], 1);
    EXPECT_EQ(keyStates[NEXO_KEY_TAB], 2);
    EXPECT_EQ(keyStates[NEXO_KEY_SHIFT], 3);
}

TEST_F(KeyCodesTest, MouseButtonsCanBeUsedInArrays) {
    int mouseButtonStates[8] = {0};
    mouseButtonStates[NEXO_MOUSE_LEFT] = 1;
    mouseButtonStates[NEXO_MOUSE_RIGHT] = 2;

    EXPECT_EQ(mouseButtonStates[NEXO_MOUSE_LEFT], 1);
    EXPECT_EQ(mouseButtonStates[NEXO_MOUSE_RIGHT], 2);
}

// =============================================================================
// Switch Statement Compatibility Tests
// =============================================================================

TEST_F(KeyCodesTest, KeyCodesCanBeUsedInSwitch) {
    auto processKey = [](int keyCode) -> int {
        switch (keyCode) {
            case NEXO_KEY_SPACE: return 1;
            case NEXO_KEY_TAB: return 2;
            case NEXO_KEY_SHIFT: return 3;
            case NEXO_KEY_UP: return 4;
            case NEXO_KEY_DOWN: return 5;
            case NEXO_KEY_LEFT: return 6;
            case NEXO_KEY_RIGHT: return 7;
            default: return 0;
        }
    };

    EXPECT_EQ(processKey(NEXO_KEY_SPACE), 1);
    EXPECT_EQ(processKey(NEXO_KEY_TAB), 2);
    EXPECT_EQ(processKey(NEXO_KEY_SHIFT), 3);
    EXPECT_EQ(processKey(NEXO_KEY_UP), 4);
    EXPECT_EQ(processKey(NEXO_KEY_DOWN), 5);
    EXPECT_EQ(processKey(NEXO_KEY_LEFT), 6);
    EXPECT_EQ(processKey(NEXO_KEY_RIGHT), 7);
}

TEST_F(KeyCodesTest, MouseButtonsCanBeUsedInSwitch) {
    auto processMouseButton = [](int button) -> int {
        switch (button) {
            case NEXO_MOUSE_LEFT: return 1;
            case NEXO_MOUSE_RIGHT: return 2;
            default: return 0;
        }
    };

    EXPECT_EQ(processMouseButton(NEXO_MOUSE_LEFT), 1);
    EXPECT_EQ(processMouseButton(NEXO_MOUSE_RIGHT), 2);
}

// =============================================================================
// Comparison and Equality Tests
// =============================================================================

TEST_F(KeyCodesTest, KeyCodesCanBeCompared) {
    EXPECT_TRUE(NEXO_KEY_SPACE == NEXO_KEY_SPACE);
    EXPECT_TRUE(NEXO_KEY_SPACE != NEXO_KEY_TAB);
    EXPECT_TRUE(NEXO_KEY_1 < NEXO_KEY_2);
    EXPECT_TRUE(NEXO_KEY_3 > NEXO_KEY_1);
    EXPECT_TRUE(NEXO_KEY_RIGHT <= NEXO_KEY_LEFT);
    EXPECT_TRUE(NEXO_KEY_UP >= NEXO_KEY_DOWN);
}

TEST_F(KeyCodesTest, MouseButtonsCanBeCompared) {
    EXPECT_TRUE(NEXO_MOUSE_LEFT == NEXO_MOUSE_LEFT);
    EXPECT_TRUE(NEXO_MOUSE_LEFT != NEXO_MOUSE_RIGHT);
    EXPECT_TRUE(NEXO_MOUSE_LEFT < NEXO_MOUSE_RIGHT);
    EXPECT_TRUE(NEXO_MOUSE_RIGHT > NEXO_MOUSE_LEFT);
}

// =============================================================================
// Edge Case and Boundary Tests
// =============================================================================

TEST_F(KeyCodesTest, NoKeyCodeIsZeroExceptMouseLeft) {
    // Only NEXO_MOUSE_LEFT should be 0
    EXPECT_NE(NEXO_KEY_SPACE, 0);
    EXPECT_NE(NEXO_KEY_1, 0);
    EXPECT_NE(NEXO_KEY_TAB, 0);
    EXPECT_NE(NEXO_KEY_SHIFT, 0);
    EXPECT_EQ(NEXO_MOUSE_LEFT, 0);
}

TEST_F(KeyCodesTest, NoKeyCodeIsNegative) {
    // All key codes should be non-negative
    EXPECT_GE(NEXO_KEY_SPACE, 0);
    EXPECT_GE(NEXO_KEY_1, 0);
    EXPECT_GE(NEXO_KEY_Q, 0);
    EXPECT_GE(NEXO_KEY_TAB, 0);
    EXPECT_GE(NEXO_KEY_SHIFT, 0);
    EXPECT_GE(NEXO_KEY_UP, 0);
    EXPECT_GE(NEXO_MOUSE_LEFT, 0);
    EXPECT_GE(NEXO_MOUSE_RIGHT, 0);
}

TEST_F(KeyCodesTest, KeyCodesAreWithinReasonableBounds) {
    // All keyboard keys should be less than 512 (common key code limit)
    EXPECT_LT(NEXO_KEY_SPACE, 512);
    EXPECT_LT(NEXO_KEY_1, 512);
    EXPECT_LT(NEXO_KEY_Q, 512);
    EXPECT_LT(NEXO_KEY_TAB, 512);
    EXPECT_LT(NEXO_KEY_SHIFT, 512);
    EXPECT_LT(NEXO_KEY_UP, 512);
}

TEST_F(KeyCodesTest, MouseButtonsWithinValidRange) {
    // Mouse buttons typically range from 0-7
    EXPECT_GE(NEXO_MOUSE_LEFT, 0);
    EXPECT_LT(NEXO_MOUSE_LEFT, 8);
    EXPECT_GE(NEXO_MOUSE_RIGHT, 0);
    EXPECT_LT(NEXO_MOUSE_RIGHT, 8);
}

// =============================================================================
// Type Safety Tests
// =============================================================================

TEST_F(KeyCodesTest, KeyCodeTypesAreConsistent) {
    // All key codes should be implicitly convertible to int
    int space_val = NEXO_KEY_SPACE;
    int shift_val = NEXO_KEY_SHIFT;
    int mouse_val = NEXO_MOUSE_LEFT;

    EXPECT_EQ(space_val, 32);
    EXPECT_EQ(shift_val, 340);
    EXPECT_EQ(mouse_val, 0);
}

TEST_F(KeyCodesTest, KeyCodesCanBeStoredInContainers) {
    std::vector<int> keys = {
        NEXO_KEY_SPACE, NEXO_KEY_TAB, NEXO_KEY_SHIFT,
        NEXO_KEY_UP, NEXO_KEY_DOWN, NEXO_KEY_LEFT, NEXO_KEY_RIGHT
    };

    EXPECT_EQ(keys.size(), 7u);
    EXPECT_EQ(keys[0], NEXO_KEY_SPACE);
    EXPECT_EQ(keys[1], NEXO_KEY_TAB);
    EXPECT_EQ(keys[2], NEXO_KEY_SHIFT);
}

TEST_F(KeyCodesTest, KeyCodesCanBeUsedInMaps) {
    std::map<int, std::string> keyNames;
    keyNames[NEXO_KEY_SPACE] = "Space";
    keyNames[NEXO_KEY_TAB] = "Tab";
    keyNames[NEXO_KEY_SHIFT] = "Shift";

    EXPECT_EQ(keyNames[NEXO_KEY_SPACE], "Space");
    EXPECT_EQ(keyNames[NEXO_KEY_TAB], "Tab");
    EXPECT_EQ(keyNames[NEXO_KEY_SHIFT], "Shift");
}

// =============================================================================
// Keyboard Layout Awareness Tests
// =============================================================================

TEST_F(KeyCodesTest, AZERTYLayoutCorrectness) {
    // The key codes suggest AZERTY layout:
    // NEXO_KEY_Q = 65 (ASCII 'A' in QWERTY)
    // NEXO_KEY_A = 81 (ASCII 'Q' in QWERTY)
    // NEXO_KEY_Z = 87 (ASCII 'W' in QWERTY)

    // In AZERTY: Q is where A is on QWERTY
    EXPECT_EQ(NEXO_KEY_Q, 65);  // 'A' in ASCII
    // In AZERTY: A is where Q is on QWERTY
    EXPECT_EQ(NEXO_KEY_A, 81);  // 'Q' in ASCII
    // In AZERTY: Z is where W is on QWERTY
    EXPECT_EQ(NEXO_KEY_Z, 87);  // 'W' in ASCII
}

TEST_F(KeyCodesTest, StandardASCIILetterKeys) {
    // These keys use standard ASCII values (not remapped)
    EXPECT_EQ(NEXO_KEY_D, 68);  // 'D' in ASCII
    EXPECT_EQ(NEXO_KEY_E, 69);  // 'E' in ASCII
    EXPECT_EQ(NEXO_KEY_I, 73);  // 'I' in ASCII
    EXPECT_EQ(NEXO_KEY_J, 74);  // 'J' in ASCII
    EXPECT_EQ(NEXO_KEY_K, 75);  // 'K' in ASCII
    EXPECT_EQ(NEXO_KEY_L, 76);  // 'L' in ASCII
    EXPECT_EQ(NEXO_KEY_S, 83);  // 'S' in ASCII
}

// =============================================================================
// Integration and Practical Usage Tests
// =============================================================================

TEST_F(KeyCodesTest, CanCreateKeyStateMap) {
    std::unordered_map<int, bool> keyStates;

    // Initialize all keys as not pressed
    keyStates[NEXO_KEY_SPACE] = false;
    keyStates[NEXO_KEY_1] = false;
    keyStates[NEXO_KEY_Q] = false;
    keyStates[NEXO_KEY_SHIFT] = false;
    keyStates[NEXO_KEY_UP] = false;

    // Simulate key presses
    keyStates[NEXO_KEY_SPACE] = true;
    keyStates[NEXO_KEY_UP] = true;

    EXPECT_TRUE(keyStates[NEXO_KEY_SPACE]);
    EXPECT_FALSE(keyStates[NEXO_KEY_1]);
    EXPECT_TRUE(keyStates[NEXO_KEY_UP]);
}

TEST_F(KeyCodesTest, CanDetectKeyCodeCollisions) {
    // Ensure no keyboard key collides with mouse buttons
    std::set<int> keyboardKeys = {
        NEXO_KEY_SPACE, NEXO_KEY_1, NEXO_KEY_2, NEXO_KEY_3,
        NEXO_KEY_Q, NEXO_KEY_A, NEXO_KEY_Z, NEXO_KEY_D, NEXO_KEY_E,
        NEXO_KEY_I, NEXO_KEY_J, NEXO_KEY_K, NEXO_KEY_L, NEXO_KEY_S,
        NEXO_KEY_TAB, NEXO_KEY_RIGHT, NEXO_KEY_LEFT, NEXO_KEY_DOWN, NEXO_KEY_UP,
        NEXO_KEY_SHIFT
    };

    std::set<int> mouseButtons = {
        NEXO_MOUSE_LEFT, NEXO_MOUSE_RIGHT
    };

    // Check for any intersection
    std::vector<int> intersection;
    std::set_intersection(
        keyboardKeys.begin(), keyboardKeys.end(),
        mouseButtons.begin(), mouseButtons.end(),
        std::back_inserter(intersection)
    );

    EXPECT_TRUE(intersection.empty()) << "Keyboard keys and mouse buttons should not share codes";
}

TEST_F(KeyCodesTest, KeyCodesPersistAcrossMultipleReferences) {
    // Ensure key codes are consistent when referenced multiple times
    int first_ref = NEXO_KEY_SPACE;
    int second_ref = NEXO_KEY_SPACE;
    int third_ref = NEXO_KEY_SPACE;

    EXPECT_EQ(first_ref, second_ref);
    EXPECT_EQ(second_ref, third_ref);
    EXPECT_EQ(first_ref, 32);
}

}  // namespace nexo::event
