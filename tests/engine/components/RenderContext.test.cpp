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

// =============================================================================
// RenderContext Field Modification Tests
// =============================================================================

TEST_F(RenderContextTest, ModifySceneRendered) {
    RenderContext ctx;
    ctx.sceneRendered = 42;
    EXPECT_EQ(ctx.sceneRendered, 42);
}

TEST_F(RenderContextTest, ModifySceneType) {
    RenderContext ctx;
    ctx.sceneType = SceneType::EDITOR;
    EXPECT_EQ(ctx.sceneType, SceneType::EDITOR);
}

TEST_F(RenderContextTest, ModifyIsChildWindow) {
    RenderContext ctx;
    ctx.isChildWindow = true;
    EXPECT_TRUE(ctx.isChildWindow);
}

TEST_F(RenderContextTest, ModifyViewportBoundsFirstElement) {
    RenderContext ctx;
    ctx.viewportBounds[0] = glm::vec2{1920.0f, 1080.0f};
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, 1920.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, 1080.0f);
}

TEST_F(RenderContextTest, ModifyViewportBoundsSecondElement) {
    RenderContext ctx;
    ctx.viewportBounds[1] = glm::vec2{800.0f, 600.0f};
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, 800.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, 600.0f);
}

TEST_F(RenderContextTest, ModifyBothViewportBounds) {
    RenderContext ctx;
    ctx.viewportBounds[0] = glm::vec2{100.0f, 50.0f};
    ctx.viewportBounds[1] = glm::vec2{1000.0f, 500.0f};

    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, 100.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, 50.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, 1000.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, 500.0f);
}

// =============================================================================
// Edge Cases - Viewport Bounds
// =============================================================================

TEST_F(RenderContextTest, ViewportBoundsZeroValues) {
    RenderContext ctx;
    ctx.viewportBounds[0] = glm::vec2{0.0f, 0.0f};
    ctx.viewportBounds[1] = glm::vec2{0.0f, 0.0f};

    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, 0.0f);
}

TEST_F(RenderContextTest, ViewportBoundsNegativeValues) {
    RenderContext ctx;
    ctx.viewportBounds[0] = glm::vec2{-100.0f, -50.0f};
    ctx.viewportBounds[1] = glm::vec2{-200.0f, -150.0f};

    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, -100.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, -50.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, -200.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, -150.0f);
}

TEST_F(RenderContextTest, ViewportBoundsMixedPositiveNegative) {
    RenderContext ctx;
    ctx.viewportBounds[0] = glm::vec2{-50.0f, 25.0f};
    ctx.viewportBounds[1] = glm::vec2{150.0f, -75.0f};

    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, -50.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, 25.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, 150.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, -75.0f);
}

TEST_F(RenderContextTest, ViewportBoundsLargeValues) {
    RenderContext ctx;
    ctx.viewportBounds[0] = glm::vec2{10000.0f, 8000.0f};
    ctx.viewportBounds[1] = glm::vec2{99999.0f, 77777.0f};

    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, 10000.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, 8000.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, 99999.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, 77777.0f);
}

TEST_F(RenderContextTest, ViewportBoundsVerySmallValues) {
    RenderContext ctx;
    ctx.viewportBounds[0] = glm::vec2{0.001f, 0.002f};
    ctx.viewportBounds[1] = glm::vec2{0.003f, 0.004f};

    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, 0.001f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, 0.002f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, 0.003f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, 0.004f);
}

// =============================================================================
// Edge Cases - Scene Rendered
// =============================================================================

TEST_F(RenderContextTest, SceneRenderedZero) {
    RenderContext ctx;
    ctx.sceneRendered = 0;
    EXPECT_EQ(ctx.sceneRendered, 0);
}

TEST_F(RenderContextTest, SceneRenderedNegativeValues) {
    RenderContext ctx;
    ctx.sceneRendered = -999;
    EXPECT_EQ(ctx.sceneRendered, -999);
}

