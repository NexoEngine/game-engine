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

TEST_F(AmbientLightComponentTest, ModifyColorComponents) {
    light.color.r = 0.8f;
    light.color.g = 0.6f;
    light.color.b = 0.4f;

    EXPECT_FLOAT_EQ(light.color.r, 0.8f);
    EXPECT_FLOAT_EQ(light.color.g, 0.6f);
    EXPECT_FLOAT_EQ(light.color.b, 0.4f);
}

TEST_F(AmbientLightComponentTest, NegativeColorValues) {
    light.color = glm::vec3(-0.5f, -1.0f, -2.0f);
    auto memento = light.save();

    EXPECT_TRUE(compareVec3(memento.color, glm::vec3(-0.5f, -1.0f, -2.0f)));

    light.color = glm::vec3(0.0f);
    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.color, glm::vec3(-0.5f, -1.0f, -2.0f)));
}

TEST_F(AmbientLightComponentTest, LargeColorValues) {
    light.color = glm::vec3(100.0f, 1000.0f, 10000.0f);
    auto memento = light.save();

    EXPECT_TRUE(compareVec3(memento.color, glm::vec3(100.0f, 1000.0f, 10000.0f)));

    light.color = glm::vec3(0.0f);
    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.color, glm::vec3(100.0f, 1000.0f, 10000.0f)));
}

TEST_F(AmbientLightComponentTest, ZeroColorValues) {
    light.color = glm::vec3(0.0f, 0.0f, 0.0f);
    auto memento = light.save();

    EXPECT_TRUE(compareVec3(memento.color, glm::vec3(0.0f, 0.0f, 0.0f)));
}

TEST_F(AmbientLightComponentTest, MultipleSaveRestore) {
    light.color = glm::vec3(0.1f, 0.2f, 0.3f);
    auto memento1 = light.save();

    light.color = glm::vec3(0.4f, 0.5f, 0.6f);
    auto memento2 = light.save();

    light.color = glm::vec3(0.7f, 0.8f, 0.9f);
    auto memento3 = light.save();

    light.restore(memento2);
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.4f, 0.5f, 0.6f)));

    light.restore(memento1);
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.1f, 0.2f, 0.3f)));

    light.restore(memento3);
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.7f, 0.8f, 0.9f)));
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

TEST_F(DirectionalLightComponentTest, ModifyDirectionAndColor) {
    light.direction.x = 1.0f;
    light.direction.y = -0.5f;
    light.direction.z = 0.25f;
    light.color.r = 0.9f;
    light.color.g = 0.85f;
    light.color.b = 0.8f;

    EXPECT_FLOAT_EQ(light.direction.x, 1.0f);
    EXPECT_FLOAT_EQ(light.direction.y, -0.5f);
    EXPECT_FLOAT_EQ(light.direction.z, 0.25f);
    EXPECT_FLOAT_EQ(light.color.r, 0.9f);
    EXPECT_FLOAT_EQ(light.color.g, 0.85f);
    EXPECT_FLOAT_EQ(light.color.b, 0.8f);
}

TEST_F(DirectionalLightComponentTest, NegativeDirectionValues) {
    light.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
    light.color = glm::vec3(1.0f, 1.0f, 1.0f);
    auto memento = light.save();

    EXPECT_TRUE(compareVec3(memento.direction, glm::vec3(-1.0f, -1.0f, -1.0f)));

    light.direction = glm::vec3(0.0f);
    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(-1.0f, -1.0f, -1.0f)));
}

TEST_F(DirectionalLightComponentTest, ZeroDirectionValues) {
    light.direction = glm::vec3(0.0f, 0.0f, 0.0f);
    light.color = glm::vec3(1.0f, 1.0f, 1.0f);
    auto memento = light.save();

    EXPECT_TRUE(compareVec3(memento.direction, glm::vec3(0.0f, 0.0f, 0.0f)));
}

