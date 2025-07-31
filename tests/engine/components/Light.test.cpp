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
//  Description: Test file for the Light components
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "components/Light.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

namespace nexo::components {

class LightComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test components
        ambientLight = AmbientLightComponent{};
        directionalLight = DirectionalLightComponent{};
        pointLight = PointLightComponent{};
        spotLight = SpotLightComponent{};
    }
    
    AmbientLightComponent ambientLight;
    DirectionalLightComponent directionalLight;
    PointLightComponent pointLight;
    SpotLightComponent spotLight;
};

// AmbientLightComponent Tests

TEST_F(LightComponentTest, AmbientLightDefaultConstruction) {
    AmbientLightComponent light;
    EXPECT_EQ(light.color, glm::vec3(0.0f));
}

TEST_F(LightComponentTest, AmbientLightValueConstruction) {
    ambientLight.color = glm::vec3(0.2f, 0.3f, 0.4f);
    EXPECT_EQ(ambientLight.color, glm::vec3(0.2f, 0.3f, 0.4f));
}

TEST_F(LightComponentTest, AmbientLightMemento) {
    // Set initial values
    ambientLight.color = glm::vec3(0.5f, 0.6f, 0.7f);
    
    // Save state
    auto memento = ambientLight.save();
    EXPECT_EQ(memento.color, glm::vec3(0.5f, 0.6f, 0.7f));
    
    // Modify values
    ambientLight.color = glm::vec3(0.1f, 0.1f, 0.1f);
    
    // Restore state
    ambientLight.restore(memento);
    EXPECT_EQ(ambientLight.color, glm::vec3(0.5f, 0.6f, 0.7f));
}

TEST_F(LightComponentTest, AmbientLightColorRange) {
    // Test various color values
    std::vector<glm::vec3> testColors = {
        glm::vec3(0.0f, 0.0f, 0.0f),  // Black
        glm::vec3(1.0f, 1.0f, 1.0f),  // White
        glm::vec3(1.0f, 0.0f, 0.0f),  // Red
        glm::vec3(0.0f, 1.0f, 0.0f),  // Green
        glm::vec3(0.0f, 0.0f, 1.0f),  // Blue
        glm::vec3(0.5f, 0.5f, 0.5f),  // Gray
        glm::vec3(2.0f, 2.0f, 2.0f),  // HDR values
        glm::vec3(-1.0f, -1.0f, -1.0f) // Negative (edge case)
    };
    
    for (const auto& color : testColors) {
        ambientLight.color = color;
        EXPECT_EQ(ambientLight.color, color);
    }
}

// DirectionalLightComponent Tests

TEST_F(LightComponentTest, DirectionalLightDefaultConstruction) {
    DirectionalLightComponent light;
    EXPECT_EQ(light.direction, glm::vec3(0.0f));
    EXPECT_EQ(light.color, glm::vec3(0.0f));
}

TEST_F(LightComponentTest, DirectionalLightParameterizedConstruction) {
    DirectionalLightComponent light(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.95f, 0.8f));
    EXPECT_EQ(light.direction, glm::vec3(0.0f, -1.0f, 0.0f));
    EXPECT_EQ(light.color, glm::vec3(1.0f, 0.95f, 0.8f));
    
    // Test with default color
    DirectionalLightComponent light2(glm::vec3(1.0f, 0.0f, 0.0f));
    EXPECT_EQ(light2.direction, glm::vec3(1.0f, 0.0f, 0.0f));
    EXPECT_EQ(light2.color, glm::vec3(1.0f, 1.0f, 1.0f));
}

TEST_F(LightComponentTest, DirectionalLightMemento) {
    // Set initial values
    directionalLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    directionalLight.color = glm::vec3(1.0f, 0.9f, 0.8f);
    
    // Save state
    auto memento = directionalLight.save();
    EXPECT_EQ(memento.direction, glm::vec3(0.0f, -1.0f, 0.0f));
    EXPECT_EQ(memento.color, glm::vec3(1.0f, 0.9f, 0.8f));
    
    // Modify values
    directionalLight.direction = glm::vec3(1.0f, 0.0f, 0.0f);
    directionalLight.color = glm::vec3(0.0f, 0.0f, 1.0f);
    
    // Restore state
    directionalLight.restore(memento);
    EXPECT_EQ(directionalLight.direction, glm::vec3(0.0f, -1.0f, 0.0f));
    EXPECT_EQ(directionalLight.color, glm::vec3(1.0f, 0.9f, 0.8f));
}