TEST_F(RenderContextTest, SceneRenderedLargePositive) {
    RenderContext ctx;
    ctx.sceneRendered = 1000000;
    EXPECT_EQ(ctx.sceneRendered, 1000000);
}

// =============================================================================
// Reset Method - Comprehensive Tests
// =============================================================================

TEST_F(RenderContextTest, ResetAfterFullModification) {
    RenderContext ctx;

    // Modify all fields
    ctx.sceneRendered = 100;
    ctx.isChildWindow = true;
    ctx.viewportBounds[0] = glm::vec2{123.0f, 456.0f};
    ctx.viewportBounds[1] = glm::vec2{789.0f, 1011.0f};
    ctx.cameras.push_back(CameraContext{});
    ctx.cameras.push_back(CameraContext{});
    ctx.sceneLights.ambientLight = glm::vec3(0.5f, 0.5f, 0.5f);
    ctx.sceneLights.pointLightCount = 10;
    ctx.sceneLights.spotLightCount = 5;

    // Reset
    ctx.reset();

    // Verify all fields are reset
    EXPECT_EQ(ctx.sceneRendered, -1);
    EXPECT_FALSE(ctx.isChildWindow);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, 0.0f);
    EXPECT_TRUE(ctx.cameras.empty());
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.r, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.g, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.b, 0.0f);
    EXPECT_EQ(ctx.sceneLights.pointLightCount, 0);
    EXPECT_EQ(ctx.sceneLights.spotLightCount, 0);
}

TEST_F(RenderContextTest, ResetMultipleTimes) {
    RenderContext ctx;
    ctx.sceneRendered = 5;
    ctx.reset();
    EXPECT_EQ(ctx.sceneRendered, -1);

    ctx.sceneRendered = 10;
    ctx.reset();
    EXPECT_EQ(ctx.sceneRendered, -1);

    ctx.sceneRendered = 15;
    ctx.reset();
    EXPECT_EQ(ctx.sceneRendered, -1);
}

TEST_F(RenderContextTest, ResetWithMultipleCameras) {
    RenderContext ctx;
    ctx.cameras.push_back(CameraContext{});
    ctx.cameras.push_back(CameraContext{});
    ctx.cameras.push_back(CameraContext{});

    EXPECT_EQ(ctx.cameras.size(), 3);

    ctx.reset();
    EXPECT_TRUE(ctx.cameras.empty());
    EXPECT_EQ(ctx.cameras.size(), 0);
}

TEST_F(RenderContextTest, ResetDirectionalLight) {
    RenderContext ctx;
    ctx.sceneLights.dirLight.direction = glm::vec3(1.0f, 2.0f, 3.0f);
    ctx.sceneLights.dirLight.color = glm::vec3(0.8f, 0.6f, 0.4f);

    ctx.reset();

    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.y, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.z, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.r, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.g, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.b, 0.0f);
}

// =============================================================================
// Default Initialization - LightContext
// =============================================================================

TEST_F(RenderContextTest, DefaultSceneLightsAmbient) {
    RenderContext ctx;
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.r, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.g, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.b, 0.0f);
}

TEST_F(RenderContextTest, DefaultPointLightCount) {
    RenderContext ctx;
    EXPECT_EQ(ctx.sceneLights.pointLightCount, 0);
}

TEST_F(RenderContextTest, DefaultSpotLightCount) {
    RenderContext ctx;
    EXPECT_EQ(ctx.sceneLights.spotLightCount, 0);
}

TEST_F(RenderContextTest, DefaultDirectionalLight) {
    RenderContext ctx;
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.y, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.z, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.r, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.g, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.b, 0.0f);
}

// =============================================================================
// SceneType Enum Tests
// =============================================================================

TEST_F(RenderContextTest, SceneTypeGameToEditor) {
    RenderContext ctx;
    EXPECT_EQ(ctx.sceneType, SceneType::GAME);
    ctx.sceneType = SceneType::EDITOR;
    EXPECT_EQ(ctx.sceneType, SceneType::EDITOR);
}