TEST_F(DirectionalLightComponentTest, LargeDirectionValues) {
    light.direction = glm::vec3(1000.0f, 5000.0f, 10000.0f);
    light.color = glm::vec3(1.0f, 1.0f, 1.0f);
    auto memento = light.save();

    light.restore(memento);
    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(1000.0f, 5000.0f, 10000.0f)));
}

TEST_F(DirectionalLightComponentTest, NegativeColorValues) {
    light.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    light.color = glm::vec3(-0.5f, -0.5f, -0.5f);
    auto memento = light.save();

    light.restore(memento);
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(-0.5f, -0.5f, -0.5f)));
}

TEST_F(DirectionalLightComponentTest, MultipleConstructorCalls) {
    DirectionalLightComponent light1(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    DirectionalLightComponent light2(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    DirectionalLightComponent light3(glm::vec3(0.0f, 0.0f, 1.0f));

    EXPECT_TRUE(compareVec3(light1.direction, glm::vec3(1.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(light1.color, glm::vec3(1.0f, 0.0f, 0.0f)));

    EXPECT_TRUE(compareVec3(light2.direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(light2.color, glm::vec3(0.0f, 1.0f, 0.0f)));

    EXPECT_TRUE(compareVec3(light3.direction, glm::vec3(0.0f, 0.0f, 1.0f)));
    EXPECT_TRUE(compareVec3(light3.color, glm::vec3(1.0f, 1.0f, 1.0f)));
}

TEST_F(DirectionalLightComponentTest, MultipleMementoRestore) {
    light.direction = glm::vec3(1.0f, 0.0f, 0.0f);
    light.color = glm::vec3(1.0f, 0.0f, 0.0f);
    auto memento1 = light.save();

    light.direction = glm::vec3(0.0f, 1.0f, 0.0f);
    light.color = glm::vec3(0.0f, 1.0f, 0.0f);
    auto memento2 = light.save();

    light.direction = glm::vec3(0.0f, 0.0f, 1.0f);
    light.color = glm::vec3(0.0f, 0.0f, 1.0f);
    auto memento3 = light.save();

    light.restore(memento2);
    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.0f, 1.0f, 0.0f)));

    light.restore(memento1);
    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(1.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(1.0f, 0.0f, 0.0f)));

    light.restore(memento3);
    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(0.0f, 0.0f, 1.0f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.0f, 0.0f, 1.0f)));
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

TEST_F(PointLightComponentTest, ModifyIndividualFields) {
    light.color.r = 0.5f;
    light.color.g = 0.6f;
    light.color.b = 0.7f;
    light.linear = 0.1f;
    light.quadratic = 0.05f;
    light.maxDistance = 100.0f;
    light.constant = 2.0f;

    EXPECT_FLOAT_EQ(light.color.r, 0.5f);
    EXPECT_FLOAT_EQ(light.color.g, 0.6f);
    EXPECT_FLOAT_EQ(light.color.b, 0.7f);
    EXPECT_FLOAT_EQ(light.linear, 0.1f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.05f);
    EXPECT_FLOAT_EQ(light.maxDistance, 100.0f);
    EXPECT_FLOAT_EQ(light.constant, 2.0f);
}

TEST_F(PointLightComponentTest, NegativeAttenuationValues) {
    light.linear = -0.09f;
    light.quadratic = -0.032f;
    light.constant = -1.0f;
    auto memento = light.save();

    EXPECT_FLOAT_EQ(memento.linear, -0.09f);
    EXPECT_FLOAT_EQ(memento.quadratic, -0.032f);
    EXPECT_FLOAT_EQ(memento.constant, -1.0f);

    light.linear = 0.0f;
    light.quadratic = 0.0f;
    light.constant = 0.0f;
    light.restore(memento);

    EXPECT_FLOAT_EQ(light.linear, -0.09f);
    EXPECT_FLOAT_EQ(light.quadratic, -0.032f);
    EXPECT_FLOAT_EQ(light.constant, -1.0f);
}

