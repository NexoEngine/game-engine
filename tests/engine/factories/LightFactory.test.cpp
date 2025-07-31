///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date: Today lol
//  Description: Test file for the LightFactory class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "LightFactory.hpp"
#include "Application.hpp"
#include "components/Light.hpp"
#include "components/Uuid.hpp"
#include "ecs/Coordinator.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace nexo {

class LightFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset and initialize coordinator
        Application::m_coordinator = std::make_shared<ecs::Coordinator>();
        Application::m_coordinator->init();
        
        // Register required components
        Application::m_coordinator->registerComponent<components::AmbientLightComponent>();
        Application::m_coordinator->registerComponent<components::DirectionalLightComponent>();
        Application::m_coordinator->registerComponent<components::PointLightComponent>();
        Application::m_coordinator->registerComponent<components::SpotLightComponent>();
        Application::m_coordinator->registerComponent<components::UuidComponent>();
    }
    
    void TearDown() override {
        Application::m_coordinator.reset();
    }
};

// Ambient Light Tests
TEST_F(LightFactoryTest, CreateAmbientLight) {
    glm::vec3 color(0.2f, 0.3f, 0.4f);
    
    ecs::Entity light = LightFactory::createAmbientLight(color);
    
    // Verify entity was created
    EXPECT_NE(light, ecs::INVALID_ENTITY);
    
    // Verify AmbientLightComponent
    EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::AmbientLightComponent>(light));
    auto& ambientLight = Application::m_coordinator->getComponent<components::AmbientLightComponent>(light);
    EXPECT_EQ(ambientLight.color, color);
    
    // Verify UUID component
    EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::UuidComponent>(light));
    auto& uuid = Application::m_coordinator->getComponent<components::UuidComponent>(light);
    EXPECT_FALSE(uuid.uuid.empty());
}

TEST_F(LightFactoryTest, CreateAmbientLight_BlackColor) {
    glm::vec3 blackColor(0.0f, 0.0f, 0.0f);
    
    ecs::Entity light = LightFactory::createAmbientLight(blackColor);
    
    auto& ambientLight = Application::m_coordinator->getComponent<components::AmbientLightComponent>(light);
    EXPECT_EQ(ambientLight.color, blackColor);
}

TEST_F(LightFactoryTest, CreateAmbientLight_BrightColor) {
    glm::vec3 brightColor(1.0f, 1.0f, 1.0f);
    
    ecs::Entity light = LightFactory::createAmbientLight(brightColor);
    
    auto& ambientLight = Application::m_coordinator->getComponent<components::AmbientLightComponent>(light);
    EXPECT_EQ(ambientLight.color, brightColor);
}

// Directional Light Tests
TEST_F(LightFactoryTest, CreateDirectionalLight_Default) {
    glm::vec3 lightDir(0.0f, -1.0f, 0.0f); // Pointing down
    
    ecs::Entity light = LightFactory::createDirectionalLight(lightDir);
    
    // Verify entity was created
    EXPECT_NE(light, ecs::INVALID_ENTITY);
    
    // Verify DirectionalLightComponent
    EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::DirectionalLightComponent>(light));
    auto& dirLight = Application::m_coordinator->getComponent<components::DirectionalLightComponent>(light);
    EXPECT_EQ(dirLight.direction, lightDir);
    EXPECT_EQ(dirLight.color, glm::vec3(1.0f, 1.0f, 1.0f)); // Default white
    
    // Verify UUID
    EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::UuidComponent>(light));
}

TEST_F(LightFactoryTest, CreateDirectionalLight_CustomColor) {
    glm::vec3 lightDir(-0.5f, -0.5f, -0.5f);
    glm::vec3 color(1.0f, 0.8f, 0.6f); // Warm light
    
    ecs::Entity light = LightFactory::createDirectionalLight(lightDir, color);
    
    auto& dirLight = Application::m_coordinator->getComponent<components::DirectionalLightComponent>(light);
    EXPECT_EQ(dirLight.direction, lightDir);
    EXPECT_EQ(dirLight.color, color);
}