TEST_F(RenderContextTest, SceneTypeEditorToGame) {
    RenderContext ctx;
    ctx.sceneType = SceneType::EDITOR;
    ctx.sceneType = SceneType::GAME;
    EXPECT_EQ(ctx.sceneType, SceneType::GAME);
}

// =============================================================================
// GridParams Edge Cases
// =============================================================================

TEST_F(RenderContextTest, GridParamsZeroGridSize) {
    RenderContext::GridParams params;
    params.gridSize = 0.0f;
    EXPECT_FLOAT_EQ(params.gridSize, 0.0f);
}

TEST_F(RenderContextTest, GridParamsNegativeGridSize) {
    RenderContext::GridParams params;
    params.gridSize = -50.0f;
    EXPECT_FLOAT_EQ(params.gridSize, -50.0f);
}

TEST_F(RenderContextTest, GridParamsVeryLargeGridSize) {
    RenderContext::GridParams params;
    params.gridSize = 999999.0f;
    EXPECT_FLOAT_EQ(params.gridSize, 999999.0f);
}

TEST_F(RenderContextTest, GridParamsZeroCellSize) {
    RenderContext::GridParams params;
    params.cellSize = 0.0f;
    EXPECT_FLOAT_EQ(params.cellSize, 0.0f);
}

TEST_F(RenderContextTest, GridParamsNegativeCellSize) {
    RenderContext::GridParams params;
    params.cellSize = -0.1f;
    EXPECT_FLOAT_EQ(params.cellSize, -0.1f);
}

TEST_F(RenderContextTest, GridParamsZeroMinPixels) {
    RenderContext::GridParams params;
    params.minPixelsBetweenCells = 0.0f;
    EXPECT_FLOAT_EQ(params.minPixelsBetweenCells, 0.0f);
}

TEST_F(RenderContextTest, GridParamsNegativeMinPixels) {
    RenderContext::GridParams params;
    params.minPixelsBetweenCells = -5.0f;
    EXPECT_FLOAT_EQ(params.minPixelsBetweenCells, -5.0f);
}

// =============================================================================
// Integration Tests - Combined Field Modifications
// =============================================================================

TEST_F(RenderContextTest, ModifyAllFieldsWithoutReset) {
    RenderContext ctx;

    ctx.sceneRendered = 99;
    ctx.sceneType = SceneType::EDITOR;
    ctx.isChildWindow = true;
    ctx.viewportBounds[0] = glm::vec2{10.0f, 20.0f};
    ctx.viewportBounds[1] = glm::vec2{30.0f, 40.0f};
    ctx.gridParams.enabled = false;
    ctx.gridParams.gridSize = 200.0f;
    ctx.gridParams.cellSize = 0.1f;
    ctx.gridParams.minPixelsBetweenCells = 5.0f;

    EXPECT_EQ(ctx.sceneRendered, 99);
    EXPECT_EQ(ctx.sceneType, SceneType::EDITOR);
    EXPECT_TRUE(ctx.isChildWindow);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, 10.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, 20.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, 30.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, 40.0f);
    EXPECT_FALSE(ctx.gridParams.enabled);
    EXPECT_FLOAT_EQ(ctx.gridParams.gridSize, 200.0f);
    EXPECT_FLOAT_EQ(ctx.gridParams.cellSize, 0.1f);
    EXPECT_FLOAT_EQ(ctx.gridParams.minPixelsBetweenCells, 5.0f);
}

TEST_F(RenderContextTest, ResetPreservesGridParams) {
    RenderContext ctx;
    ctx.gridParams.enabled = false;
    ctx.gridParams.gridSize = 500.0f;
    ctx.sceneRendered = 42;

    ctx.reset();

    // Reset should not modify gridParams (not mentioned in reset() method)
    EXPECT_FALSE(ctx.gridParams.enabled);
    EXPECT_FLOAT_EQ(ctx.gridParams.gridSize, 500.0f);

    // But should reset other fields
    EXPECT_EQ(ctx.sceneRendered, -1);
}

