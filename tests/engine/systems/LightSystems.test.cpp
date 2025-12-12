//// LightSystems.test.cpp ///////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/13/2025
//  Description: Test file for Light Systems (Ambient, Directional, Point, Spot)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <glm/glm.hpp>

#include "systems/lights/AmbientLightSystem.hpp"
#include "systems/lights/DirectionalLightsSystem.hpp"
#include "systems/lights/PointLightsSystem.hpp"
#include "systems/lights/SpotLightsSystem.hpp"
#include "components/Light.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "ecs/Coordinator.hpp"
#include "core/exceptions/Exceptions.hpp"

namespace nexo::system {

// =============================================================================
// Test Fixture Base Class
// =============================================================================

class LightSystemTestBase : public ::testing::Test {
protected:
    std::shared_ptr<ecs::Coordinator> coordinator;
    std::vector<ecs::Entity> entities;

    void SetUp() override {
        coordinator = std::make_shared<ecs::Coordinator>();
        coordinator->init();
        ecs::System::coord = coordinator;

        // Register components
        coordinator->registerComponent<components::AmbientLightComponent>();
        coordinator->registerComponent<components::DirectionalLightComponent>();
        coordinator->registerComponent<components::PointLightComponent>();
        coordinator->registerComponent<components::SpotLightComponent>();
        coordinator->registerComponent<components::SceneTag>();
        coordinator->registerComponent<components::TransformComponent>();

        // Register singleton component
        coordinator->registerSingletonComponent<components::RenderContext>();
    }

    void TearDown() override {
        for (auto entity : entities) {
            coordinator->destroyEntity(entity);
        }
        entities.clear();
        ecs::System::coord = nullptr;
    }

    // Helper to create an entity with scene tag
    ecs::Entity createEntityInScene(unsigned int sceneId) {
        ecs::Entity entity = coordinator->createEntity();
        entities.push_back(entity);

        components::SceneTag sceneTag;
        sceneTag.id = sceneId;
        coordinator->addComponent(entity, sceneTag);

        return entity;
    }

    // Helper to set the rendered scene
    void setRenderedScene(int sceneId) {
        auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
        renderContext.sceneRendered = sceneId;
    }

    // Helper to get render context
    components::RenderContext& getRenderContext() {
        return coordinator->getSingletonComponent<components::RenderContext>();
    }

    // Helper to reset light context
    void resetLightContext() {
        auto& renderContext = getRenderContext();
        renderContext.sceneLights.ambientLight = glm::vec3(0.0f);
        renderContext.sceneLights.pointLightCount = 0;
        renderContext.sceneLights.spotLightCount = 0;
        renderContext.sceneLights.dirLight = components::DirectionalLightComponent{};
    }
};

// =============================================================================
// AmbientLightSystem Tests
// =============================================================================

class AmbientLightSystemTest : public LightSystemTestBase {
protected:
    std::shared_ptr<AmbientLightSystem> system;

    void SetUp() override {
        LightSystemTestBase::SetUp();
        system = coordinator->registerGroupSystem<AmbientLightSystem>();
    }
};

TEST_F(AmbientLightSystemTest, UpdateWithNoSceneRendered) {
    // Create ambient light in scene 0
    auto entity = createEntityInScene(0);
    components::AmbientLightComponent ambient;
    ambient.color = glm::vec3(0.5f, 0.5f, 0.5f);
    coordinator->addComponent(entity, ambient);

    // Set sceneRendered to -1 (no scene)
    setRenderedScene(-1);

    // Call update
    system->update();

    // Ambient light should NOT be updated (still default)
    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.0f));
}

TEST_F(AmbientLightSystemTest, UpdateWithSingleAmbientLight) {
    // Create ambient light in scene 0
    auto entity = createEntityInScene(0);
    components::AmbientLightComponent ambient;
    ambient.color = glm::vec3(0.3f, 0.4f, 0.5f);
    coordinator->addComponent(entity, ambient);

    setRenderedScene(0);
    system->update();

    // Verify ambient light is set correctly
    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.3f, 0.4f, 0.5f));
}