TEST_F(LightComponentTest, DirectionalLightNormalizedDirection) {
    // Test that direction can be normalized
    directionalLight.direction = glm::vec3(3.0f, 4.0f, 0.0f);
    glm::vec3 normalized = glm::normalize(directionalLight.direction);
    
    EXPECT_FLOAT_EQ(glm::length(normalized), 1.0f);
    EXPECT_FLOAT_EQ(normalized.x, 0.6f);
    EXPECT_FLOAT_EQ(normalized.y, 0.8f);
    EXPECT_FLOAT_EQ(normalized.z, 0.0f);
}

// PointLightComponent Tests

TEST_F(LightComponentTest, PointLightDefaultConstruction) {
    PointLightComponent light;
    EXPECT_EQ(light.color, glm::vec3(0.0f));
    EXPECT_FLOAT_EQ(light.linear, 0.0f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.0f);
    EXPECT_FLOAT_EQ(light.maxDistance, 50.0f);
    EXPECT_FLOAT_EQ(light.constant, 1.0f);
}

TEST_F(LightComponentTest, PointLightValueAssignment) {
    pointLight.color = glm::vec3(1.0f, 0.5f, 0.0f);
    pointLight.linear = 0.09f;
    pointLight.quadratic = 0.032f;
    pointLight.maxDistance = 100.0f;
    pointLight.constant = 1.0f;
    
    EXPECT_EQ(pointLight.color, glm::vec3(1.0f, 0.5f, 0.0f));
    EXPECT_FLOAT_EQ(pointLight.linear, 0.09f);
    EXPECT_FLOAT_EQ(pointLight.quadratic, 0.032f);
    EXPECT_FLOAT_EQ(pointLight.maxDistance, 100.0f);
    EXPECT_FLOAT_EQ(pointLight.constant, 1.0f);
}

TEST_F(LightComponentTest, PointLightMemento) {
    // Set initial values
    pointLight.color = glm::vec3(1.0f, 1.0f, 0.0f);
    pointLight.linear = 0.14f;
    pointLight.quadratic = 0.07f;
    pointLight.maxDistance = 75.0f;
    pointLight.constant = 0.5f;
    
    // Save state
    auto memento = pointLight.save();
    
    // Modify values
    pointLight.color = glm::vec3(0.0f, 0.0f, 1.0f);
    pointLight.linear = 0.0f;
    pointLight.quadratic = 1.0f;
    pointLight.maxDistance = 10.0f;
    pointLight.constant = 2.0f;
    
    // Restore state
    pointLight.restore(memento);
    EXPECT_EQ(pointLight.color, glm::vec3(1.0f, 1.0f, 0.0f));
    EXPECT_FLOAT_EQ(pointLight.linear, 0.14f);
    EXPECT_FLOAT_EQ(pointLight.quadratic, 0.07f);
    EXPECT_FLOAT_EQ(pointLight.maxDistance, 75.0f);
    EXPECT_FLOAT_EQ(pointLight.constant, 0.5f);
}

TEST_F(LightComponentTest, PointLightAttenuation) {
    // Test attenuation calculation at various distances
    pointLight.constant = 1.0f;
    pointLight.linear = 0.09f;
    pointLight.quadratic = 0.032f;
    
    // Calculate attenuation at different distances
    std::vector<float> distances = {1.0f, 5.0f, 10.0f, 20.0f, 50.0f};
    for (float distance : distances) {
        float attenuation = 1.0f / (pointLight.constant + 
                                   pointLight.linear * distance + 
                                   pointLight.quadratic * distance * distance);
        
        // Verify attenuation decreases with distance
        if (distance > 1.0f) {
            EXPECT_LT(attenuation, 1.0f);
        }
    }
}