TEST_F(RenderContextTest, ViewportBoundsIndependence) {
    RenderContext ctx;
    ctx.viewportBounds[0] = glm::vec2{100.0f, 200.0f};

    // Modifying one shouldn't affect the other
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, 0.0f);

    ctx.viewportBounds[1] = glm::vec2{300.0f, 400.0f};

    // First should remain unchanged
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, 100.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, 200.0f);
}

// =============================================================================
// Camera Vector Operations Tests
// =============================================================================

TEST_F(RenderContextTest, CamerasPushBackSingleCamera) {
    RenderContext ctx;
    CameraContext camera;
    camera.cameraPosition = glm::vec3(1.0f, 2.0f, 3.0f);
    camera.clearColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    ctx.cameras.push_back(camera);

    EXPECT_EQ(ctx.cameras.size(), 1);
    EXPECT_FLOAT_EQ(ctx.cameras[0].cameraPosition.x, 1.0f);
    EXPECT_FLOAT_EQ(ctx.cameras[0].cameraPosition.y, 2.0f);
    EXPECT_FLOAT_EQ(ctx.cameras[0].cameraPosition.z, 3.0f);
}

TEST_F(RenderContextTest, CamerasPushBackMultipleCameras) {
    RenderContext ctx;

    CameraContext cam1;
    cam1.cameraPosition = glm::vec3(1.0f, 1.0f, 1.0f);
    CameraContext cam2;
    cam2.cameraPosition = glm::vec3(2.0f, 2.0f, 2.0f);
    CameraContext cam3;
    cam3.cameraPosition = glm::vec3(3.0f, 3.0f, 3.0f);

    ctx.cameras.push_back(cam1);
    ctx.cameras.push_back(cam2);
    ctx.cameras.push_back(cam3);

    EXPECT_EQ(ctx.cameras.size(), 3);
    EXPECT_FLOAT_EQ(ctx.cameras[0].cameraPosition.x, 1.0f);
    EXPECT_FLOAT_EQ(ctx.cameras[1].cameraPosition.x, 2.0f);
    EXPECT_FLOAT_EQ(ctx.cameras[2].cameraPosition.x, 3.0f);
}

TEST_F(RenderContextTest, CamerasAccessByIndex) {
    RenderContext ctx;

    for (int i = 0; i < 5; ++i) {
        CameraContext cam;
        cam.cameraPosition = glm::vec3(static_cast<float>(i), 0.0f, 0.0f);
        ctx.cameras.push_back(cam);
    }

    EXPECT_EQ(ctx.cameras.size(), 5);
    EXPECT_FLOAT_EQ(ctx.cameras[2].cameraPosition.x, 2.0f);
    EXPECT_FLOAT_EQ(ctx.cameras[4].cameraPosition.x, 4.0f);
}

TEST_F(RenderContextTest, CamerasClearManually) {
    RenderContext ctx;
    ctx.cameras.push_back(CameraContext{});
    ctx.cameras.push_back(CameraContext{});
    ctx.cameras.push_back(CameraContext{});

    EXPECT_EQ(ctx.cameras.size(), 3);

    ctx.cameras.clear();

    EXPECT_TRUE(ctx.cameras.empty());
    EXPECT_EQ(ctx.cameras.size(), 0);
}

TEST_F(RenderContextTest, CamerasEmplaceBack) {
    RenderContext ctx;

    ctx.cameras.emplace_back();
    ctx.cameras.back().cameraPosition = glm::vec3(10.0f, 20.0f, 30.0f);

    EXPECT_EQ(ctx.cameras.size(), 1);
    EXPECT_FLOAT_EQ(ctx.cameras[0].cameraPosition.x, 10.0f);
    EXPECT_FLOAT_EQ(ctx.cameras[0].cameraPosition.y, 20.0f);
    EXPECT_FLOAT_EQ(ctx.cameras[0].cameraPosition.z, 30.0f);
}

TEST_F(RenderContextTest, CamerasResize) {
    RenderContext ctx;
    ctx.cameras.resize(5);

    EXPECT_EQ(ctx.cameras.size(), 5);
}