TEST_F(LightFactoryTest, CreateDirectionalLight_VariousDirections) {
    // Test various directions
    std::vector<glm::vec3> directions = {
        glm::vec3(1.0f, 0.0f, 0.0f),   // Right
        glm::vec3(-1.0f, 0.0f, 0.0f),  // Left
        glm::vec3(0.0f, 1.0f, 0.0f),   // Up
        glm::vec3(0.0f, -1.0f, 0.0f),  // Down
        glm::vec3(0.0f, 0.0f, 1.0f),   // Forward
        glm::vec3(0.0f, 0.0f, -1.0f),  // Backward
        glm::vec3(0.577f, 0.577f, 0.577f) // Diagonal (normalized)
    };
    
    for (const auto& dir : directions) {
        ecs::Entity light = LightFactory::createDirectionalLight(dir);
        auto& dirLight = Application::m_coordinator->getComponent<components::DirectionalLightComponent>(light);
        EXPECT_FLOAT_EQ(dirLight.direction.x, dir.x);
        EXPECT_FLOAT_EQ(dirLight.direction.y, dir.y);
        EXPECT_FLOAT_EQ(dirLight.direction.z, dir.z);
    }
}

// Point Light Tests
TEST_F(LightFactoryTest, CreatePointLight_Default) {
    glm::vec3 position(5.0f, 10.0f, 15.0f);
    
    ecs::Entity light = LightFactory::createPointLight(position);
    
    // Verify entity was created
    EXPECT_NE(light, ecs::INVALID_ENTITY);
    
    // Verify PointLightComponent
    EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::PointLightComponent>(light));
    auto& pointLight = Application::m_coordinator->getComponent<components::PointLightComponent>(light);
    // Note: PointLightComponent doesn't store position - it's in TransformComponent
    EXPECT_EQ(pointLight.color, glm::vec3(1.0f, 1.0f, 1.0f)); // Default white
    EXPECT_FLOAT_EQ(pointLight.linear, 0.09f); // Default linear attenuation
    EXPECT_FLOAT_EQ(pointLight.quadratic, 0.032f); // Default quadratic attenuation
    
    // Verify UUID
    EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::UuidComponent>(light));
}

TEST_F(LightFactoryTest, CreatePointLight_CustomParameters) {
    glm::vec3 position(0.0f, 5.0f, 0.0f);
    glm::vec3 color(1.0f, 0.0f, 0.0f); // Red light
    float linear = 0.14f;
    float quadratic = 0.07f;
    
    ecs::Entity light = LightFactory::createPointLight(position, color, linear, quadratic);
    
    auto& pointLight = Application::m_coordinator->getComponent<components::PointLightComponent>(light);
    // Note: PointLightComponent doesn't store position - it's in TransformComponent
    EXPECT_EQ(pointLight.color, color);
    EXPECT_FLOAT_EQ(pointLight.linear, linear);
    EXPECT_FLOAT_EQ(pointLight.quadratic, quadratic);
}

TEST_F(LightFactoryTest, CreatePointLight_DifferentAttenuations) {
    // Test different attenuation values for different ranges
    struct AttenuationTest {
        float linear;
        float quadratic;
        std::string description;
    };
    
    std::vector<AttenuationTest> tests = {
        {0.7f, 1.8f, "Very short range"},
        {0.35f, 0.44f, "Short range"},
        {0.09f, 0.032f, "Medium range (default)"},
        {0.045f, 0.0075f, "Long range"},
        {0.022f, 0.0019f, "Very long range"}
    };
    
    glm::vec3 position(0.0f, 0.0f, 0.0f);
    
    for (const auto& test : tests) {
        ecs::Entity light = LightFactory::createPointLight(position, glm::vec3(1.0f), test.linear, test.quadratic);
        auto& pointLight = Application::m_coordinator->getComponent<components::PointLightComponent>(light);
        EXPECT_FLOAT_EQ(pointLight.linear, test.linear);
        EXPECT_FLOAT_EQ(pointLight.quadratic, test.quadratic);
    }
}