TEST_F(AmbientLightSystemTest, UpdateWithMultipleAmbientLights) {
    // Create multiple ambient lights in scene 0
    auto entity1 = createEntityInScene(0);
    components::AmbientLightComponent ambient1;
    ambient1.color = glm::vec3(0.2f, 0.3f, 0.4f);
    coordinator->addComponent(entity1, ambient1);

    auto entity2 = createEntityInScene(0);
    components::AmbientLightComponent ambient2;
    ambient2.color = glm::vec3(0.8f, 0.9f, 1.0f);
    coordinator->addComponent(entity2, ambient2);

    setRenderedScene(0);
    system->update();

    // Should use the first ambient light
    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.2f, 0.3f, 0.4f));
}

TEST_F(AmbientLightSystemTest, UpdateOnlyAffectsCurrentScene) {
    // Create ambient lights in different scenes
    auto scene0Entity = createEntityInScene(0);
    components::AmbientLightComponent ambient0;
    ambient0.color = glm::vec3(0.1f, 0.1f, 0.1f);
    coordinator->addComponent(scene0Entity, ambient0);

    auto scene1Entity = createEntityInScene(1);
    components::AmbientLightComponent ambient1;
    ambient1.color = glm::vec3(0.9f, 0.9f, 0.9f);
    coordinator->addComponent(scene1Entity, ambient1);

    // Render scene 1
    setRenderedScene(1);
    system->update();

    // Should use scene 1's ambient light
    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.9f, 0.9f, 0.9f));
}

TEST_F(AmbientLightSystemTest, UpdateWithDifferentColorValues) {
    auto entity = createEntityInScene(0);
    components::AmbientLightComponent ambient;
    ambient.color = glm::vec3(1.0f, 0.0f, 0.5f);
    coordinator->addComponent(entity, ambient);

    setRenderedScene(0);
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_FLOAT_EQ(renderContext.sceneLights.ambientLight.r, 1.0f);
    EXPECT_FLOAT_EQ(renderContext.sceneLights.ambientLight.g, 0.0f);
    EXPECT_FLOAT_EQ(renderContext.sceneLights.ambientLight.b, 0.5f);
}

// =============================================================================
// DirectionalLightsSystem Tests
// =============================================================================

class DirectionalLightsSystemTest : public LightSystemTestBase {
protected:
    std::shared_ptr<DirectionalLightsSystem> system;

    void SetUp() override {
        LightSystemTestBase::SetUp();
        system = coordinator->registerGroupSystem<DirectionalLightsSystem>();
    }
};

TEST_F(DirectionalLightsSystemTest, UpdateWithNoSceneRendered) {
    // Create directional light in scene 0
    auto entity = createEntityInScene(0);
    components::DirectionalLightComponent dirLight;
    dirLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    dirLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(entity, dirLight);

    setRenderedScene(-1);
    system->update();

    // Directional light should NOT be updated (still default)
    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.dirLight.direction, glm::vec3(0.0f));
    EXPECT_EQ(renderContext.sceneLights.dirLight.color, glm::vec3(0.0f));
}

TEST_F(DirectionalLightsSystemTest, UpdateWithSingleDirectionalLight) {
    auto entity = createEntityInScene(0);
    components::DirectionalLightComponent dirLight;
    dirLight.direction = glm::vec3(1.0f, -1.0f, 0.0f);
    dirLight.color = glm::vec3(0.8f, 0.9f, 1.0f);
    coordinator->addComponent(entity, dirLight);

    setRenderedScene(0);
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.dirLight.direction, glm::vec3(1.0f, -1.0f, 0.0f));
    EXPECT_EQ(renderContext.sceneLights.dirLight.color, glm::vec3(0.8f, 0.9f, 1.0f));
}

TEST_F(DirectionalLightsSystemTest, UpdateWithMultipleDirectionalLights) {
    // Create multiple directional lights in scene 0
    auto entity1 = createEntityInScene(0);
    components::DirectionalLightComponent dirLight1;
    dirLight1.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    dirLight1.color = glm::vec3(1.0f, 0.0f, 0.0f);
    coordinator->addComponent(entity1, dirLight1);

    auto entity2 = createEntityInScene(0);
    components::DirectionalLightComponent dirLight2;
    dirLight2.direction = glm::vec3(1.0f, 0.0f, 0.0f);
    dirLight2.color = glm::vec3(0.0f, 1.0f, 0.0f);
    coordinator->addComponent(entity2, dirLight2);

    setRenderedScene(0);
    system->update();

    // Should use the first directional light
    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.dirLight.direction, glm::vec3(0.0f, -1.0f, 0.0f));
    EXPECT_EQ(renderContext.sceneLights.dirLight.color, glm::vec3(1.0f, 0.0f, 0.0f));
}