TEST_F(RenderContextTest, CamerasPopBack) {
    RenderContext ctx;
    ctx.cameras.push_back(CameraContext{});
    ctx.cameras.push_back(CameraContext{});
    ctx.cameras.push_back(CameraContext{});

    EXPECT_EQ(ctx.cameras.size(), 3);

    ctx.cameras.pop_back();

    EXPECT_EQ(ctx.cameras.size(), 2);
}

// =============================================================================
// LightContext Integration Tests
// =============================================================================

TEST_F(RenderContextTest, LightContextPointLightsArray) {
    RenderContext ctx;

    // Verify array is default initialized
    EXPECT_EQ(ctx.sceneLights.pointLights.size(), 10);
}

TEST_F(RenderContextTest, LightContextSpotLightsArray) {
    RenderContext ctx;

    // Verify array is default initialized
    EXPECT_EQ(ctx.sceneLights.spotLights.size(), 10);
}

TEST_F(RenderContextTest, LightContextSetPointLightCount) {
    RenderContext ctx;
    ctx.sceneLights.pointLightCount = 5;

    EXPECT_EQ(ctx.sceneLights.pointLightCount, 5);
}

TEST_F(RenderContextTest, LightContextSetSpotLightCount) {
    RenderContext ctx;
    ctx.sceneLights.spotLightCount = 7;

    EXPECT_EQ(ctx.sceneLights.spotLightCount, 7);
}

TEST_F(RenderContextTest, LightContextMaxPointLights) {
    RenderContext ctx;
    ctx.sceneLights.pointLightCount = 10;

    EXPECT_EQ(ctx.sceneLights.pointLightCount, 10);
}

TEST_F(RenderContextTest, LightContextMaxSpotLights) {
    RenderContext ctx;
    ctx.sceneLights.spotLightCount = 10;

    EXPECT_EQ(ctx.sceneLights.spotLightCount, 10);
}

TEST_F(RenderContextTest, LightContextModifyAmbientLight) {
    RenderContext ctx;
    ctx.sceneLights.ambientLight = glm::vec3(0.2f, 0.4f, 0.6f);

    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.r, 0.2f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.g, 0.4f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.b, 0.6f);
}

TEST_F(RenderContextTest, LightContextModifyDirectionalLightDirection) {
    RenderContext ctx;
    ctx.sceneLights.dirLight.direction = glm::vec3(1.0f, -1.0f, 0.0f);

    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.x, 1.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.y, -1.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.z, 0.0f);
}

TEST_F(RenderContextTest, LightContextModifyDirectionalLightColor) {
    RenderContext ctx;
    ctx.sceneLights.dirLight.color = glm::vec3(1.0f, 0.8f, 0.6f);

    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.r, 1.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.g, 0.8f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.b, 0.6f);
}

TEST_F(RenderContextTest, LightContextFullDirectionalLightSetup) {
    RenderContext ctx;
    DirectionalLightComponent dirLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.9f));
    ctx.sceneLights.dirLight = dirLight;

    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.y, -1.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.z, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.r, 1.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.g, 1.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.b, 0.9f);
}

TEST_F(RenderContextTest, ResetClearsAllLightContextFields) {
    RenderContext ctx;

    // Setup all light fields
    ctx.sceneLights.ambientLight = glm::vec3(0.5f, 0.5f, 0.5f);
    ctx.sceneLights.pointLightCount = 8;
    ctx.sceneLights.spotLightCount = 6;
    ctx.sceneLights.dirLight.direction = glm::vec3(1.0f, 1.0f, 1.0f);
    ctx.sceneLights.dirLight.color = glm::vec3(0.8f, 0.8f, 0.8f);

    ctx.reset();

    // Verify all are reset
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.r, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.g, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.b, 0.0f);
    EXPECT_EQ(ctx.sceneLights.pointLightCount, 0);
    EXPECT_EQ(ctx.sceneLights.spotLightCount, 0);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.y, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.z, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.r, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.g, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.b, 0.0f);
}

// =============================================================================
// Move Constructor Tests
// =============================================================================