// Spot Light Tests
TEST_F(LightFactoryTest, CreateSpotLight_Default) {
    glm::vec3 position(0.0f, 10.0f, 0.0f);
    glm::vec3 direction(0.0f, -1.0f, 0.0f);
    
    ecs::Entity light = LightFactory::createSpotLight(position, direction);
    
    // Verify entity was created
    EXPECT_NE(light, ecs::INVALID_ENTITY);
    
    // Verify SpotLightComponent
    EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::SpotLightComponent>(light));
    auto& spotLight = Application::m_coordinator->getComponent<components::SpotLightComponent>(light);
    // Note: SpotLightComponent doesn't store position - it's in TransformComponent
    EXPECT_EQ(spotLight.direction, direction);
    EXPECT_EQ(spotLight.color, glm::vec3(1.0f, 1.0f, 1.0f)); // Default white
    EXPECT_FLOAT_EQ(spotLight.linear, 0.09f);
    EXPECT_FLOAT_EQ(spotLight.quadratic, 0.032f);
    EXPECT_FLOAT_EQ(spotLight.cutOff, glm::cos(glm::radians(12.5f)));
    EXPECT_FLOAT_EQ(spotLight.outerCutoff, glm::cos(glm::radians(15.0f)));
    
    // Verify UUID
    EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::UuidComponent>(light));
}

TEST_F(LightFactoryTest, CreateSpotLight_CustomParameters) {
    glm::vec3 position(5.0f, 5.0f, 5.0f);
    glm::vec3 direction(-1.0f, -1.0f, -1.0f);
    glm::vec3 color(0.0f, 1.0f, 0.0f); // Green light
    float linear = 0.14f;
    float quadratic = 0.07f;
    float cutOff = glm::cos(glm::radians(20.0f));
    float outerCutoff = glm::cos(glm::radians(25.0f));
    
    ecs::Entity light = LightFactory::createSpotLight(position, direction, color, linear, quadratic, cutOff, outerCutoff);
    
    auto& spotLight = Application::m_coordinator->getComponent<components::SpotLightComponent>(light);
    // Note: SpotLightComponent doesn't store position - it's in TransformComponent
    EXPECT_EQ(spotLight.direction, direction);
    EXPECT_EQ(spotLight.color, color);
    EXPECT_FLOAT_EQ(spotLight.linear, linear);
    EXPECT_FLOAT_EQ(spotLight.quadratic, quadratic);
    EXPECT_FLOAT_EQ(spotLight.cutOff, cutOff);
    EXPECT_FLOAT_EQ(spotLight.outerCutoff, outerCutoff);
}

TEST_F(LightFactoryTest, CreateSpotLight_VariousConeAngles) {
    // Test various cone angles
    struct ConeTest {
        float innerAngle;
        float outerAngle;
        std::string description;
    };
    
    std::vector<ConeTest> tests = {
        {5.0f, 7.5f, "Very narrow cone"},
        {12.5f, 15.0f, "Default cone"},
        {30.0f, 35.0f, "Wide cone"},
        {45.0f, 50.0f, "Very wide cone"},
        {60.0f, 70.0f, "Extremely wide cone"}
    };
    
    glm::vec3 position(0.0f, 0.0f, 0.0f);
    glm::vec3 direction(0.0f, -1.0f, 0.0f);
    
    for (const auto& test : tests) {
        float cutOff = glm::cos(glm::radians(test.innerAngle));
        float outerCutoff = glm::cos(glm::radians(test.outerAngle));
        
        ecs::Entity light = LightFactory::createSpotLight(
            position, direction, glm::vec3(1.0f), 0.09f, 0.032f, cutOff, outerCutoff
        );
        
        auto& spotLight = Application::m_coordinator->getComponent<components::SpotLightComponent>(light);
        EXPECT_FLOAT_EQ(spotLight.cutOff, cutOff);
        EXPECT_FLOAT_EQ(spotLight.outerCutoff, outerCutoff);
    }
}

