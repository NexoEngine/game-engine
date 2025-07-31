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
//  Description: Integration test suite for NexoEngine
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils/TestHelpers.hpp"
#include "ecs/Coordinator.hpp"
#include "components/Transform.hpp"
#include "components/Name.hpp"
#include "components/Uuid.hpp"
#include "components/Parent.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "components/Render.hpp"
#include "components/SceneComponents.hpp"
#include "components/RenderContext.hpp"
#include "systems/TransformMatrixSystem.hpp"
#include "systems/TransformHierarchySystem.hpp"
#include "systems/CameraSystem.hpp"
#include "systems/lights/AmbientLightSystem.hpp"
#include "Application.hpp"
#include "scene/SceneManager.hpp"
#include <chrono>
#include <memory>

namespace nexo::integration {

using namespace nexo::test;

// Mock Application for integration tests
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

class IntegrationTest : public ::testing::Test {
protected:
    std::unique_ptr<MockApplication> mockApp;
    std::unique_ptr<ecs::Coordinator> coordinator;
    
    void SetUp() override {
        mockApp = std::make_unique<MockApplication>();
        coordinator = std::make_unique<ecs::Coordinator>();
        coordinator->init();
        
        // Register all components
        registerComponents();
        
        // Register singleton components
        components::RenderContext renderContext;
        coordinator->setSingletonComponent(renderContext);
        
        // Create test scene
        mockApp->getSceneManager().createScene("IntegrationTestScene");
    }
    
    void TearDown() override {
        coordinator.reset();
        mockApp.reset();
    }
    