TEST_F(RenderContextTest, MoveConstructorTransfersSceneRendered) {
    RenderContext original;
    original.sceneRendered = 42;

    RenderContext moved(std::move(original));

    EXPECT_EQ(moved.sceneRendered, 42);
}

TEST_F(RenderContextTest, MoveConstructorTransfersCameras) {
    RenderContext original;
    CameraContext cam1;
    cam1.cameraPosition = glm::vec3(1.0f, 2.0f, 3.0f);
    CameraContext cam2;
    cam2.cameraPosition = glm::vec3(4.0f, 5.0f, 6.0f);

    original.cameras.push_back(cam1);
    original.cameras.push_back(cam2);

    RenderContext moved(std::move(original));

    EXPECT_EQ(moved.cameras.size(), 2);
    EXPECT_FLOAT_EQ(moved.cameras[0].cameraPosition.x, 1.0f);
    EXPECT_FLOAT_EQ(moved.cameras[0].cameraPosition.y, 2.0f);
    EXPECT_FLOAT_EQ(moved.cameras[0].cameraPosition.z, 3.0f);
    EXPECT_FLOAT_EQ(moved.cameras[1].cameraPosition.x, 4.0f);
}

TEST_F(RenderContextTest, MoveConstructorTransfersSceneLights) {
    RenderContext original;
    original.sceneLights.ambientLight = glm::vec3(0.3f, 0.4f, 0.5f);
    original.sceneLights.pointLightCount = 5;
    original.sceneLights.spotLightCount = 3;
    original.sceneLights.dirLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    original.sceneLights.dirLight.color = glm::vec3(1.0f, 0.9f, 0.8f);

    RenderContext moved(std::move(original));

    EXPECT_FLOAT_EQ(moved.sceneLights.ambientLight.r, 0.3f);
    EXPECT_FLOAT_EQ(moved.sceneLights.ambientLight.g, 0.4f);
    EXPECT_FLOAT_EQ(moved.sceneLights.ambientLight.b, 0.5f);
    EXPECT_EQ(moved.sceneLights.pointLightCount, 5);
    EXPECT_EQ(moved.sceneLights.spotLightCount, 3);
    EXPECT_FLOAT_EQ(moved.sceneLights.dirLight.direction.x, 0.0f);
    EXPECT_FLOAT_EQ(moved.sceneLights.dirLight.direction.y, -1.0f);
    EXPECT_FLOAT_EQ(moved.sceneLights.dirLight.direction.z, 0.0f);
    EXPECT_FLOAT_EQ(moved.sceneLights.dirLight.color.r, 1.0f);
    EXPECT_FLOAT_EQ(moved.sceneLights.dirLight.color.g, 0.9f);
    EXPECT_FLOAT_EQ(moved.sceneLights.dirLight.color.b, 0.8f);
}

TEST_F(RenderContextTest, MoveConstructorEmptyCameras) {
    RenderContext original;
    // Don't add any cameras

    RenderContext moved(std::move(original));

    EXPECT_TRUE(moved.cameras.empty());
}

TEST_F(RenderContextTest, MoveConstructorWithMultipleCameras) {
    RenderContext original;

    for (int i = 0; i < 10; ++i) {
        CameraContext cam;
        cam.cameraPosition = glm::vec3(static_cast<float>(i), 0.0f, 0.0f);
        original.cameras.push_back(cam);
    }

    RenderContext moved(std::move(original));

    EXPECT_EQ(moved.cameras.size(), 10);
    for (int i = 0; i < 10; ++i) {
        EXPECT_FLOAT_EQ(moved.cameras[i].cameraPosition.x, static_cast<float>(i));
    }
}

// =============================================================================
// Additional Edge Cases
// =============================================================================

TEST_F(RenderContextTest, SceneRenderedIntMinValue) {
    RenderContext ctx;
    ctx.sceneRendered = std::numeric_limits<int>::min();
    EXPECT_EQ(ctx.sceneRendered, std::numeric_limits<int>::min());
}

