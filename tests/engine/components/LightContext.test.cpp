//// LightContext.test.cpp ///////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for LightContext struct
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <array>
#include "components/Light.hpp"

namespace nexo::components {

// Helper to compare vec3
static bool compareVec3(const glm::vec3& a, const glm::vec3& b, float epsilon = 0.0001f) {
    return glm::all(glm::epsilonEqual(a, b, epsilon));
}

// =============================================================================
// Constant Tests
// =============================================================================

TEST(LightContextConstants, MaxPointLightsValue) {
    EXPECT_EQ(MAX_POINT_LIGHTS, 10);
}

TEST(LightContextConstants, MaxSpotLightsValue) {
    EXPECT_EQ(MAX_SPOT_LIGHTS, 10);
}

// =============================================================================
// LightContext Tests
// =============================================================================

class LightContextTest : public ::testing::Test {
protected:
    LightContext context;
};

TEST_F(LightContextTest, DefaultConstruction_PointLightCountZero) {
    EXPECT_EQ(context.pointLightCount, 0);
}

TEST_F(LightContextTest, DefaultConstruction_SpotLightCountZero) {
    EXPECT_EQ(context.spotLightCount, 0);
}

TEST_F(LightContextTest, PointLightsArraySize) {
    EXPECT_EQ(context.pointLights.size(), MAX_POINT_LIGHTS);
}

TEST_F(LightContextTest, SpotLightsArraySize) {
    EXPECT_EQ(context.spotLights.size(), MAX_SPOT_LIGHTS);
}

TEST_F(LightContextTest, CanSetAndReadAmbientLight) {
    glm::vec3 testColor(0.2f, 0.3f, 0.4f);
    context.ambientLight = testColor;

    EXPECT_TRUE(compareVec3(context.ambientLight, testColor));
}

TEST_F(LightContextTest, CanModifyPointLightCount) {
    context.pointLightCount = 5;
    EXPECT_EQ(context.pointLightCount, 5);

    context.pointLightCount = MAX_POINT_LIGHTS;
    EXPECT_EQ(context.pointLightCount, MAX_POINT_LIGHTS);
}

TEST_F(LightContextTest, CanModifySpotLightCount) {
    context.spotLightCount = 7;
    EXPECT_EQ(context.spotLightCount, 7);

    context.spotLightCount = MAX_SPOT_LIGHTS;
    EXPECT_EQ(context.spotLightCount, MAX_SPOT_LIGHTS);
}

TEST_F(LightContextTest, DirLightMemberAccessible) {
    // Test that dirLight member exists and can be accessed
    context.dirLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    context.dirLight.color = glm::vec3(1.0f, 1.0f, 1.0f);

    EXPECT_TRUE(compareVec3(context.dirLight.direction, glm::vec3(0.0f, -1.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(context.dirLight.color, glm::vec3(1.0f, 1.0f, 1.0f)));
}

// =============================================================================
// Additional Integration Tests
// =============================================================================

TEST_F(LightContextTest, CanStorePointLightEntities) {
    // Test that we can store entities in the pointLights array
    context.pointLights[0] = 100;
    context.pointLights[5] = 500;
    context.pointLights[9] = 999;

    EXPECT_EQ(context.pointLights[0], 100);
    EXPECT_EQ(context.pointLights[5], 500);
    EXPECT_EQ(context.pointLights[9], 999);
}

TEST_F(LightContextTest, CanStoreSpotLightEntities) {
    // Test that we can store entities in the spotLights array
    context.spotLights[0] = 200;
    context.spotLights[4] = 400;
    context.spotLights[9] = 888;

    EXPECT_EQ(context.spotLights[0], 200);
    EXPECT_EQ(context.spotLights[4], 400);
    EXPECT_EQ(context.spotLights[9], 888);
}

TEST_F(LightContextTest, CompleteScenarioWithAllFields) {
    // Set up a complete lighting context
    context.ambientLight = glm::vec3(0.1f, 0.1f, 0.15f);

    // Add some point lights
    context.pointLights[0] = 10;
    context.pointLights[1] = 20;
    context.pointLightCount = 2;

    // Add some spot lights
    context.spotLights[0] = 30;
    context.spotLights[1] = 40;
    context.spotLights[2] = 50;
    context.spotLightCount = 3;

    // Set directional light
    context.dirLight = DirectionalLightComponent(
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.9f, 0.9f, 0.8f)
    );

    // Verify all fields
    EXPECT_TRUE(compareVec3(context.ambientLight, glm::vec3(0.1f, 0.1f, 0.15f)));
    EXPECT_EQ(context.pointLightCount, 2);
    EXPECT_EQ(context.pointLights[0], 10);
    EXPECT_EQ(context.pointLights[1], 20);
    EXPECT_EQ(context.spotLightCount, 3);
    EXPECT_EQ(context.spotLights[0], 30);
    EXPECT_EQ(context.spotLights[1], 40);
    EXPECT_EQ(context.spotLights[2], 50);
    EXPECT_TRUE(compareVec3(context.dirLight.direction, glm::vec3(0.0f, -1.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(context.dirLight.color, glm::vec3(0.9f, 0.9f, 0.8f)));
}

}  // namespace nexo::components