// SpotLightComponent Tests

TEST_F(LightComponentTest, SpotLightDefaultConstruction) {
    SpotLightComponent light;
    EXPECT_EQ(light.direction, glm::vec3(0.0f));
    EXPECT_EQ(light.color, glm::vec3(0.0f));
    EXPECT_FLOAT_EQ(light.cutOff, 0.0f);
    EXPECT_FLOAT_EQ(light.outerCutoff, 0.0f);
    EXPECT_FLOAT_EQ(light.linear, 0.0f);
    EXPECT_FLOAT_EQ(light.quadratic, 0.0f);
    EXPECT_FLOAT_EQ(light.maxDistance, 325.0f);
    EXPECT_FLOAT_EQ(light.constant, 1.0f);
}

TEST_F(LightComponentTest, SpotLightValueAssignment) {
    spotLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    spotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLight.cutOff = glm::cos(glm::radians(12.5f));
    spotLight.outerCutoff = glm::cos(glm::radians(17.5f));
    spotLight.linear = 0.09f;
    spotLight.quadratic = 0.032f;
    spotLight.maxDistance = 200.0f;
    spotLight.constant = 1.0f;
    
    EXPECT_EQ(spotLight.direction, glm::vec3(0.0f, -1.0f, 0.0f));
    EXPECT_EQ(spotLight.color, glm::vec3(1.0f, 1.0f, 1.0f));
    EXPECT_FLOAT_EQ(spotLight.cutOff, glm::cos(glm::radians(12.5f)));
    EXPECT_FLOAT_EQ(spotLight.outerCutoff, glm::cos(glm::radians(17.5f)));
    EXPECT_FLOAT_EQ(spotLight.linear, 0.09f);
    EXPECT_FLOAT_EQ(spotLight.quadratic, 0.032f);
    EXPECT_FLOAT_EQ(spotLight.maxDistance, 200.0f);
    EXPECT_FLOAT_EQ(spotLight.constant, 1.0f);
}

TEST_F(LightComponentTest, SpotLightMemento) {
    // Set all values
    spotLight.direction = glm::vec3(1.0f, -1.0f, 0.0f);
    spotLight.color = glm::vec3(0.8f, 0.8f, 1.0f);
    spotLight.cutOff = 0.95f;
    spotLight.outerCutoff = 0.90f;
    spotLight.linear = 0.05f;
    spotLight.quadratic = 0.01f;
    spotLight.maxDistance = 500.0f;
    spotLight.constant = 0.8f;
    
    // Save state
    auto memento = spotLight.save();
    
    // Verify memento contents
    EXPECT_EQ(memento.direction, glm::vec3(1.0f, -1.0f, 0.0f));
    EXPECT_EQ(memento.color, glm::vec3(0.8f, 0.8f, 1.0f));
    EXPECT_FLOAT_EQ(memento.cutOff, 0.95f);
    EXPECT_FLOAT_EQ(memento.outerCutoff, 0.90f);
    EXPECT_FLOAT_EQ(memento.linear, 0.05f);
    EXPECT_FLOAT_EQ(memento.quadratic, 0.01f);
    EXPECT_FLOAT_EQ(memento.maxDistance, 500.0f);
    EXPECT_FLOAT_EQ(memento.constant, 0.8f);
    
    // Modify all values
    spotLight.direction = glm::vec3(0.0f, 0.0f, -1.0f);
    spotLight.color = glm::vec3(1.0f, 0.0f, 0.0f);
    spotLight.cutOff = 0.5f;
    spotLight.outerCutoff = 0.4f;
    spotLight.linear = 1.0f;
    spotLight.quadratic = 2.0f;
    spotLight.maxDistance = 10.0f;
    spotLight.constant = 3.0f;
    
    // Restore state
    spotLight.restore(memento);
    EXPECT_EQ(spotLight.direction, glm::vec3(1.0f, -1.0f, 0.0f));
    EXPECT_EQ(spotLight.color, glm::vec3(0.8f, 0.8f, 1.0f));
    EXPECT_FLOAT_EQ(spotLight.cutOff, 0.95f);
    EXPECT_FLOAT_EQ(spotLight.outerCutoff, 0.90f);
    EXPECT_FLOAT_EQ(spotLight.linear, 0.05f);
    EXPECT_FLOAT_EQ(spotLight.quadratic, 0.01f);
    EXPECT_FLOAT_EQ(spotLight.maxDistance, 500.0f);
    EXPECT_FLOAT_EQ(spotLight.constant, 0.8f);
}

