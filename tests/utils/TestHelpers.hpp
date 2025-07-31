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
//  Description: Common test utilities and helpers for NexoEngine tests
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ecs/Coordinator.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/quaternion.hpp>
#include "components/Transform.hpp"
#include "components/Name.hpp"
#include "components/Uuid.hpp"
#include "components/Parent.hpp"
#include "components/SceneComponents.hpp"
#include "components/RenderContext.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "components/MaterialComponent.hpp"
#include <memory>
#include <vector>

namespace nexo::test {

// Base test fixture for ECS tests
class EcsTestFixture : public ::testing::Test {
public:
    std::unique_ptr<ecs::Coordinator> coordinator;
    
    void SetUp() override {
        coordinator = std::make_unique<ecs::Coordinator>();
        coordinator->init();
        
        // Register common components
        registerComponents();
    }
    
    void TearDown() override {
        coordinator.reset();
    }
    
protected:
    virtual void registerComponents() {
        // Register all common components used in tests
        coordinator->registerComponent<components::TransformComponent>();
        coordinator->registerComponent<components::NameComponent>();
        coordinator->registerComponent<components::UuidComponent>();
        coordinator->registerComponent<components::ParentComponent>();
        coordinator->registerComponent<components::SceneTag>();
        coordinator->registerComponent<components::CameraComponent>();
        // Note: Light components are separate types (AmbientLightComponent, etc.)
        coordinator->registerComponent<components::MaterialComponent>();
        
        // Register singleton components
        coordinator->registerSingletonComponent<components::RenderContext>();
    }
    
    // Helper to create entity with transform
    ecs::Entity createEntityWithTransform(
        const glm::vec3& pos = glm::vec3(0.0f),
        const glm::quat& rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        const glm::vec3& scale = glm::vec3(1.0f))
    {
        auto entity = coordinator->createEntity();
        components::TransformComponent transform;
        transform.pos = pos;
        transform.quat = rot;
        transform.size = scale;
        coordinator->addComponent(entity, transform);
        return entity;
    }
    
    // Helper to create named entity
    ecs::Entity createNamedEntity(const std::string& name) {
        auto entity = coordinator->createEntity();
        coordinator->addComponent(entity, components::NameComponent{name});
        coordinator->addComponent(entity, components::UuidComponent{});
        return entity;
    }
    
    // Helper to create entity with scene tag
    ecs::Entity createEntityInScene(unsigned int sceneId) {
        auto entity = coordinator->createEntity();
        coordinator->addComponent(entity, components::SceneTag{sceneId});
        return entity;
    }
    
    // Helper to create parent-child relationship
    void createParentChildRelationship(ecs::Entity parent, ecs::Entity child) {
        coordinator->addComponent(child, components::ParentComponent{parent});
        
        // Update child count if parent has RootComponent
        if (coordinator->entityHasComponent<components::RootComponent>(parent)) {
            auto& root = coordinator->getComponent<components::RootComponent>(parent);
            root.childCount++;
        }
    }
    
    // Helper to create entity hierarchy
    std::vector<ecs::Entity> createEntityHierarchy(int depth, int childrenPerNode = 2) {
        std::vector<ecs::Entity> entities;
        
        // Create root
        auto root = createEntityWithTransform();
        coordinator->addComponent(root, components::RootComponent{"HierarchyRoot", {}, 0});
        entities.push_back(root);
        
        // Create hierarchy recursively
        createHierarchyLevel(root, depth - 1, childrenPerNode, entities);
        
        return entities;
    }
    
private:
    void createHierarchyLevel(ecs::Entity parent, int remainingDepth, 
                             int childrenPerNode, std::vector<ecs::Entity>& entities) {
        if (remainingDepth <= 0) return;
        
        for (int i = 0; i < childrenPerNode; ++i) {
            auto child = createEntityWithTransform();
            createParentChildRelationship(parent, child);
            entities.push_back(child);
            
            // Recurse
            createHierarchyLevel(child, remainingDepth - 1, childrenPerNode, entities);
        }
    }
};

// Test fixture for system tests
template<typename SystemType>
class SystemTestFixture : public EcsTestFixture {
protected:
    std::shared_ptr<SystemType> system;
    
