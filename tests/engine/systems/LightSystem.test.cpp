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
//  Description: Test file for the Light systems
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "systems/LightSystem.hpp"
#include "systems/lights/AmbientLightSystem.hpp"
#include "systems/lights/DirectionalLightsSystem.hpp"
#include "systems/lights/PointLightsSystem.hpp"
#include "systems/lights/SpotLightsSystem.hpp"
#include "utils/TestHelpers.hpp"
#include "components/Light.hpp"
#include "components/Transform.hpp"
#include "components/SceneComponents.hpp"
#include "components/RenderContext.hpp"
#include "Application.hpp"
#include "scene/SceneManager.hpp"
#include "scene/Scene.hpp"

namespace nexo::system {

using namespace nexo::test;
using ::testing::_;
using ::testing::Return;

// Mock for Application
class MockApplication : public Application {
public:
    MockApplication() : Application() {
        m_instance = this;
        m_sceneManager = std::make_unique<scene::SceneManager>();
    }
    
    ~MockApplication() {
        m_instance = nullptr;
    }
    
    scene::SceneManager& getSceneManager() override { return *m_sceneManager; }
    
private:
    std::unique_ptr<scene::SceneManager> m_sceneManager;
};

// Base test fixture for light systems
class LightSystemTestBase : public EcsTestFixture {
protected:
    std::unique_ptr<MockApplication> mockApp;
    
    void SetUp() override {
        mockApp = std::make_unique<MockApplication>();
        EcsTestFixture::SetUp();
        
        // Register light components
        coordinator->registerComponent<components::AmbientLightComponent>();
        coordinator->registerComponent<components::DirectionalLightComponent>();
        coordinator->registerComponent<components::PointLightComponent>();
        coordinator->registerComponent<components::SpotLightComponent>();
        
        // Initialize render context
        components::RenderContext renderContext;
        coordinator->setSingletonComponent(renderContext);
        
        // Create test scene
        mockApp->getSceneManager().createScene("TestScene");
    }
    
    void TearDown() override {
        EcsTestFixture::TearDown();
        mockApp.reset();
    }
};

// Mock subsystems for LightSystem tests
class MockAmbientLightSystem : public AmbientLightSystem {
public:
    MOCK_METHOD(void, update, (), (override));
};

class MockDirectionalLightsSystem : public DirectionalLightsSystem {
public:
    MOCK_METHOD(void, update, (), (override));
};

class MockPointLightsSystem : public PointLightsSystem {
public:
    MOCK_METHOD(void, update, (), (override));
};

class MockSpotLightsSystem : public SpotLightsSystem {
public:
    MOCK_METHOD(void, update, (), (override));
};

// LightSystem Integration Tests
class LightSystemTest : public LightSystemTestBase {
protected:
    std::shared_ptr<MockAmbientLightSystem> mockAmbientSystem;
    std::shared_ptr<MockDirectionalLightsSystem> mockDirectionalSystem;
    std::shared_ptr<MockPointLightsSystem> mockPointSystem;
    std::shared_ptr<MockSpotLightsSystem> mockSpotSystem;
    std::unique_ptr<LightSystem> lightSystem;
    
    void SetUp() override {
        LightSystemTestBase::SetUp();
        
        // Create mock subsystems
        mockAmbientSystem = std::make_shared<MockAmbientLightSystem>();
        mockDirectionalSystem = std::make_shared<MockDirectionalLightsSystem>();
        mockPointSystem = std::make_shared<MockPointLightsSystem>();
        mockSpotSystem = std::make_shared<MockSpotLightsSystem>();
        
        // Create LightSystem
        lightSystem = std::make_unique<LightSystem>(
            mockAmbientSystem,
            mockDirectionalSystem,
            mockPointSystem,
            mockSpotSystem
        );
    }
};

TEST_F(LightSystemTest, UpdateCallsAllSubsystems) {
    // Expect all subsystems to be updated
    EXPECT_CALL(*mockAmbientSystem, update()).Times(1);
    EXPECT_CALL(*mockDirectionalSystem, update()).Times(1);
    EXPECT_CALL(*mockPointSystem, update()).Times(1);
    EXPECT_CALL(*mockSpotSystem, update()).Times(1);
    
    // Update light system
    lightSystem->update();
}

TEST_F(LightSystemTest, UpdateOrderIsConsistent) {
    // Use sequence to verify order
    ::testing::InSequence sequence;
    
    EXPECT_CALL(*mockAmbientSystem, update()).Times(1);
    EXPECT_CALL(*mockDirectionalSystem, update()).Times(1);
    EXPECT_CALL(*mockPointSystem, update()).Times(1);
    EXPECT_CALL(*mockSpotSystem, update()).Times(1);
    
    lightSystem->update();
}

// AmbientLightSystem Tests
class AmbientLightSystemTest : public LightSystemTestBase {
protected:
    std::shared_ptr<AmbientLightSystem> system;
    