TEST_F(PointLightComponentTest, ZeroAttenuationValues) {
    light.linear = 0.0f;
    light.quadratic = 0.0f;
    light.maxDistance = 0.0f;
    light.constant = 0.0f;
    auto memento = light.save();

    EXPECT_FLOAT_EQ(memento.linear, 0.0f);
    EXPECT_FLOAT_EQ(memento.quadratic, 0.0f);
    EXPECT_FLOAT_EQ(memento.maxDistance, 0.0f);
    EXPECT_FLOAT_EQ(memento.constant, 0.0f);
}

TEST_F(PointLightComponentTest, LargeAttenuationValues) {
    light.linear = 1000.0f;
    light.quadratic = 5000.0f;
    light.maxDistance = 10000.0f;
    light.constant = 10000.0f;
    auto memento = light.save();

    light.restore(memento);

    EXPECT_FLOAT_EQ(light.linear, 1000.0f);
    EXPECT_FLOAT_EQ(light.quadratic, 5000.0f);
    EXPECT_FLOAT_EQ(light.maxDistance, 10000.0f);
    EXPECT_FLOAT_EQ(light.constant, 10000.0f);
}

TEST_F(PointLightComponentTest, NegativeColorValues) {
    light.color = glm::vec3(-0.5f, -1.0f, -2.0f);
    auto memento = light.save();

    light.restore(memento);
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(-0.5f, -1.0f, -2.0f)));
}

TEST_F(PointLightComponentTest, VerySmallAttenuationValues) {
    light.linear = 0.00001f;
    light.quadratic = 0.000001f;
    light.constant = 0.0001f;
    auto memento = light.save();

    light.restore(memento);

    EXPECT_FLOAT_EQ(light.linear, 0.00001f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.000001f);
    EXPECT_FLOAT_EQ(light.constant, 0.0001f);
}

TEST_F(PointLightComponentTest, MaxDistanceEdgeCases) {
    light.maxDistance = 0.0f;
    auto memento1 = light.save();
    EXPECT_FLOAT_EQ(memento1.maxDistance, 0.0f);

    light.maxDistance = 50.0f;
    auto memento2 = light.save();
    EXPECT_FLOAT_EQ(memento2.maxDistance, 50.0f);

    light.maxDistance = 10000.0f;
    auto memento3 = light.save();
    EXPECT_FLOAT_EQ(memento3.maxDistance, 10000.0f);
}

TEST_F(PointLightComponentTest, MultipleMementoRestore) {
    light.color = glm::vec3(1.0f, 0.0f, 0.0f);
    light.linear = 0.1f;
    light.quadratic = 0.01f;
    light.maxDistance = 100.0f;
    light.constant = 1.0f;
    auto memento1 = light.save();

    light.color = glm::vec3(0.0f, 1.0f, 0.0f);
    light.linear = 0.2f;
    light.quadratic = 0.02f;
    light.maxDistance = 200.0f;
    light.constant = 2.0f;
    auto memento2 = light.save();

    light.color = glm::vec3(0.0f, 0.0f, 1.0f);
    light.linear = 0.3f;
    light.quadratic = 0.03f;
    light.maxDistance = 300.0f;
    light.constant = 3.0f;
    auto memento3 = light.save();

    light.restore(memento2);
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.0f, 1.0f, 0.0f)));
    EXPECT_FLOAT_EQ(light.linear, 0.2f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.02f);
    EXPECT_FLOAT_EQ(light.maxDistance, 200.0f);
    EXPECT_FLOAT_EQ(light.constant, 2.0f);

    light.restore(memento1);
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(1.0f, 0.0f, 0.0f)));
    EXPECT_FLOAT_EQ(light.linear, 0.1f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.01f);
    EXPECT_FLOAT_EQ(light.maxDistance, 100.0f);
    EXPECT_FLOAT_EQ(light.constant, 1.0f);

    light.restore(memento3);
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.0f, 0.0f, 1.0f)));
    EXPECT_FLOAT_EQ(light.linear, 0.3f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.03f);
    EXPECT_FLOAT_EQ(light.maxDistance, 300.0f);
    EXPECT_FLOAT_EQ(light.constant, 3.0f);
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