    void registerComponents() {
        coordinator->registerComponent<components::TransformComponent>();
        coordinator->registerComponent<components::NameComponent>();
        coordinator->registerComponent<components::UuidComponent>();
        coordinator->registerComponent<components::ParentComponent>();
        coordinator->registerComponent<components::RootComponent>();
        coordinator->registerComponent<components::CameraComponent>();
        coordinator->registerComponent<components::SceneTag>();
        coordinator->registerComponent<components::AmbientLightComponent>();
        coordinator->registerComponent<components::DirectionalLightComponent>();
        coordinator->registerComponent<components::PointLightComponent>();
        coordinator->registerComponent<components::RenderComponent>();
        
        coordinator->registerSingletonComponent<components::RenderContext>();
    }
};

TEST_F(IntegrationTest, CompleteEntityLifecycle) {
    // Create entity
    auto entity = coordinator->createEntity();
    EXPECT_NE(entity, ecs::NULL_ENTITY);
    
    // Add components
    coordinator->addComponent(entity, components::NameComponent{"TestEntity"});
    coordinator->addComponent(entity, components::UuidComponent{});
    coordinator->addComponent(entity, components::TransformComponent{});
    coordinator->addComponent(entity, components::RenderComponent{});
    coordinator->addComponent(entity, components::SceneTag{0});
    
    // Verify components
    EXPECT_TRUE(coordinator->hasComponent<components::NameComponent>(entity));
    EXPECT_TRUE(coordinator->hasComponent<components::UuidComponent>(entity));
    EXPECT_TRUE(coordinator->hasComponent<components::TransformComponent>(entity));
    EXPECT_TRUE(coordinator->hasComponent<components::RenderComponent>(entity));
    EXPECT_TRUE(coordinator->hasComponent<components::SceneTag>(entity));
    
    // Modify components
    auto& name = coordinator->getComponent<components::NameComponent>(entity);
    name.name = "ModifiedEntity";
    EXPECT_EQ(name.name, "ModifiedEntity");
    
    // Remove component
    coordinator->removeComponent<components::RenderComponent>(entity);
    EXPECT_FALSE(coordinator->hasComponent<components::RenderComponent>(entity));
    
    // Destroy entity
    coordinator->destroyEntity(entity);
    
    // Entity should no longer have components
    EXPECT_FALSE(coordinator->hasComponent<components::NameComponent>(entity));
}

TEST_F(IntegrationTest, TransformSystemIntegration) {
    // Create transform systems
    auto matrixSystem = coordinator->registerSystem<system::TransformMatrixSystem>();
    auto hierarchySystem = coordinator->registerSystem<system::TransformHierarchySystem>();
    
    // Set system signatures
    ecs::ComponentSignature matrixSig;
    matrixSig.set(coordinator->getComponentType<components::TransformComponent>());
    matrixSig.set(coordinator->getComponentType<components::SceneTag>());
    coordinator->setSystemSignature<system::TransformMatrixSystem>(matrixSig);
    
    ecs::ComponentSignature hierarchySig;
    hierarchySig.set(coordinator->getComponentType<components::TransformComponent>());
    hierarchySig.set(coordinator->getComponentType<components::SceneTag>());
    coordinator->setSystemSignature<system::TransformHierarchySystem>(hierarchySig);
    
    // Create parent-child hierarchy
    auto parent = coordinator->createEntity();
    components::TransformComponent parentTransform;
    parentTransform.pos = glm::vec3(10, 0, 0);
    parentTransform.quat = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
    coordinator->addComponent(parent, parentTransform);
    coordinator->addComponent(parent, components::SceneTag{0});
    
    auto child = coordinator->createEntity();
    components::TransformComponent childTransform;
    childTransform.pos = glm::vec3(5, 0, 0);
    coordinator->addComponent(child, childTransform);
    coordinator->addComponent(child, components::SceneTag{0});
    
    // Set up hierarchy
    auto& parentTrans = coordinator->getComponent<components::TransformComponent>(parent);
    parentTrans.children.push_back(child);
    
    // Set render context
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    renderContext.sceneRendered = 0;
    
    // Update systems
    matrixSystem->update();
    hierarchySystem->update();
    
    // Verify matrices were computed
    auto& updatedParent = coordinator->getComponent<components::TransformComponent>(parent);
    auto& updatedChild = coordinator->getComponent<components::TransformComponent>(child);
    
    EXPECT_NE(updatedParent.worldMatrix, glm::mat4(1.0f));
    EXPECT_NE(updatedChild.worldMatrix, glm::mat4(1.0f));
    EXPECT_NE(updatedChild.worldMatrix, updatedParent.worldMatrix);
}

TEST_F(IntegrationTest, CameraLightIntegration) {
    // Create camera system
    auto cameraSystem = coordinator->registerSystem<system::CameraContextSystem>();
    
    ecs::ComponentSignature cameraSig;
    cameraSig.set(coordinator->getComponentType<components::CameraComponent>());
    cameraSig.set(coordinator->getComponentType<components::TransformComponent>());
    cameraSig.set(coordinator->getComponentType<components::SceneTag>());
    coordinator->setSystemSignature<system::CameraContextSystem>(cameraSig);
    
    // Create ambient light system
    auto ambientSystem = coordinator->registerSystem<system::AmbientLightSystem>();
    
    ecs::ComponentSignature ambientSig;
    ambientSig.set(coordinator->getComponentType<components::AmbientLightComponent>());
    ambientSig.set(coordinator->getComponentType<components::SceneTag>());
    coordinator->setSystemSignature<system::AmbientLightSystem>(ambientSig);
    
    // Create camera
    auto camera = coordinator->createEntity();
    components::TransformComponent cameraTransform;
    cameraTransform.pos = glm::vec3(0, 0, 10);
    coordinator->addComponent(camera, cameraTransform);
    
    components::CameraComponent cameraComp;
    cameraComp.active = true;
    cameraComp.render = true;
    coordinator->addComponent(camera, cameraComp);
    coordinator->addComponent(camera, components::SceneTag{0});
    
    // Create ambient light
    auto light = coordinator->createEntity();
    components::AmbientLightComponent ambient;
    ambient.color = glm::vec3(0.2f, 0.2f, 0.2f);
    coordinator->addComponent(light, ambient);
    coordinator->addComponent(light, components::SceneTag{0});
    
    // Set render context
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    renderContext.sceneRendered = 0;
    
    // Update systems
    cameraSystem->update();
    ambientSystem->update();
    
    // Verify camera and light are in render context
    EXPECT_EQ(renderContext.cameras.size(), 1);
    EXPECT_EQ(renderContext.sceneLights.ambientLight, glm::vec3(0.2f, 0.2f, 0.2f));
}

TEST_F(IntegrationTest, SceneSwitching) {
    // Create entities in different scenes
    std::vector<ecs::Entity> scene0Entities;
    std::vector<ecs::Entity> scene1Entities;
    
    // Scene 0 entities
    for (int i = 0; i < 5; ++i) {
        auto entity = coordinator->createEntity();
        coordinator->addComponent(entity, components::NameComponent{"Scene0_Entity" + std::to_string(i)});
        coordinator->addComponent(entity, components::SceneTag{0});
        scene0Entities.push_back(entity);
    }
    
    // Scene 1 entities
    for (int i = 0; i < 3; ++i) {
        auto entity = coordinator->createEntity();
        coordinator->addComponent(entity, components::NameComponent{"Scene1_Entity" + std::to_string(i)});
        coordinator->addComponent(entity, components::SceneTag{1});
        scene1Entities.push_back(entity);
    }
    
    // Create scene-aware system
    auto matrixSystem = coordinator->registerSystem<system::TransformMatrixSystem>();
    ecs::ComponentSignature sig;
    sig.set(coordinator->getComponentType<components::TransformComponent>());
    sig.set(coordinator->getComponentType<components::SceneTag>());
    coordinator->setSystemSignature<system::TransformMatrixSystem>(sig);
    
    // Add transforms to some entities
    for (auto entity : scene0Entities) {
        coordinator->addComponent(entity, components::TransformComponent{});
    }
    for (auto entity : scene1Entities) {
        coordinator->addComponent(entity, components::TransformComponent{});
    }
    
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    
    // Process scene 0
    renderContext.sceneRendered = 0;
    matrixSystem->update();
    
    // Verify scene 0 entities were processed
    for (auto entity : scene0Entities) {
        auto& transform = coordinator->getComponent<components::TransformComponent>(entity);
        EXPECT_NE(transform.localMatrix, glm::mat4(1.0f));
    }
    
    // Scene 1 entities should not be processed
    for (auto entity : scene1Entities) {
        auto& transform = coordinator->getComponent<components::TransformComponent>(entity);
        EXPECT_EQ(transform.localMatrix, glm::mat4(1.0f));
    }
}

TEST_F(IntegrationTest, PerformanceBaseline) {
    // Create many entities with components
    const int entityCount = 10000;
    std::vector<ecs::Entity> entities;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < entityCount; ++i) {
        auto entity = coordinator->createEntity();
        coordinator->addComponent(entity, components::NameComponent{"Entity_" + std::to_string(i)});
        coordinator->addComponent(entity, components::UuidComponent{});
        coordinator->addComponent(entity, components::TransformComponent{});
        coordinator->addComponent(entity, components::SceneTag{0});
        entities.push_back(entity);
    }
    
