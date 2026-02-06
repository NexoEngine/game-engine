//// Types.test.cpp ///////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for common engine types
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "Types.hpp"
#include <type_traits>

namespace nexo {

// =============================================================================
// RenderingType Tests
// =============================================================================

class RenderingTypeTest : public ::testing::Test {};

TEST_F(RenderingTypeTest, IsEnumClass) {
    static_assert(std::is_enum_v<RenderingType>);
    static_assert(!std::is_convertible_v<RenderingType, int>);
    SUCCEED();
}

TEST_F(RenderingTypeTest, HasWindowValue) {
    RenderingType type = RenderingType::WINDOW;
    EXPECT_EQ(type, RenderingType::WINDOW);
}

TEST_F(RenderingTypeTest, HasFramebufferValue) {
    RenderingType type = RenderingType::FRAMEBUFFER;
    EXPECT_EQ(type, RenderingType::FRAMEBUFFER);
}

TEST_F(RenderingTypeTest, ValuesAreDistinct) {
    EXPECT_NE(RenderingType::WINDOW, RenderingType::FRAMEBUFFER);
}

TEST_F(RenderingTypeTest, CanBeUsedInSwitch) {
    RenderingType type = RenderingType::WINDOW;
    bool handled = false;

    switch (type) {
        case RenderingType::WINDOW:
            handled = true;
            break;
        case RenderingType::FRAMEBUFFER:
            handled = false;
            break;
    }

    EXPECT_TRUE(handled);
}

TEST_F(RenderingTypeTest, CanBeCompared) {
    RenderingType type1 = RenderingType::WINDOW;
    RenderingType type2 = RenderingType::WINDOW;
    RenderingType type3 = RenderingType::FRAMEBUFFER;

    EXPECT_TRUE(type1 == type2);
    EXPECT_FALSE(type1 == type3);
}

TEST_F(RenderingTypeTest, CanBeCopied) {
    RenderingType original = RenderingType::FRAMEBUFFER;
    RenderingType copy = original;
    EXPECT_EQ(copy, RenderingType::FRAMEBUFFER);
}

TEST_F(RenderingTypeTest, CanBeAssigned) {
    RenderingType type = RenderingType::WINDOW;
    type = RenderingType::FRAMEBUFFER;
    EXPECT_EQ(type, RenderingType::FRAMEBUFFER);
}

// =============================================================================
// SceneType Tests
// =============================================================================

class SceneTypeTest : public ::testing::Test {};

TEST_F(SceneTypeTest, IsEnumClass) {
    static_assert(std::is_enum_v<SceneType>);
    static_assert(!std::is_convertible_v<SceneType, int>);
    SUCCEED();
}

TEST_F(SceneTypeTest, HasEditorValue) {
    SceneType type = SceneType::EDITOR;
    EXPECT_EQ(type, SceneType::EDITOR);
}

TEST_F(SceneTypeTest, HasGameValue) {
    SceneType type = SceneType::GAME;
    EXPECT_EQ(type, SceneType::GAME);
}

TEST_F(SceneTypeTest, ValuesAreDistinct) {
    EXPECT_NE(SceneType::EDITOR, SceneType::GAME);
}

TEST_F(SceneTypeTest, CanBeUsedInSwitch) {
    SceneType type = SceneType::GAME;
    bool isGame = false;

    switch (type) {
        case SceneType::EDITOR:
            isGame = false;
            break;
        case SceneType::GAME:
            isGame = true;
            break;
    }

    EXPECT_TRUE(isGame);
}

TEST_F(SceneTypeTest, CanBeCompared) {
    SceneType type1 = SceneType::EDITOR;
    SceneType type2 = SceneType::EDITOR;
    SceneType type3 = SceneType::GAME;

    EXPECT_TRUE(type1 == type2);
    EXPECT_FALSE(type1 == type3);
}

TEST_F(SceneTypeTest, CanBeCopied) {
    SceneType original = SceneType::GAME;
    SceneType copy = original;
    EXPECT_EQ(copy, SceneType::GAME);
}

TEST_F(SceneTypeTest, CanBeAssigned) {
    SceneType type = SceneType::EDITOR;
    type = SceneType::GAME;
    EXPECT_EQ(type, SceneType::GAME);
}

// =============================================================================
// Cross-Type Tests
// =============================================================================

class TypesCrossTest : public ::testing::Test {};

TEST_F(TypesCrossTest, TypesAreIndependent) {
    // Ensure these are separate enum types
    static_assert(!std::is_same_v<RenderingType, SceneType>);
    SUCCEED();
}

TEST_F(TypesCrossTest, BothTypesScopedToNexo) {
    // Both should be in nexo namespace
    using namespace nexo;
    RenderingType rt = RenderingType::WINDOW;
    SceneType st = SceneType::EDITOR;
    (void)rt;
    (void)st;
    SUCCEED();
}

}  // namespace nexo