TEST_F(RenderContextTest, SceneRenderedIntMaxValue) {
    RenderContext ctx;
    ctx.sceneRendered = std::numeric_limits<int>::max();
    EXPECT_EQ(ctx.sceneRendered, std::numeric_limits<int>::max());
}

TEST_F(RenderContextTest, ViewportBoundsFloatMax) {
    RenderContext ctx;
    ctx.viewportBounds[0] = glm::vec2{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};

    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, std::numeric_limits<float>::max());
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, std::numeric_limits<float>::max());
}

TEST_F(RenderContextTest, ViewportBoundsFloatMin) {
    RenderContext ctx;
    ctx.viewportBounds[0] = glm::vec2{std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};

    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, std::numeric_limits<float>::lowest());
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, std::numeric_limits<float>::lowest());
}

TEST_F(RenderContextTest, GridParamsFloatPrecision) {
    RenderContext::GridParams params;
    params.gridSize = 0.0001f;
    params.cellSize = 0.00001f;
    params.minPixelsBetweenCells = 0.000001f;

    EXPECT_FLOAT_EQ(params.gridSize, 0.0001f);
    EXPECT_FLOAT_EQ(params.cellSize, 0.00001f);
    EXPECT_FLOAT_EQ(params.minPixelsBetweenCells, 0.000001f);
}

TEST_F(RenderContextTest, AmbientLightZeroVector) {
    RenderContext ctx;
    ctx.sceneLights.ambientLight = glm::vec3(0.0f, 0.0f, 0.0f);

    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.r, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.g, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.b, 0.0f);
}

TEST_F(RenderContextTest, AmbientLightMaxValues) {
    RenderContext ctx;
    ctx.sceneLights.ambientLight = glm::vec3(1.0f, 1.0f, 1.0f);

    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.r, 1.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.g, 1.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.b, 1.0f);
}

TEST_F(RenderContextTest, AmbientLightAboveMaxValues) {
    RenderContext ctx;
    ctx.sceneLights.ambientLight = glm::vec3(2.0f, 5.0f, 10.0f);

    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.r, 2.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.g, 5.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.b, 10.0f);
}

TEST_F(RenderContextTest, AmbientLightNegativeValues) {
    RenderContext ctx;
    ctx.sceneLights.ambientLight = glm::vec3(-0.5f, -1.0f, -2.0f);

    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.r, -0.5f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.g, -1.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.b, -2.0f);
}

TEST_F(RenderContextTest, LightCountsZero) {
    RenderContext ctx;
    ctx.sceneLights.pointLightCount = 0;
    ctx.sceneLights.spotLightCount = 0;

    EXPECT_EQ(ctx.sceneLights.pointLightCount, 0);
    EXPECT_EQ(ctx.sceneLights.spotLightCount, 0);
}

TEST_F(RenderContextTest, LightCountsAboveMax) {
    RenderContext ctx;
    ctx.sceneLights.pointLightCount = 100;
    ctx.sceneLights.spotLightCount = 50;

    EXPECT_EQ(ctx.sceneLights.pointLightCount, 100);
    EXPECT_EQ(ctx.sceneLights.spotLightCount, 50);
}

// =============================================================================
// Complex Interaction Tests
// =============================================================================

TEST_F(RenderContextTest, ResetDoesNotAffectSceneType) {
    RenderContext ctx;
    ctx.sceneType = SceneType::EDITOR;

    ctx.reset();

    // sceneType is not reset in the reset() method
    EXPECT_EQ(ctx.sceneType, SceneType::EDITOR);
}

TEST_F(RenderContextTest, MultipleFieldModificationsWithCameras) {
    RenderContext ctx;

    ctx.sceneRendered = 100;
    ctx.isChildWindow = true;
    ctx.viewportBounds[0] = glm::vec2{50.0f, 50.0f};
    ctx.viewportBounds[1] = glm::vec2{1920.0f, 1080.0f};

    CameraContext cam;
    cam.cameraPosition = glm::vec3(0.0f, 5.0f, -10.0f);
    ctx.cameras.push_back(cam);

    ctx.sceneLights.ambientLight = glm::vec3(0.2f, 0.2f, 0.2f);
    ctx.sceneLights.pointLightCount = 3;

    EXPECT_EQ(ctx.sceneRendered, 100);
    EXPECT_TRUE(ctx.isChildWindow);
    EXPECT_EQ(ctx.cameras.size(), 1);
    EXPECT_FLOAT_EQ(ctx.cameras[0].cameraPosition.y, 5.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.r, 0.2f);
    EXPECT_EQ(ctx.sceneLights.pointLightCount, 3);
}