TEST_F(DirectionalLightsSystemTest, UpdateOnlyAffectsCurrentScene) {
    auto scene0Entity = createEntityInScene(0);
    components::DirectionalLightComponent dirLight0;
    dirLight0.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    dirLight0.color = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(scene0Entity, dirLight0);

    auto scene1Entity = createEntityInScene(1);
    components::DirectionalLightComponent dirLight1;
    dirLight1.direction = glm::vec3(1.0f, 0.0f, 0.0f);
    dirLight1.color = glm::vec3(0.5f, 0.5f, 0.5f);
    coordinator->addComponent(scene1Entity, dirLight1);

    // Render scene 1
    setRenderedScene(1);
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.dirLight.direction, glm::vec3(1.0f, 0.0f, 0.0f));
    EXPECT_EQ(renderContext.sceneLights.dirLight.color, glm::vec3(0.5f, 0.5f, 0.5f));
}

TEST_F(DirectionalLightsSystemTest, UpdateWithNonNormalizedDirection) {
    auto entity = createEntityInScene(0);
    components::DirectionalLightComponent dirLight;
    dirLight.direction = glm::vec3(5.0f, -5.0f, 0.0f);  // Non-normalized
    dirLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(entity, dirLight);

    setRenderedScene(0);
    system->update();

    // Direction should be stored as-is (normalization happens in shader/rendering)
    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.dirLight.direction, glm::vec3(5.0f, -5.0f, 0.0f));
}

TEST_F(DirectionalLightsSystemTest, UpdateWithVariousColorValues) {
    auto entity = createEntityInScene(0);
    components::DirectionalLightComponent dirLight;
    dirLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    dirLight.color = glm::vec3(0.25f, 0.75f, 0.5f);
    coordinator->addComponent(entity, dirLight);

    setRenderedScene(0);
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_FLOAT_EQ(renderContext.sceneLights.dirLight.color.r, 0.25f);
    EXPECT_FLOAT_EQ(renderContext.sceneLights.dirLight.color.g, 0.75f);
    EXPECT_FLOAT_EQ(renderContext.sceneLights.dirLight.color.b, 0.5f);
}

// =============================================================================
// PointLightsSystem Tests
// =============================================================================

class PointLightsSystemTest : public LightSystemTestBase {
protected:
    std::shared_ptr<PointLightsSystem> system;

    void SetUp() override {
        LightSystemTestBase::SetUp();
        system = coordinator->registerGroupSystem<PointLightsSystem>();
    }
};

TEST_F(PointLightsSystemTest, UpdateWithNoSceneRendered) {
    auto entity = createEntityInScene(0);
    components::PointLightComponent pointLight;
    pointLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(entity, pointLight);

    setRenderedScene(-1);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 0u);
}

TEST_F(PointLightsSystemTest, UpdateWithSinglePointLight) {
    auto entity = createEntityInScene(0);
    components::PointLightComponent pointLight;
    pointLight.color = glm::vec3(1.0f, 0.5f, 0.0f);
    pointLight.constant = 1.0f;
    pointLight.linear = 0.09f;
    pointLight.quadratic = 0.032f;
    coordinator->addComponent(entity, pointLight);

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 1u);
    EXPECT_EQ(renderContext.sceneLights.pointLights[0], entity);
}

TEST_F(PointLightsSystemTest, UpdateWithFourPointLights) {
    for (int i = 0; i < 4; ++i) {
        auto entity = createEntityInScene(0);
        components::PointLightComponent pointLight;
        pointLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
        coordinator->addComponent(entity, pointLight);
    }

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 4u);
}

TEST_F(PointLightsSystemTest, UpdateWithEightPointLights) {
    for (int i = 0; i < 8; ++i) {
        auto entity = createEntityInScene(0);
        components::PointLightComponent pointLight;
        pointLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
        coordinator->addComponent(entity, pointLight);
    }

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 8u);
}

TEST_F(PointLightsSystemTest, UpdateWithMaxPointLights) {
    for (unsigned int i = 0; i < MAX_POINT_LIGHTS; ++i) {
        auto entity = createEntityInScene(0);
        components::PointLightComponent pointLight;
        pointLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
        coordinator->addComponent(entity, pointLight);
    }

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, MAX_POINT_LIGHTS);
}