    void SetUp() override {
        LightSystemTestBase::SetUp();
        
        // Create and register system
        system = coordinator->registerSystem<AmbientLightSystem>();
        
        // Set up system signature
        ecs::ComponentSignature signature;
        signature.set(coordinator->getComponentType<components::AmbientLightComponent>());
        signature.set(coordinator->getComponentType<components::SceneTag>());
        coordinator->setSystemSignature<AmbientLightSystem>(signature);
    }
    
    ecs::Entity createAmbientLight(unsigned int sceneId, const glm::vec3& color) {
        auto entity = coordinator->createEntity();
        
        components::AmbientLightComponent ambient;
        ambient.color = color;
        coordinator->addComponent(entity, ambient);
        coordinator->addComponent(entity, components::SceneTag{sceneId});
        
        return entity;
    }
};

TEST_F(AmbientLightSystemTest, NoSceneRendered) {
    setRenderContext(-1);
    
    createAmbientLight(0, glm::vec3(0.1f, 0.1f, 0.1f));
    
    system->update();
    
    // Light context should not be updated
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.0f));
}

TEST_F(AmbientLightSystemTest, SingleAmbientLight) {
    setRenderContext(0);
    
    createAmbientLight(0, glm::vec3(0.2f, 0.3f, 0.4f));
    
    system->update();
    
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.2f, 0.3f, 0.4f));
}

TEST_F(AmbientLightSystemTest, MultipleAmbientLights) {
    setRenderContext(0);
    
    // Create multiple ambient lights in same scene
    createAmbientLight(0, glm::vec3(0.1f, 0.1f, 0.1f));
    createAmbientLight(0, glm::vec3(0.2f, 0.2f, 0.2f));
    createAmbientLight(0, glm::vec3(0.3f, 0.3f, 0.3f));
    
    system->update();
    
    // Should use first one (warning logged)
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.1f, 0.1f, 0.1f));
}

TEST_F(AmbientLightSystemTest, SceneFiltering) {
    setRenderContext(1);
    
    createAmbientLight(0, glm::vec3(0.1f, 0.0f, 0.0f));
    createAmbientLight(1, glm::vec3(0.0f, 0.1f, 0.0f));
    createAmbientLight(2, glm::vec3(0.0f, 0.0f, 0.1f));
    
    system->update();
    
    // Only scene 1 light should be used
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.0f, 0.1f, 0.0f));
}

// DirectionalLightsSystem Tests
class DirectionalLightsSystemTest : public LightSystemTestBase {
protected:
    std::shared_ptr<DirectionalLightsSystem> system;
    
    void SetUp() override {
        LightSystemTestBase::SetUp();
        
        // Create and register system
        system = coordinator->registerSystem<DirectionalLightsSystem>();
        
        // Set up system signature
        ecs::ComponentSignature signature;
        signature.set(coordinator->getComponentType<components::DirectionalLightComponent>());
        signature.set(coordinator->getComponentType<components::SceneTag>());
        coordinator->setSystemSignature<DirectionalLightsSystem>(signature);
    }
    
    ecs::Entity createDirectionalLight(unsigned int sceneId, 
                                      const glm::vec3& direction,
                                      const glm::vec3& color) {
        auto entity = coordinator->createEntity();
        
        components::DirectionalLightComponent dirLight(direction, color);
        coordinator->addComponent(entity, dirLight);
        coordinator->addComponent(entity, components::SceneTag{sceneId});
        
        return entity;
    }
};

TEST_F(DirectionalLightsSystemTest, SingleDirectionalLight) {
    setRenderContext(0);
    
    createDirectionalLight(0, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.95f, 0.8f));
    
    system->update();
    
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.dirLight.direction, glm::vec3(0.0f, -1.0f, 0.0f));
    EXPECT_EQ(renderContext.sceneLights.dirLight.color, glm::vec3(1.0f, 0.95f, 0.8f));
}