TEST_F(SpotLightComponentTest, ModifyIndividualFields) {
    light.direction.x = 0.5f;
    light.direction.y = -0.5f;
    light.direction.z = 0.707f;
    light.color.r = 0.9f;
    light.color.g = 0.8f;
    light.color.b = 0.7f;
    light.cutOff = 20.0f;
    light.outerCutoff = 25.0f;
    light.linear = 0.1f;
    light.quadratic = 0.05f;
    light.maxDistance = 500.0f;
    light.constant = 2.5f;

    EXPECT_FLOAT_EQ(light.direction.x, 0.5f);
    EXPECT_FLOAT_EQ(light.direction.y, -0.5f);
    EXPECT_FLOAT_EQ(light.direction.z, 0.707f);
    EXPECT_FLOAT_EQ(light.color.r, 0.9f);
    EXPECT_FLOAT_EQ(light.color.g, 0.8f);
    EXPECT_FLOAT_EQ(light.color.b, 0.7f);
    EXPECT_FLOAT_EQ(light.cutOff, 20.0f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 25.0f);
    EXPECT_FLOAT_EQ(light.linear, 0.1f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.05f);
    EXPECT_FLOAT_EQ(light.maxDistance, 500.0f);
    EXPECT_FLOAT_EQ(light.constant, 2.5f);
}

TEST_F(SpotLightComponentTest, NegativeDirectionValues) {
    light.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
    light.color = glm::vec3(1.0f, 1.0f, 1.0f);
    auto memento = light.save();

    EXPECT_TRUE(compareVec3(memento.direction, glm::vec3(-1.0f, -1.0f, -1.0f)));

    light.direction = glm::vec3(0.0f);
    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(-1.0f, -1.0f, -1.0f)));
}

TEST_F(SpotLightComponentTest, NegativeColorValues) {
    light.color = glm::vec3(-0.5f, -1.0f, -2.0f);
    auto memento = light.save();

    light.restore(memento);
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(-0.5f, -1.0f, -2.0f)));
}

TEST_F(SpotLightComponentTest, NegativeCutoffValues) {
    light.cutOff = -10.0f;
    light.outerCutoff = -5.0f;
    auto memento = light.save();

    EXPECT_FLOAT_EQ(memento.cutOff, -10.0f);
    EXPECT_FLOAT_EQ(memento.outerCutoff, -5.0f);

    light.cutOff = 0.0f;
    light.outerCutoff = 0.0f;
    light.restore(memento);

    EXPECT_FLOAT_EQ(light.cutOff, -10.0f);
    EXPECT_FLOAT_EQ(light.outerCutoff, -5.0f);
}