    auto creationEnd = std::chrono::high_resolution_clock::now();
    auto creationTime = std::chrono::duration_cast<std::chrono::milliseconds>(creationEnd - start);
    
    // Create and update system
    auto matrixSystem = coordinator->registerSystem<system::TransformMatrixSystem>();
    ecs::ComponentSignature sig;
    sig.set(coordinator->getComponentType<components::TransformComponent>());
    sig.set(coordinator->getComponentType<components::SceneTag>());
    coordinator->setSystemSignature<system::TransformMatrixSystem>(sig);
    
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    renderContext.sceneRendered = 0;
    
    auto updateStart = std::chrono::high_resolution_clock::now();
    matrixSystem->update();
    auto updateEnd = std::chrono::high_resolution_clock::now();
    auto updateTime = std::chrono::duration_cast<std::chrono::milliseconds>(updateEnd - updateStart);
    
    // Performance assertions
    EXPECT_LT(creationTime.count(), 1000) << "Creating " << entityCount << " entities took " 
                                          << creationTime.count() << "ms";
    EXPECT_LT(updateTime.count(), 100) << "Updating " << entityCount << " entities took " 
                                       << updateTime.count() << "ms";
    
    // Cleanup
    auto cleanupStart = std::chrono::high_resolution_clock::now();
    for (auto entity : entities) {
        coordinator->destroyEntity(entity);
    }
    auto cleanupEnd = std::chrono::high_resolution_clock::now();
    auto cleanupTime = std::chrono::duration_cast<std::chrono::milliseconds>(cleanupEnd - cleanupStart);
    
    EXPECT_LT(cleanupTime.count(), 500) << "Destroying " << entityCount << " entities took " 
                                        << cleanupTime.count() << "ms";
}

TEST_F(IntegrationTest, ComponentMemoryManagement) {
    // Test RAII and memory management
    const int iterations = 100;
    
    for (int i = 0; i < iterations; ++i) {
        std::vector<ecs::Entity> tempEntities;
        
        // Create entities with various components
        for (int j = 0; j < 50; ++j) {
            auto entity = coordinator->createEntity();
            
            // Add different combinations of components
            if (j % 2 == 0) {
                coordinator->addComponent(entity, components::NameComponent{"Temp_" + std::to_string(j)});
            }
            if (j % 3 == 0) {
                coordinator->addComponent(entity, components::TransformComponent{});
            }
            if (j % 5 == 0) {
                coordinator->addComponent(entity, components::CameraComponent{});
            }
            
            coordinator->addComponent(entity, components::UuidComponent{});
            tempEntities.push_back(entity);
        }
        
        // Remove some components
        for (size_t j = 0; j < tempEntities.size() / 2; ++j) {
            if (coordinator->hasComponent<components::UuidComponent>(tempEntities[j])) {
                coordinator->removeComponent<components::UuidComponent>(tempEntities[j]);
            }
        }
        
        // Destroy all entities
        for (auto entity : tempEntities) {
            coordinator->destroyEntity(entity);
        }
    }
    
    // If we get here without crashes or memory issues, test passes
    SUCCEED();
}

