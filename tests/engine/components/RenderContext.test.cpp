//// RenderContext.test.cpp ////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for RenderContext singleton component
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "components/RenderContext.hpp"

namespace nexo::components {

class RenderContextTest : public ::testing::Test {};

// =============================================================================
// GridParams Default Value Tests
// =============================================================================

TEST_F(RenderContextTest, GridParamsDefaultEnabled) {
    RenderContext::GridParams params;
    EXPECT_TRUE(params.enabled);
}

TEST_F(RenderContextTest, GridParamsDefaultGridSize) {
    RenderContext::GridParams params;
    EXPECT_FLOAT_EQ(params.gridSize, 100.0f);
}

TEST_F(RenderContextTest, GridParamsDefaultMinPixelsBetweenCells) {
    RenderContext::GridParams params;
    EXPECT_FLOAT_EQ(params.minPixelsBetweenCells, 2.0f);
}

TEST_F(RenderContextTest, GridParamsDefaultCellSize) {
    RenderContext::GridParams params;
    EXPECT_FLOAT_EQ(params.cellSize, 0.025f);
}

// =============================================================================
// GridParams Modification Tests
// =============================================================================

TEST_F(RenderContextTest, GridParamsModifyEnabled) {
    RenderContext::GridParams params;
    params.enabled = false;
    EXPECT_FALSE(params.enabled);
}

TEST_F(RenderContextTest, GridParamsModifyGridSize) {
    RenderContext::GridParams params;
    params.gridSize = 200.0f;
    EXPECT_FLOAT_EQ(params.gridSize, 200.0f);
}

TEST_F(RenderContextTest, GridParamsModifyMinPixelsBetweenCells) {
    RenderContext::GridParams params;
    params.minPixelsBetweenCells = 5.0f;
    EXPECT_FLOAT_EQ(params.minPixelsBetweenCells, 5.0f);
}

TEST_F(RenderContextTest, GridParamsModifyCellSize) {
    RenderContext::GridParams params;
    params.cellSize = 0.1f;
    EXPECT_FLOAT_EQ(params.cellSize, 0.1f);
}

// =============================================================================
// RenderContext Default Value Tests
// =============================================================================

TEST_F(RenderContextTest, DefaultSceneRendered) {
    RenderContext ctx;
    EXPECT_EQ(ctx.sceneRendered, -1);
}

TEST_F(RenderContextTest, DefaultSceneType) {
    RenderContext ctx;
    EXPECT_EQ(ctx.sceneType, SceneType::GAME);
}

TEST_F(RenderContextTest, DefaultIsChildWindow) {
    RenderContext ctx;
    EXPECT_FALSE(ctx.isChildWindow);
}

TEST_F(RenderContextTest, DefaultViewportBounds) {
    RenderContext ctx;
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, 0.0f);
}

TEST_F(RenderContextTest, DefaultCamerasEmpty) {
    RenderContext ctx;
    EXPECT_TRUE(ctx.cameras.empty());
}

TEST_F(RenderContextTest, DefaultGridParamsInitialized) {
    RenderContext ctx;
    EXPECT_TRUE(ctx.gridParams.enabled);
    EXPECT_FLOAT_EQ(ctx.gridParams.gridSize, 100.0f);
}

// =============================================================================
// Reset Tests
// =============================================================================

TEST_F(RenderContextTest, ResetClearsSceneRendered) {
    RenderContext ctx;
    ctx.sceneRendered = 5;
    ctx.reset();
    EXPECT_EQ(ctx.sceneRendered, -1);
}

TEST_F(RenderContextTest, ResetClearsIsChildWindow) {
    RenderContext ctx;
    ctx.isChildWindow = true;
    ctx.reset();
    EXPECT_FALSE(ctx.isChildWindow);
}

TEST_F(RenderContextTest, ResetClearsViewportBounds) {
    RenderContext ctx;
    ctx.viewportBounds[0] = glm::vec2{100.0f, 200.0f};
    ctx.viewportBounds[1] = glm::vec2{300.0f, 400.0f};
    ctx.reset();
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, 0.0f);
}

TEST_F(RenderContextTest, ResetClearsCameras) {
    RenderContext ctx;
    ctx.cameras.push_back(CameraContext{});
    ctx.reset();
    EXPECT_TRUE(ctx.cameras.empty());
}

TEST_F(RenderContextTest, ResetClearsSceneLightsAmbient) {
    RenderContext ctx;
    ctx.sceneLights.ambientLight = glm::vec3(1.0f, 0.5f, 0.25f);
    ctx.reset();
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.r, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.g, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.b, 0.0f);
}

TEST_F(RenderContextTest, ResetClearsLightCounts) {
    RenderContext ctx;
    ctx.sceneLights.pointLightCount = 5;
    ctx.sceneLights.spotLightCount = 3;
    ctx.reset();
    EXPECT_EQ(ctx.sceneLights.pointLightCount, 0);
    EXPECT_EQ(ctx.sceneLights.spotLightCount, 0);
}

// =============================================================================
// Type Traits Tests
// =============================================================================

TEST_F(RenderContextTest, GridParamsIsDefaultConstructible) {
    EXPECT_TRUE(std::is_default_constructible_v<RenderContext::GridParams>);
}

TEST_F(RenderContextTest, GridParamsIsCopyConstructible) {
    EXPECT_TRUE(std::is_copy_constructible_v<RenderContext::GridParams>);
}

TEST_F(RenderContextTest, GridParamsIsCopyAssignable) {
    EXPECT_TRUE(std::is_copy_assignable_v<RenderContext::GridParams>);
}

TEST_F(RenderContextTest, RenderContextIsDefaultConstructible) {
    EXPECT_TRUE(std::is_default_constructible_v<RenderContext>);
}

TEST_F(RenderContextTest, RenderContextIsNotCopyConstructible) {
    // RenderContext has deleted copy constructor for singleton semantics
    EXPECT_FALSE(std::is_copy_constructible_v<RenderContext>);
}

TEST_F(RenderContextTest, RenderContextIsNotCopyAssignable) {
    EXPECT_FALSE(std::is_copy_assignable_v<RenderContext>);
}

TEST_F(RenderContextTest, RenderContextIsMoveConstructible) {
    EXPECT_TRUE(std::is_move_constructible_v<RenderContext>);
}

// =============================================================================
// GridParams Copy Tests
// =============================================================================

TEST_F(RenderContextTest, GridParamsCopyConstruction) {
    RenderContext::GridParams original;
    original.enabled = false;
    original.gridSize = 50.0f;
    original.cellSize = 0.5f;

    RenderContext::GridParams copy = original;

    EXPECT_EQ(copy.enabled, original.enabled);
    EXPECT_FLOAT_EQ(copy.gridSize, original.gridSize);
    EXPECT_FLOAT_EQ(copy.cellSize, original.cellSize);
}

TEST_F(RenderContextTest, GridParamsCopyAssignment) {
    RenderContext::GridParams original;
    original.enabled = false;
    original.minPixelsBetweenCells = 10.0f;

    RenderContext::GridParams copy;
    copy = original;

    EXPECT_EQ(copy.enabled, original.enabled);
    EXPECT_FLOAT_EQ(copy.minPixelsBetweenCells, original.minPixelsBetweenCells);
}

}  // namespace nexo::components