    void SetUp() override {
        EcsTestFixture::SetUp();
        
        // Create and register system
        // Note: System registration API may have changed - this needs to be updated
        
        // Set up system requirements
        setupSystemRequirements();
    }
    
    virtual void setupSystemRequirements() {
        // Override in derived classes to set system component signatures
    }
    
    // Helper to set render context for scene-aware systems
    void setRenderContext(int sceneId) {
        components::RenderContext context;
        context.sceneRendered = sceneId;
        // Note: Singleton component API may have changed - needs to be updated
        // coordinator->addSingletonComponent(context);
    }
};

// Mock coordinator for isolated component tests
class MockCoordinator {
public:
    template<typename T>
    void registerComponent() {
        // Mock registration
    }
    
    template<typename T>
    void registerSingletonComponent() {
        // Mock singleton registration
    }
    
    ecs::Entity createEntity() {
        return nextEntity++;
    }
    
    template<typename T>
    void addComponent(ecs::Entity entity, const T& component) {
        // Mock component addition
    }
    
    template<typename T>
    T& getComponent(ecs::Entity entity) {
        static T component;
        return component;
    }
    
    template<typename T>
    bool hasComponent(ecs::Entity entity) const {
        return true; // Mock has component
    }
    
private:
    ecs::Entity nextEntity = 0;
};

// Matcher for comparing transforms
MATCHER_P(TransformNear, expected, "Transform is near expected") {
    const float epsilon = 0.0001f;
    return glm::all(glm::epsilonEqual(arg.pos, expected.pos, epsilon)) &&
           glm::all(glm::epsilonEqual(arg.quat, expected.quat, epsilon)) &&
           glm::all(glm::epsilonEqual(arg.size, expected.size, epsilon));
}

// Matcher for comparing matrices
MATCHER_P(MatrixNear, expected, "Matrix is near expected") {
    const float epsilon = 0.0001f;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (std::abs(arg[i][j] - expected[i][j]) > epsilon) {
                return false;
            }
        }
    }
    return true;
}

// Helper to create test scenes
class TestSceneBuilder {
public:
    TestSceneBuilder(ecs::Coordinator* coord) : coordinator(coord) {}
    
    TestSceneBuilder& withEntities(int count) {
        for (int i = 0; i < count; ++i) {
            entities.push_back(coordinator->createEntity());
        }
        return *this;
    }
    
    TestSceneBuilder& withTransforms() {
        for (auto entity : entities) {
            coordinator->addComponent(entity, components::TransformComponent{});
        }
        return *this;
    }
    
    TestSceneBuilder& inScene(unsigned int sceneId) {
        for (auto entity : entities) {
            coordinator->addComponent(entity, components::SceneTag{sceneId});
        }
        return *this;
    }
    
    TestSceneBuilder& withNames(const std::string& prefix = "Entity") {
        int index = 0;
        for (auto entity : entities) {
            coordinator->addComponent(entity, 
                components::NameComponent{prefix + std::to_string(index++)});
        }
        return *this;
    }
    
    std::vector<ecs::Entity> build() {
        return entities;
    }
    
private:
    ecs::Coordinator* coordinator;
    std::vector<ecs::Entity> entities;
};

// Performance testing helpers
class PerformanceTimer {
public:
    void start() {
        startTime = std::chrono::high_resolution_clock::now();
    }
    
    double elapsedMilliseconds() {
        auto endTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(endTime - startTime).count();
    }
    
    double elapsedMicroseconds() {
        auto endTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::micro>(endTime - startTime).count();
    }
    
private:
    std::chrono::high_resolution_clock::time_point startTime;
};

// Asset reference mock for MaterialComponent tests
template<typename T>
class MockAssetRef {
public:
    MockAssetRef() = default;
    MockAssetRef(const T* asset) : mockAsset(asset) {}
    
    bool isValid() const { return mockAsset != nullptr; }
    const T* get() const { return mockAsset; }
    T* get() { return const_cast<T*>(mockAsset); }
    
    void reset() { mockAsset = nullptr; }
    
private:
    const T* mockAsset = nullptr;
};

// Mock material for testing
struct MockMaterial {
    std::string name = "TestMaterial";
    glm::vec4 color = glm::vec4(1.0f);
    float shininess = 32.0f;
};

} // namespace nexo::test