TEST_F(IntegrationTest, ComplexHierarchyOperations) {
    // Create a complex hierarchy
    auto root = coordinator->createEntity();
    coordinator->addComponent(root, components::RootComponent{"HierarchyRoot"});
    coordinator->addComponent(root, components::TransformComponent{});
    coordinator->addComponent(root, components::SceneTag{0});
    
    std::vector<ecs::Entity> allEntities;
    allEntities.push_back(root);
    
    // Create multi-level hierarchy
    std::vector<ecs::Entity> currentLevel = {root};
    for (int level = 0; level < 4; ++level) {
        std::vector<ecs::Entity> nextLevel;
        
        for (auto parent : currentLevel) {
            auto& parentTransform = coordinator->getComponent<components::TransformComponent>(parent);
            
            for (int i = 0; i < 3; ++i) {
                auto child = coordinator->createEntity();
                coordinator->addComponent(child, components::TransformComponent{});
                coordinator->addComponent(child, components::ParentComponent{parent});
                coordinator->addComponent(child, components::SceneTag{0});
                
                parentTransform.children.push_back(child);
                nextLevel.push_back(child);
                allEntities.push_back(child);
            }
        }
        currentLevel = nextLevel;
    }
    
    // Create hierarchy system
    auto hierarchySystem = coordinator->registerSystem<system::TransformHierarchySystem>();
    ecs::ComponentSignature sig;
    sig.set(coordinator->getComponentType<components::TransformComponent>());
    sig.set(coordinator->getComponentType<components::SceneTag>());
    coordinator->setSystemSignature<system::TransformHierarchySystem>(sig);
    
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    renderContext.sceneRendered = 0;
    
    // Update hierarchy
    hierarchySystem->update();
    
    // Verify all entities have world matrices
    for (auto entity : allEntities) {
        auto& transform = coordinator->getComponent<components::TransformComponent>(entity);
        EXPECT_NE(transform.worldMatrix, glm::mat4(1.0f));
    }
    
    // Test hierarchy modification
    if (allEntities.size() > 10) {
        auto entityToMove = allEntities[10];
        auto newParent = allEntities[5];
        
        // Remove from old parent (would need to find it in real scenario)
        // Add to new parent
        auto& newParentTransform = coordinator->getComponent<components::TransformComponent>(newParent);
        newParentTransform.children.push_back(entityToMove);
        
        // Update hierarchy again
        hierarchySystem->update();
        
        // Entity should still have valid world matrix
        auto& movedTransform = coordinator->getComponent<components::TransformComponent>(entityToMove);
        EXPECT_NE(movedTransform.worldMatrix, glm::mat4(1.0f));
    }
}

TEST_F(IntegrationTest, MementoPatternIntegration) {
    // Test memento pattern across multiple component types
    auto entity = coordinator->createEntity();
    
    // Add components with specific values
    components::TransformComponent transform;
    transform.pos = glm::vec3(10, 20, 30);
    transform.quat = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
    transform.size = glm::vec3(2, 2, 2);
    coordinator->addComponent(entity, transform);
    
    components::AmbientLightComponent ambient;
    ambient.color = glm::vec3(0.5f, 0.6f, 0.7f);
    coordinator->addComponent(entity, ambient);
    
    components::SceneTag sceneTag{5, false, true};
    coordinator->addComponent(entity, sceneTag);
    
    // Save all states
    auto& savedTransform = coordinator->getComponent<components::TransformComponent>(entity);
    auto& savedAmbient = coordinator->getComponent<components::AmbientLightComponent>(entity);
    auto& savedScene = coordinator->getComponent<components::SceneTag>(entity);
    
    auto transformMemento = savedTransform.save();
    auto ambientMemento = savedAmbient.save();
    auto sceneMemento = savedScene.save();
    
    // Modify all components
    savedTransform.pos = glm::vec3(0);
    savedTransform.quat = glm::quat(1, 0, 0, 0);
    savedTransform.size = glm::vec3(1);
    
    savedAmbient.color = glm::vec3(0);
    
    savedScene.id = 0;
    savedScene.isActive = true;
    savedScene.isRendered = false;
    
    // Restore all states
    savedTransform.restore(transformMemento);
    savedAmbient.restore(ambientMemento);
    savedScene.restore(sceneMemento);
    
    // Verify restoration
    EXPECT_EQ(savedTransform.pos, glm::vec3(10, 20, 30));
    EXPECT_EQ(savedAmbient.color, glm::vec3(0.5f, 0.6f, 0.7f));
    EXPECT_EQ(savedScene.id, 5);
    EXPECT_FALSE(savedScene.isActive);
    EXPECT_TRUE(savedScene.isRendered);
}

} // namespace nexo::integration