//// Projection.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for math projection utilities
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/epsilon.hpp>
#include <cmath>
#include "math/Projection.hpp"

namespace nexo::math {

class ProjectionTest : public ::testing::Test {
protected:
    // Helper to compare vec3 with epsilon
    static bool compareVec3(const glm::vec3& a, const glm::vec3& b, float epsilon = 0.001f) {
        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }

    // Standard test setup: camera at origin looking down -Z
    glm::mat4 createViewProjection(const glm::vec3& cameraPos = glm::vec3(0.0f),
                                   const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f),
                                   float fov = 45.0f,
                                   float aspect = 1.0f,
                                   float nearPlane = 0.1f,
                                   float farPlane = 100.0f) {
        glm::mat4 view = glm::lookAt(cameraPos, target, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
        return proj * view;
    }

    static constexpr unsigned int WIDTH = 800;
    static constexpr unsigned int HEIGHT = 600;
};

// =============================================================================
// Basic Ray Direction Tests
// =============================================================================

TEST_F(ProjectionTest, CenterRayPointsForward) {
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    glm::mat4 viewProj = createViewProjection(cameraPos);

    // Center of screen should point forward (-Z direction)
    glm::vec3 ray = projectRayToWorld(WIDTH / 2.0f, HEIGHT / 2.0f, viewProj, cameraPos, WIDTH, HEIGHT);

    // Ray should be normalized
    EXPECT_NEAR(glm::length(ray), 1.0f, 0.001f);

    // Center ray should point roughly in -Z direction
    EXPECT_NEAR(ray.z, -1.0f, 0.1f);
    EXPECT_NEAR(ray.x, 0.0f, 0.1f);
    EXPECT_NEAR(ray.y, 0.0f, 0.1f);
}

TEST_F(ProjectionTest, RayIsNormalized) {
    glm::vec3 cameraPos(5.0f, 3.0f, 10.0f);
    glm::mat4 viewProj = createViewProjection(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f));

    // Test various screen positions
    std::vector<std::pair<float, float>> positions = {
        {0.0f, 0.0f},
        {WIDTH, 0.0f},
        {0.0f, HEIGHT},
        {WIDTH, HEIGHT},
        {WIDTH / 2.0f, HEIGHT / 2.0f},
        {WIDTH / 4.0f, HEIGHT / 4.0f}
    };

    for (const auto& [x, y] : positions) {
        glm::vec3 ray = projectRayToWorld(x, y, viewProj, cameraPos, WIDTH, HEIGHT);
        EXPECT_NEAR(glm::length(ray), 1.0f, 0.001f)
            << "Ray not normalized at (" << x << ", " << y << ")";
    }
}

TEST_F(ProjectionTest, TopLeftCornerRayDirection) {
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    glm::mat4 viewProj = createViewProjection(cameraPos);

    // Top-left corner (0, 0)
    glm::vec3 ray = projectRayToWorld(0.0f, 0.0f, viewProj, cameraPos, WIDTH, HEIGHT);

    // Should point up-left and forward
    EXPECT_LT(ray.x, 0.0f);  // Left
    EXPECT_GT(ray.y, 0.0f);  // Up
    EXPECT_LT(ray.z, 0.0f);  // Forward
}

TEST_F(ProjectionTest, BottomRightCornerRayDirection) {
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    glm::mat4 viewProj = createViewProjection(cameraPos);

    // Bottom-right corner
    glm::vec3 ray = projectRayToWorld(static_cast<float>(WIDTH), static_cast<float>(HEIGHT),
                                      viewProj, cameraPos, WIDTH, HEIGHT);

    // Should point down-right and forward
    EXPECT_GT(ray.x, 0.0f);  // Right
    EXPECT_LT(ray.y, 0.0f);  // Down
    EXPECT_LT(ray.z, 0.0f);  // Forward
}

// =============================================================================
// Camera Position Tests
// =============================================================================

TEST_F(ProjectionTest, OffsetCameraPosition) {
    glm::vec3 cameraPos(10.0f, 5.0f, 20.0f);
    glm::vec3 target(0.0f, 0.0f, 0.0f);
    glm::mat4 viewProj = createViewProjection(cameraPos, target);

    glm::vec3 ray = projectRayToWorld(WIDTH / 2.0f, HEIGHT / 2.0f, viewProj, cameraPos, WIDTH, HEIGHT);

    // Ray should be normalized regardless of camera position
    EXPECT_NEAR(glm::length(ray), 1.0f, 0.001f);

    // Center ray should point toward target (roughly)
    glm::vec3 expectedDir = glm::normalize(target - cameraPos);
    float dotProduct = glm::dot(ray, expectedDir);
    EXPECT_GT(dotProduct, 0.9f);  // Should be close to 1.0
}

TEST_F(ProjectionTest, NegativeCameraPosition) {
    glm::vec3 cameraPos(-5.0f, -3.0f, -10.0f);
    glm::vec3 target(0.0f, 0.0f, 0.0f);
    glm::mat4 viewProj = createViewProjection(cameraPos, target);

    glm::vec3 ray = projectRayToWorld(WIDTH / 2.0f, HEIGHT / 2.0f, viewProj, cameraPos, WIDTH, HEIGHT);

    EXPECT_NEAR(glm::length(ray), 1.0f, 0.001f);
}

// =============================================================================
// Screen Coordinate Tests
// =============================================================================