TEST_F(LightComponentTest, SpotLightConeAngles) {
    // Test typical flashlight cone
    float innerAngle = 12.5f;
    float outerAngle = 17.5f;
    
    spotLight.cutOff = glm::cos(glm::radians(innerAngle));
    spotLight.outerCutoff = glm::cos(glm::radians(outerAngle));
    
    // Inner cutoff should be greater than outer (cosine decreases as angle increases)
    EXPECT_GT(spotLight.cutOff, spotLight.outerCutoff);
    
    // Test edge cases
    spotLight.cutOff = 1.0f;      // 0 degrees (laser)
    spotLight.outerCutoff = 0.0f;  // 90 degrees
    EXPECT_FLOAT_EQ(spotLight.cutOff, 1.0f);
    EXPECT_FLOAT_EQ(spotLight.outerCutoff, 0.0f);
    
    // Test invalid case (should be handled by application logic)
    spotLight.cutOff = 0.0f;
    spotLight.outerCutoff = 1.0f;  // Outer smaller than inner angle
    // System should handle this case appropriately
}

// LightContext Tests

TEST_F(LightComponentTest, LightContextDefaultConstruction) {
    LightContext context;
    
    EXPECT_EQ(context.ambientLight, glm::vec3(0.0f));
    EXPECT_EQ(context.pointLightCount, 0);
    EXPECT_EQ(context.spotLightCount, 0);
    
    // Arrays should be initialized
    EXPECT_EQ(context.pointLights.size(), MAX_POINT_LIGHTS);
    EXPECT_EQ(context.spotLights.size(), MAX_SPOT_LIGHTS);
}

TEST_F(LightComponentTest, LightContextAddLights) {
    LightContext context;
    
    // Set ambient light
    context.ambientLight = glm::vec3(0.1f, 0.1f, 0.1f);
    
    // Add directional light
    context.dirLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    context.dirLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    
    // Add point lights
    for (unsigned int i = 0; i < 5; ++i) {
        context.pointLights[i] = static_cast<ecs::Entity>(i + 100);
        context.pointLightCount++;
    }
    
    // Add spot lights
    for (unsigned int i = 0; i < 3; ++i) {
        context.spotLights[i] = static_cast<ecs::Entity>(i + 200);
        context.spotLightCount++;
    }
    
    // Verify
    EXPECT_EQ(context.ambientLight, glm::vec3(0.1f, 0.1f, 0.1f));
    EXPECT_EQ(context.dirLight.direction, glm::vec3(0.0f, -1.0f, 0.0f));
    EXPECT_EQ(context.pointLightCount, 5);
    EXPECT_EQ(context.spotLightCount, 3);
    
    // Verify entity IDs
    for (unsigned int i = 0; i < context.pointLightCount; ++i) {
        EXPECT_EQ(context.pointLights[i], static_cast<ecs::Entity>(i + 100));
    }
    for (unsigned int i = 0; i < context.spotLightCount; ++i) {
        EXPECT_EQ(context.spotLights[i], static_cast<ecs::Entity>(i + 200));
    }
}

TEST_F(LightComponentTest, LightContextMaxLimits) {
    LightContext context;
    
    // Fill to max capacity
    for (unsigned int i = 0; i < MAX_POINT_LIGHTS; ++i) {
        context.pointLights[i] = static_cast<ecs::Entity>(i);
    }
    context.pointLightCount = MAX_POINT_LIGHTS;
    
    for (unsigned int i = 0; i < MAX_SPOT_LIGHTS; ++i) {
        context.spotLights[i] = static_cast<ecs::Entity>(i + 1000);
    }
    context.spotLightCount = MAX_SPOT_LIGHTS;
    
    EXPECT_EQ(context.pointLightCount, MAX_POINT_LIGHTS);
    EXPECT_EQ(context.spotLightCount, MAX_SPOT_LIGHTS);
}