TEST_F(SpotLightComponentTest, ZeroValues) {
    light.direction = glm::vec3(0.0f, 0.0f, 0.0f);
    light.color = glm::vec3(0.0f, 0.0f, 0.0f);
    light.cutOff = 0.0f;
    light.outerCutoff = 0.0f;
    light.linear = 0.0f;
    light.quadratic = 0.0f;
    light.maxDistance = 0.0f;
    light.constant = 0.0f;
    auto memento = light.save();

    EXPECT_TRUE(compareVec3(memento.direction, glm::vec3(0.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(memento.color, glm::vec3(0.0f, 0.0f, 0.0f)));
    EXPECT_FLOAT_EQ(memento.cutOff, 0.0f);
    EXPECT_FLOAT_EQ(memento.outerCutoff, 0.0f);
    EXPECT_FLOAT_EQ(memento.linear, 0.0f);
    EXPECT_FLOAT_EQ(memento.quadratic, 0.0f);
    EXPECT_FLOAT_EQ(memento.maxDistance, 0.0f);
    EXPECT_FLOAT_EQ(memento.constant, 0.0f);
}

TEST_F(SpotLightComponentTest, LargeValues) {
    light.direction = glm::vec3(1000.0f, 5000.0f, 10000.0f);
    light.color = glm::vec3(100.0f, 500.0f, 1000.0f);
    light.cutOff = 1000.0f;
    light.outerCutoff = 5000.0f;
    light.linear = 1000.0f;
    light.quadratic = 5000.0f;
    light.maxDistance = 100000.0f;
    light.constant = 10000.0f;
    auto memento = light.save();

    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(1000.0f, 5000.0f, 10000.0f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(100.0f, 500.0f, 1000.0f)));
    EXPECT_FLOAT_EQ(light.cutOff, 1000.0f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 5000.0f);
    EXPECT_FLOAT_EQ(light.linear, 1000.0f);
    EXPECT_FLOAT_EQ(light.quadratic, 5000.0f);
    EXPECT_FLOAT_EQ(light.maxDistance, 100000.0f);
    EXPECT_FLOAT_EQ(light.constant, 10000.0f);
}

TEST_F(SpotLightComponentTest, VerySmallValues) {
    light.cutOff = 0.0001f;
    light.outerCutoff = 0.0002f;
    light.linear = 0.00001f;
    light.quadratic = 0.000001f;
    light.constant = 0.0001f;
    auto memento = light.save();

    light.restore(memento);

    EXPECT_FLOAT_EQ(light.cutOff, 0.0001f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 0.0002f);
    EXPECT_FLOAT_EQ(light.linear, 0.00001f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.000001f);
    EXPECT_FLOAT_EQ(light.constant, 0.0001f);
}

TEST_F(SpotLightComponentTest, CutoffValuesEqual) {
    light.cutOff = 12.5f;
    light.outerCutoff = 12.5f;
    auto memento = light.save();

    EXPECT_FLOAT_EQ(memento.cutOff, 12.5f);
    EXPECT_FLOAT_EQ(memento.outerCutoff, 12.5f);
}

TEST_F(SpotLightComponentTest, OuterCutoffSmallerThanCutoff) {
    light.cutOff = 20.0f;
    light.outerCutoff = 10.0f;
    auto memento = light.save();

    light.restore(memento);

    EXPECT_FLOAT_EQ(light.cutOff, 20.0f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 10.0f);
}

TEST_F(SpotLightComponentTest, NegativeAttenuationValues) {
    light.linear = -0.09f;
    light.quadratic = -0.032f;
    light.constant = -1.0f;
    auto memento = light.save();

    light.restore(memento);

    EXPECT_FLOAT_EQ(light.linear, -0.09f);
    EXPECT_FLOAT_EQ(light.quadratic, -0.032f);
    EXPECT_FLOAT_EQ(light.constant, -1.0f);
}

TEST_F(SpotLightComponentTest, MaxDistanceEdgeCases) {
    light.maxDistance = 0.0f;
    auto memento1 = light.save();
    EXPECT_FLOAT_EQ(memento1.maxDistance, 0.0f);

    light.maxDistance = 325.0f;
    auto memento2 = light.save();
    EXPECT_FLOAT_EQ(memento2.maxDistance, 325.0f);

    light.maxDistance = 100000.0f;
    auto memento3 = light.save();
    EXPECT_FLOAT_EQ(memento3.maxDistance, 100000.0f);

    light.maxDistance = -100.0f;
    auto memento4 = light.save();
    EXPECT_FLOAT_EQ(memento4.maxDistance, -100.0f);
}

TEST_F(SpotLightComponentTest, MultipleMementoRestore) {
    light.direction = glm::vec3(1.0f, 0.0f, 0.0f);
    light.color = glm::vec3(1.0f, 0.0f, 0.0f);
    light.cutOff = 10.0f;
    light.outerCutoff = 15.0f;
    light.linear = 0.1f;
    light.quadratic = 0.01f;
    light.maxDistance = 100.0f;
    light.constant = 1.0f;
    auto memento1 = light.save();

    light.direction = glm::vec3(0.0f, 1.0f, 0.0f);
    light.color = glm::vec3(0.0f, 1.0f, 0.0f);
    light.cutOff = 20.0f;
    light.outerCutoff = 25.0f;
    light.linear = 0.2f;
    light.quadratic = 0.02f;
    light.maxDistance = 200.0f;
    light.constant = 2.0f;
    auto memento2 = light.save();

    light.direction = glm::vec3(0.0f, 0.0f, 1.0f);
    light.color = glm::vec3(0.0f, 0.0f, 1.0f);
    light.cutOff = 30.0f;
    light.outerCutoff = 35.0f;
    light.linear = 0.3f;
    light.quadratic = 0.03f;
    light.maxDistance = 300.0f;
    light.constant = 3.0f;
    auto memento3 = light.save();

    light.restore(memento2);
    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.0f, 1.0f, 0.0f)));
    EXPECT_FLOAT_EQ(light.cutOff, 20.0f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 25.0f);
    EXPECT_FLOAT_EQ(light.linear, 0.2f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.02f);
    EXPECT_FLOAT_EQ(light.maxDistance, 200.0f);
    EXPECT_FLOAT_EQ(light.constant, 2.0f);

    light.restore(memento1);
    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(1.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(1.0f, 0.0f, 0.0f)));
    EXPECT_FLOAT_EQ(light.cutOff, 10.0f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 15.0f);
    EXPECT_FLOAT_EQ(light.linear, 0.1f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.01f);
    EXPECT_FLOAT_EQ(light.maxDistance, 100.0f);
    EXPECT_FLOAT_EQ(light.constant, 1.0f);

    light.restore(memento3);
    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(0.0f, 0.0f, 1.0f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(0.0f, 0.0f, 1.0f)));
    EXPECT_FLOAT_EQ(light.cutOff, 30.0f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 35.0f);
    EXPECT_FLOAT_EQ(light.linear, 0.3f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.03f);
    EXPECT_FLOAT_EQ(light.maxDistance, 300.0f);
    EXPECT_FLOAT_EQ(light.constant, 3.0f);
}

