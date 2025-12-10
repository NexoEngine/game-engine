//// Light.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for Light components (memento pattern)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include "components/Light.hpp"

namespace nexo::components {

// Helper to compare vec3
static bool compareVec3(const glm::vec3& a, const glm::vec3& b, float epsilon = 0.0001f) {
    return glm::all(glm::epsilonEqual(a, b, epsilon));
}

// =============================================================================
// AmbientLightComponent Tests
// =============================================================================

class AmbientLightComponentTest : public ::testing::Test {
protected:
    AmbientLightComponent light;
};

TEST_F(AmbientLightComponentTest, DefaultColor) {
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.0f)));
}

TEST_F(AmbientLightComponentTest, SaveCapturesColor) {
    light.color = glm::vec3(0.5f, 0.6f, 0.7f);
    auto memento = light.save();
    EXPECT_TRUE(compareVec3(memento.color, glm::vec3(0.5f, 0.6f, 0.7f)));
}

TEST_F(AmbientLightComponentTest, RestoreAppliesColor) {
    AmbientLightComponent::Memento memento;
    memento.color = glm::vec3(1.0f, 0.8f, 0.6f);

    light.restore(memento);
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(1.0f, 0.8f, 0.6f)));
}

TEST_F(AmbientLightComponentTest, SaveRestoreRoundTrip) {
    light.color = glm::vec3(0.3f, 0.4f, 0.5f);
    auto memento = light.save();

    light.color = glm::vec3(0.0f);
    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.3f, 0.4f, 0.5f)));
}

// =============================================================================
// DirectionalLightComponent Tests
// =============================================================================

class DirectionalLightComponentTest : public ::testing::Test {
protected:
    DirectionalLightComponent light;
};

TEST_F(DirectionalLightComponentTest, DefaultConstruction) {
    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(0.0f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.0f)));
}

TEST_F(DirectionalLightComponentTest, ParameterizedConstruction) {
    DirectionalLightComponent paramLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.8f));

    EXPECT_TRUE(compareVec3(paramLight.direction, glm::vec3(0.0f, -1.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(paramLight.color, glm::vec3(1.0f, 1.0f, 0.8f)));
}

TEST_F(DirectionalLightComponentTest, ConstructionWithDefaultColor) {
    DirectionalLightComponent paramLight(glm::vec3(1.0f, 0.0f, 0.0f));

    EXPECT_TRUE(compareVec3(paramLight.direction, glm::vec3(1.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(paramLight.color, glm::vec3(1.0f, 1.0f, 1.0f)));
}

TEST_F(DirectionalLightComponentTest, SaveCapturesDirection) {
    light.direction = glm::vec3(0.0f, -1.0f, 0.5f);
    auto memento = light.save();
    EXPECT_TRUE(compareVec3(memento.direction, glm::vec3(0.0f, -1.0f, 0.5f)));
}

TEST_F(DirectionalLightComponentTest, SaveCapturesColor) {
    light.color = glm::vec3(0.9f, 0.8f, 0.7f);
    auto memento = light.save();
    EXPECT_TRUE(compareVec3(memento.color, glm::vec3(0.9f, 0.8f, 0.7f)));
}

TEST_F(DirectionalLightComponentTest, RestoreAppliesAll) {
    DirectionalLightComponent::Memento memento;
    memento.direction = glm::vec3(1.0f, -0.5f, 0.0f);
    memento.color = glm::vec3(0.5f, 0.5f, 0.5f);

    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(1.0f, -0.5f, 0.0f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.5f, 0.5f, 0.5f)));
}

TEST_F(DirectionalLightComponentTest, SaveRestoreRoundTrip) {
    light.direction = glm::vec3(0.577f, -0.577f, 0.577f);
    light.color = glm::vec3(1.0f, 0.95f, 0.9f);
    auto memento = light.save();

    light.direction = glm::vec3(0.0f);
    light.color = glm::vec3(0.0f);
    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(0.577f, -0.577f, 0.577f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(1.0f, 0.95f, 0.9f)));
}

// =============================================================================
// PointLightComponent Tests
// =============================================================================

class PointLightComponentTest : public ::testing::Test {
protected:
    PointLightComponent light;
};

TEST_F(PointLightComponentTest, DefaultValues) {
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.0f)));
    EXPECT_FLOAT_EQ(light.linear, 0.0f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.0f);
    EXPECT_FLOAT_EQ(light.maxDistance, 50.0f);
    EXPECT_FLOAT_EQ(light.constant, 1.0f);
}

TEST_F(PointLightComponentTest, SaveCapturesAllFields) {
    light.color = glm::vec3(1.0f, 0.5f, 0.0f);
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    light.maxDistance = 100.0f;
    light.constant = 0.5f;

    auto memento = light.save();

    EXPECT_TRUE(compareVec3(memento.color, glm::vec3(1.0f, 0.5f, 0.0f)));
    EXPECT_FLOAT_EQ(memento.linear, 0.09f);
    EXPECT_FLOAT_EQ(memento.quadratic, 0.032f);
    EXPECT_FLOAT_EQ(memento.maxDistance, 100.0f);
    EXPECT_FLOAT_EQ(memento.constant, 0.5f);
}