TEST_F(PointLightsSystemTest, UpdateWithTooManyPointLightsThrowsException) {
    // Create more than MAX_POINT_LIGHTS
    for (unsigned int i = 0; i < MAX_POINT_LIGHTS + 1; ++i) {
        auto entity = createEntityInScene(0);
        components::PointLightComponent pointLight;
        pointLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
        coordinator->addComponent(entity, pointLight);
    }

    setRenderedScene(0);
    resetLightContext();

    EXPECT_THROW({
        system->update();
    }, core::TooManyPointLightsException);
}

TEST_F(PointLightsSystemTest, UpdateOnlyAffectsCurrentScene) {
    // Create point lights in different scenes
    auto scene0Entity = createEntityInScene(0);
    components::PointLightComponent pointLight0;
    pointLight0.color = glm::vec3(1.0f, 0.0f, 0.0f);
    coordinator->addComponent(scene0Entity, pointLight0);

    auto scene1Entity = createEntityInScene(1);
    components::PointLightComponent pointLight1;
    pointLight1.color = glm::vec3(0.0f, 1.0f, 0.0f);
    coordinator->addComponent(scene1Entity, pointLight1);

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 1u);
    EXPECT_EQ(renderContext.sceneLights.pointLights[0], scene0Entity);
}

TEST_F(PointLightsSystemTest, UpdateWithAttenuationParameters) {
    auto entity = createEntityInScene(0);
    components::PointLightComponent pointLight;
    pointLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLight.constant = 1.0f;
    pointLight.linear = 0.14f;
    pointLight.quadratic = 0.07f;
    pointLight.maxDistance = 100.0f;
    coordinator->addComponent(entity, pointLight);

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 1u);

    // Verify the entity is correctly stored
    auto& storedLight = coordinator->getComponent<components::PointLightComponent>(
        renderContext.sceneLights.pointLights[0]
    );
    EXPECT_FLOAT_EQ(storedLight.constant, 1.0f);
    EXPECT_FLOAT_EQ(storedLight.linear, 0.14f);
    EXPECT_FLOAT_EQ(storedLight.quadratic, 0.07f);
    EXPECT_FLOAT_EQ(storedLight.maxDistance, 100.0f);
}

TEST_F(PointLightsSystemTest, UpdatePreservesEntityOrder) {
    std::vector<ecs::Entity> createdEntities;
    for (int i = 0; i < 3; ++i) {
        auto entity = createEntityInScene(0);
        components::PointLightComponent pointLight;
        pointLight.color = glm::vec3(static_cast<float>(i) / 3.0f, 1.0f, 1.0f);
        coordinator->addComponent(entity, pointLight);
        createdEntities.push_back(entity);
    }

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 3u);

    // Verify entities are in order
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(renderContext.sceneLights.pointLights[i], createdEntities[i]);
    }
}

// =============================================================================
// SpotLightsSystem Tests
// =============================================================================

class SpotLightsSystemTest : public LightSystemTestBase {
protected:
    std::shared_ptr<SpotLightsSystem> system;

    void SetUp() override {
        LightSystemTestBase::SetUp();
        system = coordinator->registerGroupSystem<SpotLightsSystem>();
    }
};

TEST_F(SpotLightsSystemTest, UpdateWithNoSceneRendered) {
    auto entity = createEntityInScene(0);
    components::SpotLightComponent spotLight;
    spotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    coordinator->addComponent(entity, spotLight);

    setRenderedScene(-1);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.spotLightCount, 0u);
}

TEST_F(SpotLightsSystemTest, UpdateWithSingleSpotLight) {
    auto entity = createEntityInScene(0);
    components::SpotLightComponent spotLight;
    spotLight.color = glm::vec3(1.0f, 1.0f, 0.0f);
    spotLight.direction = glm::vec3(0.0f, 0.0f, -1.0f);
    spotLight.cutOff = 12.5f;
    spotLight.outerCutoff = 17.5f;
    coordinator->addComponent(entity, spotLight);

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.spotLightCount, 1u);
    EXPECT_EQ(renderContext.sceneLights.spotLights[0], entity);
}

TEST_F(SpotLightsSystemTest, UpdateWithMultipleSpotLights) {
    for (int i = 0; i < 5; ++i) {
        auto entity = createEntityInScene(0);
        components::SpotLightComponent spotLight;
        spotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
        spotLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
        coordinator->addComponent(entity, spotLight);
    }

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.spotLightCount, 5u);
}