TEST_F(SpotLightComponentTest, RealisticFlashlightScenario) {
    light.direction = glm::vec3(0.0f, 0.0f, -1.0f);
    light.color = glm::vec3(1.0f, 1.0f, 0.9f);
    light.cutOff = 12.5f;
    light.outerCutoff = 17.5f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    light.maxDistance = 100.0f;
    light.constant = 1.0f;
    auto memento = light.save();

    light.direction = glm::vec3(0.0f);
    light.color = glm::vec3(0.0f);
    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(0.0f, 0.0f, -1.0f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(1.0f, 1.0f, 0.9f)));
    EXPECT_FLOAT_EQ(light.cutOff, 12.5f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 17.5f);
    EXPECT_FLOAT_EQ(light.linear, 0.09f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.032f);
    EXPECT_FLOAT_EQ(light.maxDistance, 100.0f);
    EXPECT_FLOAT_EQ(light.constant, 1.0f);
}

TEST_F(SpotLightComponentTest, RealisticSpotlightScenario) {
    light.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    light.color = glm::vec3(1.0f, 1.0f, 0.8f);
    light.cutOff = 20.0f;
    light.outerCutoff = 30.0f;
    light.linear = 0.045f;
    light.quadratic = 0.0075f;
    light.maxDistance = 200.0f;
    light.constant = 1.0f;
    auto memento = light.save();

    light.restore(memento);

    EXPECT_TRUE(compareVec3(light.direction, glm::vec3(0.0f, -1.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(light.color, glm::vec3(1.0f, 1.0f, 0.8f)));
    EXPECT_FLOAT_EQ(light.cutOff, 20.0f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 30.0f);
    EXPECT_FLOAT_EQ(light.linear, 0.045f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.0075f);
    EXPECT_FLOAT_EQ(light.maxDistance, 200.0f);
    EXPECT_FLOAT_EQ(light.constant, 1.0f);
}

}  // namespace nexo::components