// Component Copy/Move Tests

TEST_F(LightComponentTest, AmbientLightCopySemantics) {
    ambientLight.color = glm::vec3(0.5f, 0.5f, 0.5f);
    
    // Copy construction
    AmbientLightComponent copy(ambientLight);
    EXPECT_EQ(copy.color, ambientLight.color);
    
    // Copy assignment
    AmbientLightComponent assigned;
    assigned = ambientLight;
    EXPECT_EQ(assigned.color, ambientLight.color);
    
    // Independence after copy
    copy.color = glm::vec3(1.0f, 0.0f, 0.0f);
    EXPECT_NE(copy.color, ambientLight.color);
}

TEST_F(LightComponentTest, DirectionalLightCopySemantics) {
    directionalLight.direction = glm::vec3(1.0f, -1.0f, 0.0f);
    directionalLight.color = glm::vec3(1.0f, 0.9f, 0.8f);
    
    // Copy construction
    DirectionalLightComponent copy(directionalLight);
    EXPECT_EQ(copy.direction, directionalLight.direction);
    EXPECT_EQ(copy.color, directionalLight.color);
    
    // Move construction
    DirectionalLightComponent moved(std::move(copy));
    EXPECT_EQ(moved.direction, directionalLight.direction);
    EXPECT_EQ(moved.color, directionalLight.color);
}

// HDR Lighting Tests

TEST_F(LightComponentTest, HDRLightingValues) {
    // Test HDR values (greater than 1.0)
    ambientLight.color = glm::vec3(2.0f, 2.5f, 3.0f);
    EXPECT_EQ(ambientLight.color, glm::vec3(2.0f, 2.5f, 3.0f));
    
    directionalLight.color = glm::vec3(5.0f, 5.0f, 5.0f);
    EXPECT_EQ(directionalLight.color, glm::vec3(5.0f, 5.0f, 5.0f));
    
    pointLight.color = glm::vec3(10.0f, 8.0f, 6.0f);
    EXPECT_EQ(pointLight.color, glm::vec3(10.0f, 8.0f, 6.0f));
    
    spotLight.color = glm::vec3(20.0f, 15.0f, 10.0f);
    EXPECT_EQ(spotLight.color, glm::vec3(20.0f, 15.0f, 10.0f));
}

// Edge Case Tests

TEST_F(LightComponentTest, ZeroIntensityLights) {
    // Test lights with zero color (no contribution)
    ambientLight.color = glm::vec3(0.0f);
    directionalLight.color = glm::vec3(0.0f);
    pointLight.color = glm::vec3(0.0f);
    spotLight.color = glm::vec3(0.0f);
    
    // All should be black
    EXPECT_EQ(ambientLight.color, glm::vec3(0.0f));
    EXPECT_EQ(directionalLight.color, glm::vec3(0.0f));
    EXPECT_EQ(pointLight.color, glm::vec3(0.0f));
    EXPECT_EQ(spotLight.color, glm::vec3(0.0f));
}

TEST_F(LightComponentTest, ExtremeAttenuationValues) {
    // Very high attenuation (light falls off quickly)
    pointLight.linear = 10.0f;
    pointLight.quadratic = 100.0f;
    
    // Very low attenuation (light travels far)
    spotLight.linear = 0.0001f;
    spotLight.quadratic = 0.00001f;
    
    EXPECT_FLOAT_EQ(pointLight.linear, 10.0f);
    EXPECT_FLOAT_EQ(pointLight.quadratic, 100.0f);
    EXPECT_FLOAT_EQ(spotLight.linear, 0.0001f);
    EXPECT_FLOAT_EQ(spotLight.quadratic, 0.00001f);
}

} // namespace nexo::components