// Multiple Light Creation Test
TEST_F(LightFactoryTest, CreateMultipleLights) {
    std::vector<ecs::Entity> lights;
    
    // Create one of each light type
    lights.push_back(LightFactory::createAmbientLight({0.1f, 0.1f, 0.1f}));
    lights.push_back(LightFactory::createDirectionalLight({0.0f, -1.0f, 0.0f}));
    lights.push_back(LightFactory::createPointLight({5.0f, 5.0f, 5.0f}));
    lights.push_back(LightFactory::createSpotLight({0.0f, 10.0f, 0.0f}, {0.0f, -1.0f, 0.0f}));
    
    // Verify all lights were created
    EXPECT_EQ(lights.size(), 4);
    
    // Verify all have unique entities
    std::set<ecs::Entity> uniqueEntities(lights.begin(), lights.end());
    EXPECT_EQ(uniqueEntities.size(), 4);
    
    // Verify all have UUID components
    for (auto light : lights) {
        EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::UuidComponent>(light));
        auto& uuid = Application::m_coordinator->getComponent<components::UuidComponent>(light);
        EXPECT_FALSE(uuid.uuid.empty());
    }
}

// Edge Case Tests
TEST_F(LightFactoryTest, CreateLights_ZeroValues) {
    // Test lights with zero values
    glm::vec3 zeroVector(0.0f, 0.0f, 0.0f);
    
    // Ambient light with black color
    ecs::Entity ambientLight = LightFactory::createAmbientLight(zeroVector);
    EXPECT_NE(ambientLight, ecs::INVALID_ENTITY);
    
    // Directional light with zero direction (edge case - should still create)
    ecs::Entity dirLight = LightFactory::createDirectionalLight(zeroVector);
    EXPECT_NE(dirLight, ecs::INVALID_ENTITY);
    
    // Point light at origin with black color
    ecs::Entity pointLight = LightFactory::createPointLight(zeroVector, zeroVector);
    EXPECT_NE(pointLight, ecs::INVALID_ENTITY);
    
    // Spot light with zero direction
    ecs::Entity spotLight = LightFactory::createSpotLight(zeroVector, zeroVector);
    EXPECT_NE(spotLight, ecs::INVALID_ENTITY);
}

TEST_F(LightFactoryTest, CreateLights_ExtremeValues) {
    // Test with extreme values
    glm::vec3 farPosition(10000.0f, 10000.0f, 10000.0f);
    glm::vec3 brightColor(10.0f, 10.0f, 10.0f); // HDR values
    float extremeLinear = 100.0f;
    float extremeQuadratic = 100.0f;
    
    // Point light with extreme values
    ecs::Entity pointLight = LightFactory::createPointLight(farPosition, brightColor, extremeLinear, extremeQuadratic);
    auto& pLight = Application::m_coordinator->getComponent<components::PointLightComponent>(pointLight);
    // Note: PointLightComponent doesn't store position - it's in TransformComponent
    EXPECT_EQ(pLight.color, brightColor);
    EXPECT_FLOAT_EQ(pLight.linear, extremeLinear);
    EXPECT_FLOAT_EQ(pLight.quadratic, extremeQuadratic);
    
    // Spot light with very narrow cone
    float veryNarrowCutOff = glm::cos(glm::radians(1.0f));
    float veryNarrowOuterCutOff = glm::cos(glm::radians(2.0f));
    ecs::Entity spotLight = LightFactory::createSpotLight(
        farPosition, glm::vec3(0.0f, -1.0f, 0.0f), brightColor,
        extremeLinear, extremeQuadratic, veryNarrowCutOff, veryNarrowOuterCutOff
    );
    EXPECT_NE(spotLight, ecs::INVALID_ENTITY);
}

} // namespace nexo