TEST_F(DirectionalLightsSystemTest, MultipleDirectionalLights) {
    setRenderContext(0);
    
    // Create multiple directional lights
    createDirectionalLight(0, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    createDirectionalLight(0, glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    
    system->update();
    
    // Should use first one
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.dirLight.direction, glm::vec3(0.0f, -1.0f, 0.0f));
    EXPECT_EQ(renderContext.sceneLights.dirLight.color, glm::vec3(1.0f, 1.0f, 1.0f));
}

// PointLightsSystem Tests
class PointLightsSystemTest : public LightSystemTestBase {
protected:
    std::shared_ptr<PointLightsSystem> system;
    
    void SetUp() override {
        LightSystemTestBase::SetUp();
        
        // Create and register system
        system = coordinator->registerSystem<PointLightsSystem>();
        
        // Set up system signature
        ecs::ComponentSignature signature;
        signature.set(coordinator->getComponentType<components::PointLightComponent>());
        signature.set(coordinator->getComponentType<components::TransformComponent>());
        signature.set(coordinator->getComponentType<components::SceneTag>());
        coordinator->setSystemSignature<PointLightsSystem>(signature);
    }
    
    ecs::Entity createPointLight(unsigned int sceneId,
                                const glm::vec3& position,
                                const glm::vec3& color,
                                float linear = 0.09f,
                                float quadratic = 0.032f) {
        auto entity = coordinator->createEntity();
        
        components::TransformComponent transform;
        transform.pos = position;
        coordinator->addComponent(entity, transform);
        
        components::PointLightComponent pointLight;
        pointLight.color = color;
        pointLight.linear = linear;
        pointLight.quadratic = quadratic;
        coordinator->addComponent(entity, pointLight);
        
        coordinator->addComponent(entity, components::SceneTag{sceneId});
        
        return entity;
    }
};

TEST_F(PointLightsSystemTest, NoPointLights) {
    setRenderContext(0);
    
    system->update();
    
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 0);
}

TEST_F(PointLightsSystemTest, SinglePointLight) {
    setRenderContext(0);
    
    auto light = createPointLight(0, glm::vec3(10.0f, 5.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    system->update();
    
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 1);
    EXPECT_EQ(renderContext.sceneLights.pointLights[0], light);
}

TEST_F(PointLightsSystemTest, MultiplePointLights) {
    setRenderContext(0);
    
    std::vector<ecs::Entity> lights;
    for (int i = 0; i < 5; ++i) {
        auto light = createPointLight(0, 
            glm::vec3(i * 10.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f));
        lights.push_back(light);
    }
    
    system->update();
    
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 5);
    
    // Verify all lights are in the list
    for (size_t i = 0; i < lights.size(); ++i) {
        bool found = false;
        for (unsigned int j = 0; j < renderContext.sceneLights.pointLightCount; ++j) {
            if (renderContext.sceneLights.pointLights[j] == lights[i]) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found) << "Light " << i << " not found in render context";
    }
}

TEST_F(PointLightsSystemTest, MaxPointLights) {
    setRenderContext(0);
    
    // Create more than max lights
    for (unsigned int i = 0; i < MAX_POINT_LIGHTS + 5; ++i) {
        createPointLight(0, glm::vec3(i * 10.0f, 0.0f, 0.0f), glm::vec3(1.0f));
    }
    
    system->update();
    
    // Should be capped at max
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, MAX_POINT_LIGHTS);
}