TEST_F(RenderContextTest, ResetAfterComplexSetup) {
    RenderContext ctx;

    // Complex setup
    ctx.sceneRendered = 999;
    ctx.isChildWindow = true;
    ctx.viewportBounds[0] = glm::vec2{100.0f, 100.0f};
    ctx.viewportBounds[1] = glm::vec2{1000.0f, 1000.0f};

    for (int i = 0; i < 5; ++i) {
        CameraContext cam;
        cam.cameraPosition = glm::vec3(static_cast<float>(i), 0.0f, 0.0f);
        ctx.cameras.push_back(cam);
    }

    ctx.sceneLights.ambientLight = glm::vec3(1.0f, 1.0f, 1.0f);
    ctx.sceneLights.pointLightCount = 10;
    ctx.sceneLights.spotLightCount = 10;
    ctx.sceneLights.dirLight.direction = glm::vec3(1.0f, 1.0f, 1.0f);
    ctx.sceneLights.dirLight.color = glm::vec3(1.0f, 1.0f, 1.0f);

    // Reset
    ctx.reset();

    // Verify everything is reset
    EXPECT_EQ(ctx.sceneRendered, -1);
    EXPECT_FALSE(ctx.isChildWindow);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[0].y, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.viewportBounds[1].y, 0.0f);
    EXPECT_TRUE(ctx.cameras.empty());
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.r, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.g, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.ambientLight.b, 0.0f);
    EXPECT_EQ(ctx.sceneLights.pointLightCount, 0);
    EXPECT_EQ(ctx.sceneLights.spotLightCount, 0);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.x, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.y, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.direction.z, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.r, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.g, 0.0f);
    EXPECT_FLOAT_EQ(ctx.sceneLights.dirLight.color.b, 0.0f);
}

TEST_F(RenderContextTest, ConsecutiveResetsIdempotent) {
    RenderContext ctx;
    ctx.sceneRendered = 100;
    ctx.cameras.push_back(CameraContext{});

    ctx.reset();
    EXPECT_EQ(ctx.sceneRendered, -1);
    EXPECT_TRUE(ctx.cameras.empty());

    ctx.reset();
    EXPECT_EQ(ctx.sceneRendered, -1);
    EXPECT_TRUE(ctx.cameras.empty());

    ctx.reset();
    EXPECT_EQ(ctx.sceneRendered, -1);
    EXPECT_TRUE(ctx.cameras.empty());
}

TEST_F(RenderContextTest, GridParamsIndependentOfReset) {
    RenderContext ctx;

    // Modify grid params
    ctx.gridParams.enabled = false;
    ctx.gridParams.gridSize = 250.0f;
    ctx.gridParams.cellSize = 0.5f;
    ctx.gridParams.minPixelsBetweenCells = 10.0f;

    // Modify other fields
    ctx.sceneRendered = 50;
    ctx.cameras.push_back(CameraContext{});

    // Reset
    ctx.reset();

    // Grid params should not be affected by reset
    EXPECT_FALSE(ctx.gridParams.enabled);
    EXPECT_FLOAT_EQ(ctx.gridParams.gridSize, 250.0f);
    EXPECT_FLOAT_EQ(ctx.gridParams.cellSize, 0.5f);
    EXPECT_FLOAT_EQ(ctx.gridParams.minPixelsBetweenCells, 10.0f);

    // Other fields should be reset
    EXPECT_EQ(ctx.sceneRendered, -1);
    EXPECT_TRUE(ctx.cameras.empty());
}

}  // namespace nexo::components