TEST_F(SpotLightsSystemTest, UpdateWithMaxSpotLights) {
    for (unsigned int i = 0; i < MAX_SPOT_LIGHTS; ++i) {
        auto entity = createEntityInScene(0);
        components::SpotLightComponent spotLight;
        spotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
        coordinator->addComponent(entity, spotLight);
    }

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.spotLightCount, MAX_SPOT_LIGHTS);
}

TEST_F(SpotLightsSystemTest, UpdateWithTooManySpotLightsThrowsException) {
    // Create more than MAX_SPOT_LIGHTS
    for (unsigned int i = 0; i < MAX_SPOT_LIGHTS + 1; ++i) {
        auto entity = createEntityInScene(0);
        components::SpotLightComponent spotLight;
        spotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
        coordinator->addComponent(entity, spotLight);
    }

    setRenderedScene(0);
    resetLightContext();

    EXPECT_THROW({
        system->update();
    }, core::TooManySpotLightsException);
}

TEST_F(SpotLightsSystemTest, UpdateOnlyAffectsCurrentScene) {
    auto scene0Entity = createEntityInScene(0);
    components::SpotLightComponent spotLight0;
    spotLight0.color = glm::vec3(1.0f, 0.0f, 0.0f);
    coordinator->addComponent(scene0Entity, spotLight0);

    auto scene1Entity = createEntityInScene(1);
    components::SpotLightComponent spotLight1;
    spotLight1.color = glm::vec3(0.0f, 1.0f, 0.0f);
    coordinator->addComponent(scene1Entity, spotLight1);

    setRenderedScene(1);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.spotLightCount, 1u);
    EXPECT_EQ(renderContext.sceneLights.spotLights[0], scene1Entity);
}

TEST_F(SpotLightsSystemTest, UpdateWithDirectionAndCutoffParameters) {
    auto entity = createEntityInScene(0);
    components::SpotLightComponent spotLight;
    spotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLight.direction = glm::vec3(1.0f, -1.0f, 0.0f);
    spotLight.cutOff = 10.0f;
    spotLight.outerCutoff = 15.0f;
    spotLight.constant = 1.0f;
    spotLight.linear = 0.09f;
    spotLight.quadratic = 0.032f;
    coordinator->addComponent(entity, spotLight);

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.spotLightCount, 1u);

    // Verify the entity is correctly stored
    auto& storedLight = coordinator->getComponent<components::SpotLightComponent>(
        renderContext.sceneLights.spotLights[0]
    );
    EXPECT_EQ(storedLight.direction, glm::vec3(1.0f, -1.0f, 0.0f));
    EXPECT_FLOAT_EQ(storedLight.cutOff, 10.0f);
    EXPECT_FLOAT_EQ(storedLight.outerCutoff, 15.0f);
    EXPECT_FLOAT_EQ(storedLight.constant, 1.0f);
    EXPECT_FLOAT_EQ(storedLight.linear, 0.09f);
    EXPECT_FLOAT_EQ(storedLight.quadratic, 0.032f);
}

TEST_F(SpotLightsSystemTest, UpdatePreservesEntityOrder) {
    std::vector<ecs::Entity> createdEntities;
    for (int i = 0; i < 3; ++i) {
        auto entity = createEntityInScene(0);
        components::SpotLightComponent spotLight;
        spotLight.color = glm::vec3(static_cast<float>(i) / 3.0f, 1.0f, 1.0f);
        coordinator->addComponent(entity, spotLight);
        createdEntities.push_back(entity);
    }

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.spotLightCount, 3u);

    // Verify entities are in order
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(renderContext.sceneLights.spotLights[i], createdEntities[i]);
    }
}

TEST_F(SpotLightsSystemTest, UpdateWithVaryingCutoffAngles) {
    auto entity = createEntityInScene(0);
    components::SpotLightComponent spotLight;
    spotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    spotLight.cutOff = 5.0f;  // Inner cutoff
    spotLight.outerCutoff = 20.0f;  // Outer cutoff
    coordinator->addComponent(entity, spotLight);

    setRenderedScene(0);
    resetLightContext();
    system->update();

    auto& renderContext = getRenderContext();
    auto& storedLight = coordinator->getComponent<components::SpotLightComponent>(
        renderContext.sceneLights.spotLights[0]
    );
    EXPECT_FLOAT_EQ(storedLight.cutOff, 5.0f);
    EXPECT_FLOAT_EQ(storedLight.outerCutoff, 20.0f);
}

// =============================================================================
// Integration Tests - Multiple Light Systems Together
// =============================================================================