TEST_F(PointLightsSystemTest, SceneFiltering) {
    setRenderContext(1);
    
    createPointLight(0, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    createPointLight(1, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    createPointLight(2, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    system->update();
    
    // Only scene 1 light should be processed
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.pointLightCount, 1);
}

// SpotLightsSystem Tests
class SpotLightsSystemTest : public LightSystemTestBase {
protected:
    std::shared_ptr<SpotLightsSystem> system;
    
    void SetUp() override {
        LightSystemTestBase::SetUp();
        
        // Create and register system
        system = coordinator->registerSystem<SpotLightsSystem>();
        
        // Set up system signature
        ecs::ComponentSignature signature;
        signature.set(coordinator->getComponentType<components::SpotLightComponent>());
        signature.set(coordinator->getComponentType<components::TransformComponent>());
        signature.set(coordinator->getComponentType<components::SceneTag>());
        coordinator->setSystemSignature<SpotLightsSystem>(signature);
    }
    
    ecs::Entity createSpotLight(unsigned int sceneId,
                               const glm::vec3& position,
                               const glm::vec3& direction,
                               const glm::vec3& color,
                               float cutOff = glm::cos(glm::radians(12.5f)),
                               float outerCutoff = glm::cos(glm::radians(17.5f))) {
        auto entity = coordinator->createEntity();
        
        components::TransformComponent transform;
        transform.pos = position;
        coordinator->addComponent(entity, transform);
        
        components::SpotLightComponent spotLight;
        spotLight.direction = direction;
        spotLight.color = color;
        spotLight.cutOff = cutOff;
        spotLight.outerCutoff = outerCutoff;
        spotLight.linear = 0.09f;
        spotLight.quadratic = 0.032f;
        coordinator->addComponent(entity, spotLight);
        
        coordinator->addComponent(entity, components::SceneTag{sceneId});
        
        return entity;
    }
};

TEST_F(SpotLightsSystemTest, SingleSpotLight) {
    setRenderContext(0);
    
    auto light = createSpotLight(0, 
        glm::vec3(0.0f, 10.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f));
    
    system->update();
    
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.spotLightCount, 1);
    EXPECT_EQ(renderContext.sceneLights.spotLights[0], light);
}

TEST_F(SpotLightsSystemTest, MultipleSpotLights) {
    setRenderContext(0);
    
    std::vector<ecs::Entity> lights;
    for (int i = 0; i < 3; ++i) {
        auto light = createSpotLight(0,
            glm::vec3(i * 10.0f, 10.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(1.0f));
        lights.push_back(light);
    }
    
    system->update();
    
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.spotLightCount, 3);
}

TEST_F(SpotLightsSystemTest, MaxSpotLights) {
    setRenderContext(0);
    
    // Create more than max lights
    for (unsigned int i = 0; i < MAX_SPOT_LIGHTS + 5; ++i) {
        createSpotLight(0,
            glm::vec3(i * 10.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(1.0f));
    }
    
    system->update();
    
    // Should be capped at max
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.spotLightCount, MAX_SPOT_LIGHTS);
}

// Integration test for complete light setup
TEST_F(LightSystemTestBase, CompleteLightingSetup) {
    setRenderContext(0);
    
    // Create all light subsystems
    auto ambientSystem = coordinator->registerSystem<AmbientLightSystem>();
    auto directionalSystem = coordinator->registerSystem<DirectionalLightsSystem>();
    auto pointSystem = coordinator->registerSystem<PointLightsSystem>();
    auto spotSystem = coordinator->registerSystem<SpotLightsSystem>();
    
    // Set signatures
    ecs::ComponentSignature ambientSig;
    ambientSig.set(coordinator->getComponentType<components::AmbientLightComponent>());
    ambientSig.set(coordinator->getComponentType<components::SceneTag>());
    coordinator->setSystemSignature<AmbientLightSystem>(ambientSig);
    
    ecs::ComponentSignature dirSig;
    dirSig.set(coordinator->getComponentType<components::DirectionalLightComponent>());
    dirSig.set(coordinator->getComponentType<components::SceneTag>());
    coordinator->setSystemSignature<DirectionalLightsSystem>(dirSig);
    
    ecs::ComponentSignature pointSig;
    pointSig.set(coordinator->getComponentType<components::PointLightComponent>());
    pointSig.set(coordinator->getComponentType<components::TransformComponent>());
    pointSig.set(coordinator->getComponentType<components::SceneTag>());
    coordinator->setSystemSignature<PointLightsSystem>(pointSig);
    
    ecs::ComponentSignature spotSig;
    spotSig.set(coordinator->getComponentType<components::SpotLightComponent>());
    spotSig.set(coordinator->getComponentType<components::TransformComponent>());
    spotSig.set(coordinator->getComponentType<components::SceneTag>());
    coordinator->setSystemSignature<SpotLightsSystem>(spotSig);
    
    // Create lights
    auto ambient = coordinator->createEntity();
    components::AmbientLightComponent ambientComp;
    ambientComp.color = glm::vec3(0.1f, 0.1f, 0.1f);
    coordinator->addComponent(ambient, ambientComp);
    coordinator->addComponent(ambient, components::SceneTag{0});
    
    auto directional = coordinator->createEntity();
    components::DirectionalLightComponent dirComp(glm::vec3(0, -1, 0), glm::vec3(1));
    coordinator->addComponent(directional, dirComp);
    coordinator->addComponent(directional, components::SceneTag{0});
    
    // Create LightSystem
    LightSystem lightSystem(ambientSystem, directionalSystem, pointSystem, spotSystem);
    
    // Update all
    lightSystem.update();
    
    // Verify all lights are set
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.1f, 0.1f, 0.1f));
    EXPECT_EQ(renderContext.sceneLights.dirLight.direction, glm::vec3(0, -1, 0));
    EXPECT_EQ(renderContext.sceneLights.dirLight.color, glm::vec3(1));
}

} // namespace nexo::system