TEST_F(PointLightComponentTest, RestoreAppliesAllFields) {
    PointLightComponent::Memento memento;
    memento.color = glm::vec3(0.2f, 0.8f, 0.2f);
    memento.linear = 0.14f;
    memento.quadratic = 0.07f;
    memento.maxDistance = 75.0f;
    memento.constant = 2.0f;

    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.2f, 0.8f, 0.2f)));
    EXPECT_FLOAT_EQ(light.linear, 0.14f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.07f);
    EXPECT_FLOAT_EQ(light.maxDistance, 75.0f);
    EXPECT_FLOAT_EQ(light.constant, 2.0f);
}

TEST_F(PointLightComponentTest, SaveRestoreRoundTrip) {
    light.color = glm::vec3(0.8f, 0.8f, 1.0f);
    light.linear = 0.045f;
    light.quadratic = 0.0075f;
    light.maxDistance = 200.0f;
    light.constant = 1.5f;

    auto memento = light.save();

    light.color = glm::vec3(0.0f);
    light.linear = 0.0f;
    light.quadratic = 0.0f;
    light.maxDistance = 0.0f;
    light.constant = 0.0f;

    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.8f, 0.8f, 1.0f)));
    EXPECT_FLOAT_EQ(light.linear, 0.045f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.0075f);
    EXPECT_FLOAT_EQ(light.maxDistance, 200.0f);
    EXPECT_FLOAT_EQ(light.constant, 1.5f);
}

// =============================================================================
// SpotLightComponent Tests
// =============================================================================

class SpotLightComponentTest : public ::testing::Test {
protected:
    SpotLightComponent light;
};

TEST_F(SpotLightComponentTest, DefaultValues) {
    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(0.0f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.0f)));
    EXPECT_FLOAT_EQ(light.cutOff, 0.0f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 0.0f);
    EXPECT_FLOAT_EQ(light.linear, 0.0f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.0f);
    EXPECT_FLOAT_EQ(light.maxDistance, 325.0f);
    EXPECT_FLOAT_EQ(light.constant, 1.0f);
}

TEST_F(SpotLightComponentTest, SaveCapturesAllFields) {
    light.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    light.color = glm::vec3(1.0f, 1.0f, 0.9f);
    light.cutOff = 12.5f;
    light.outerCutoff = 17.5f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    light.maxDistance = 150.0f;
    light.constant = 1.2f;

    auto memento = light.save();

    EXPECT_TRUE(compareVec3(memento.direction, glm::vec3(0.0f, -1.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(memento.color, glm::vec3(1.0f, 1.0f, 0.9f)));
    EXPECT_FLOAT_EQ(memento.cutOff, 12.5f);
    EXPECT_FLOAT_EQ(memento.outerCutoff, 17.5f);
    EXPECT_FLOAT_EQ(memento.linear, 0.09f);
    EXPECT_FLOAT_EQ(memento.quadratic, 0.032f);
    EXPECT_FLOAT_EQ(memento.maxDistance, 150.0f);
    EXPECT_FLOAT_EQ(memento.constant, 1.2f);
}

TEST_F(SpotLightComponentTest, RestoreAppliesAllFields) {
    SpotLightComponent::Memento memento;
    memento.direction = glm::vec3(0.5f, -0.5f, 0.707f);
    memento.color = glm::vec3(0.9f, 0.7f, 0.5f);
    memento.cutOff = 15.0f;
    memento.outerCutoff = 20.0f;
    memento.linear = 0.07f;
    memento.quadratic = 0.017f;
    memento.maxDistance = 250.0f;
    memento.constant = 0.8f;

    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(0.5f, -0.5f, 0.707f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.9f, 0.7f, 0.5f)));
    EXPECT_FLOAT_EQ(light.cutOff, 15.0f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 20.0f);
    EXPECT_FLOAT_EQ(light.linear, 0.07f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.017f);
    EXPECT_FLOAT_EQ(light.maxDistance, 250.0f);
    EXPECT_FLOAT_EQ(light.constant, 0.8f);
}

TEST_F(SpotLightComponentTest, SaveRestoreRoundTrip) {
    light.direction = glm::vec3(0.0f, -0.707f, -0.707f);
    light.color = glm::vec3(1.0f, 0.9f, 0.8f);
    light.cutOff = 10.0f;
    light.outerCutoff = 15.0f;
    light.linear = 0.022f;
    light.quadratic = 0.0019f;
    light.maxDistance = 400.0f;
    light.constant = 1.0f;

    auto memento = light.save();

    // Reset all values
    light.direction = glm::vec3(0.0f);
    light.color = glm::vec3(0.0f);
    light.cutOff = 0.0f;
    light.outerCutoff = 0.0f;
    light.linear = 0.0f;
    light.quadratic = 0.0f;
    light.maxDistance = 0.0f;
    light.constant = 0.0f;

    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(0.0f, -0.707f, -0.707f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(1.0f, 0.9f, 0.8f)));
    EXPECT_FLOAT_EQ(light.cutOff, 10.0f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 15.0f);
    EXPECT_FLOAT_EQ(light.linear, 0.022f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.0019f);
    EXPECT_FLOAT_EQ(light.maxDistance, 400.0f);
    EXPECT_FLOAT_EQ(light.constant, 1.0f);
}

}  // namespace nexo::components