class LightSystemsIntegrationTest : public LightSystemTestBase {
protected:
    std::shared_ptr<AmbientLightSystem> ambientSystem;
    std::shared_ptr<DirectionalLightsSystem> directionalSystem;
    std::shared_ptr<PointLightsSystem> pointSystem;
    std::shared_ptr<SpotLightsSystem> spotSystem;

    void SetUp() override {
        LightSystemTestBase::SetUp();
        ambientSystem = coordinator->registerGroupSystem<AmbientLightSystem>();
        directionalSystem = coordinator->registerGroupSystem<DirectionalLightsSystem>();
        pointSystem = coordinator->registerGroupSystem<PointLightsSystem>();
        spotSystem = coordinator->registerGroupSystem<SpotLightsSystem>();
    }
};

TEST_F(LightSystemsIntegrationTest, UpdateAllLightSystemsTogether) {
    // Create ambient light
    auto ambientEntity = createEntityInScene(0);
    components::AmbientLightComponent ambient;
    ambient.color = glm::vec3(0.2f, 0.2f, 0.2f);
    coordinator->addComponent(ambientEntity, ambient);

    // Create directional light
    auto dirEntity = createEntityInScene(0);
    components::DirectionalLightComponent dirLight;
    dirLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    dirLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(dirEntity, dirLight);

    // Create point lights
    for (int i = 0; i < 3; ++i) {
        auto pointEntity = createEntityInScene(0);
        components::PointLightComponent pointLight;
        pointLight.color = glm::vec3(1.0f, 0.5f, 0.0f);
        coordinator->addComponent(pointEntity, pointLight);
    }

    // Create spot lights
    for (int i = 0; i < 2; ++i) {
        auto spotEntity = createEntityInScene(0);
        components::SpotLightComponent spotLight;
        spotLight.color = glm::vec3(1.0f, 1.0f, 0.0f);
        spotLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
        coordinator->addComponent(spotEntity, spotLight);
    }

    setRenderedScene(0);
    resetLightContext();

    // Update all systems
    ambientSystem->update();
    directionalSystem->update();
    pointSystem->update();
    spotSystem->update();

    // Verify all lights are set
    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.2f, 0.2f, 0.2f));
    EXPECT_EQ(renderContext.sceneLights.dirLight.direction, glm::vec3(0.0f, -1.0f, 0.0f));
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 3u);
    EXPECT_EQ(renderContext.sceneLights.spotLightCount, 2u);
}

TEST_F(LightSystemsIntegrationTest, SwitchingSceneUpdatesAllLights) {
    // Scene 0 lights
    auto ambient0 = createEntityInScene(0);
    components::AmbientLightComponent ambientComp0;
    ambientComp0.color = glm::vec3(0.1f, 0.1f, 0.1f);
    coordinator->addComponent(ambient0, ambientComp0);

    auto point0 = createEntityInScene(0);
    components::PointLightComponent pointComp0;
    pointComp0.color = glm::vec3(1.0f, 0.0f, 0.0f);
    coordinator->addComponent(point0, pointComp0);

    // Scene 1 lights
    auto ambient1 = createEntityInScene(1);
    components::AmbientLightComponent ambientComp1;
    ambientComp1.color = glm::vec3(0.9f, 0.9f, 0.9f);
    coordinator->addComponent(ambient1, ambientComp1);

    auto point1 = createEntityInScene(1);
    components::PointLightComponent pointComp1;
    pointComp1.color = glm::vec3(0.0f, 1.0f, 0.0f);
    coordinator->addComponent(point1, pointComp1);

    // Update with scene 0
    setRenderedScene(0);
    resetLightContext();
    ambientSystem->update();
    pointSystem->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.1f, 0.1f, 0.1f));
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 1u);

    // Switch to scene 1
    setRenderedScene(1);
    resetLightContext();
    ambientSystem->update();
    pointSystem->update();

    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.9f, 0.9f, 0.9f));
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 1u);
}

TEST_F(LightSystemsIntegrationTest, EmptySceneHasNoLights) {
    setRenderedScene(0);
    resetLightContext();

    ambientSystem->update();
    directionalSystem->update();
    pointSystem->update();
    spotSystem->update();

    auto& renderContext = getRenderContext();
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.0f));
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 0u);
    EXPECT_EQ(renderContext.sceneLights.spotLightCount, 0u);
}

}  // namespace nexo::system