TEST_F(ProjectionTest, SymmetricRaysAroundCenter) {
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    glm::mat4 viewProj = createViewProjection(cameraPos);

    float centerX = WIDTH / 2.0f;
    float centerY = HEIGHT / 2.0f;
    float offset = 100.0f;

    // Get rays at symmetric positions
    glm::vec3 leftRay = projectRayToWorld(centerX - offset, centerY, viewProj, cameraPos, WIDTH, HEIGHT);
    glm::vec3 rightRay = projectRayToWorld(centerX + offset, centerY, viewProj, cameraPos, WIDTH, HEIGHT);

    // X components should be opposite, Y and Z should be similar
    EXPECT_NEAR(leftRay.x, -rightRay.x, 0.01f);
    EXPECT_NEAR(leftRay.y, rightRay.y, 0.01f);
    EXPECT_NEAR(leftRay.z, rightRay.z, 0.01f);
}

TEST_F(ProjectionTest, VerticalSymmetry) {
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    glm::mat4 viewProj = createViewProjection(cameraPos);

    float centerX = WIDTH / 2.0f;
    float centerY = HEIGHT / 2.0f;
    float offset = 100.0f;

    glm::vec3 topRay = projectRayToWorld(centerX, centerY - offset, viewProj, cameraPos, WIDTH, HEIGHT);
    glm::vec3 bottomRay = projectRayToWorld(centerX, centerY + offset, viewProj, cameraPos, WIDTH, HEIGHT);

    // Y components should be opposite, X and Z should be similar
    EXPECT_NEAR(topRay.x, bottomRay.x, 0.01f);
    EXPECT_NEAR(topRay.y, -bottomRay.y, 0.01f);
    EXPECT_NEAR(topRay.z, bottomRay.z, 0.01f);
}

// =============================================================================
// Different Viewport Sizes
// =============================================================================

TEST_F(ProjectionTest, SquareViewport) {
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    glm::mat4 viewProj = createViewProjection(cameraPos, glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f);

    const unsigned int size = 512;
    glm::vec3 ray = projectRayToWorld(size / 2.0f, size / 2.0f, viewProj, cameraPos, size, size);

    EXPECT_NEAR(glm::length(ray), 1.0f, 0.001f);
    EXPECT_NEAR(ray.z, -1.0f, 0.1f);
}

TEST_F(ProjectionTest, WideViewport) {
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    const unsigned int wideWidth = 1920;
    const unsigned int wideHeight = 1080;
    float aspect = static_cast<float>(wideWidth) / static_cast<float>(wideHeight);
    glm::mat4 viewProj = createViewProjection(cameraPos, glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, aspect);

    glm::vec3 ray = projectRayToWorld(wideWidth / 2.0f, wideHeight / 2.0f,
                                      viewProj, cameraPos, wideWidth, wideHeight);

    EXPECT_NEAR(glm::length(ray), 1.0f, 0.001f);
}

TEST_F(ProjectionTest, SmallViewport) {
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    const unsigned int smallWidth = 100;
    const unsigned int smallHeight = 100;
    glm::mat4 viewProj = createViewProjection(cameraPos, glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f);

    glm::vec3 ray = projectRayToWorld(smallWidth / 2.0f, smallHeight / 2.0f,
                                      viewProj, cameraPos, smallWidth, smallHeight);

    EXPECT_NEAR(glm::length(ray), 1.0f, 0.001f);
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST_F(ProjectionTest, ExtremeCorners) {
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    glm::mat4 viewProj = createViewProjection(cameraPos);

    // All four corners should produce valid normalized rays
    std::vector<std::pair<float, float>> corners = {
        {0.0f, 0.0f},
        {static_cast<float>(WIDTH), 0.0f},
        {0.0f, static_cast<float>(HEIGHT)},
        {static_cast<float>(WIDTH), static_cast<float>(HEIGHT)}
    };

    for (const auto& [x, y] : corners) {
        glm::vec3 ray = projectRayToWorld(x, y, viewProj, cameraPos, WIDTH, HEIGHT);
        EXPECT_NEAR(glm::length(ray), 1.0f, 0.001f)
            << "Invalid ray at corner (" << x << ", " << y << ")";
        EXPECT_FALSE(std::isnan(ray.x) || std::isnan(ray.y) || std::isnan(ray.z))
            << "NaN in ray at corner (" << x << ", " << y << ")";
    }
}

TEST_F(ProjectionTest, DifferentFOV) {
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);

    // Narrow FOV
    glm::mat4 narrowViewProj = createViewProjection(cameraPos, glm::vec3(0.0f, 0.0f, -1.0f), 30.0f);
    glm::vec3 narrowRay = projectRayToWorld(0.0f, HEIGHT / 2.0f, narrowViewProj, cameraPos, WIDTH, HEIGHT);

    // Wide FOV
    glm::mat4 wideViewProj = createViewProjection(cameraPos, glm::vec3(0.0f, 0.0f, -1.0f), 90.0f);
    glm::vec3 wideRay = projectRayToWorld(0.0f, HEIGHT / 2.0f, wideViewProj, cameraPos, WIDTH, HEIGHT);

    // Wide FOV should have larger X magnitude (more spread)
    // Both rays point left (negative X), but wide FOV spreads more
    EXPECT_LT(std::abs(narrowRay.x), std::abs(wideRay.x));
}

}  // namespace nexo::math
