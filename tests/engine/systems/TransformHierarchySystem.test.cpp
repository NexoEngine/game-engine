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
//  Description: Test file for the TransformHierarchySystem
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "systems/TransformHierarchySystem.hpp"
#include "utils/TestHelpers.hpp"
#include "components/Transform.hpp"
#include "components/Parent.hpp"
#include "components/SceneComponents.hpp"
#include "components/RenderContext.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace nexo::system {

using namespace nexo::test;

class TransformHierarchySystemTest : public SystemTestFixture<TransformHierarchySystem> {
protected:
    void SetUp() override {
        SystemTestFixture::SetUp();
        
        // Register RootComponent which is used by the system
        coordinator->registerComponent<components::RootComponent>();
    }
    
    void setupSystemRequirements() override {
        // Set up system signature - system uses a group
        ecs::ComponentSignature signature;
        signature.set(coordinator->getComponentType<components::TransformComponent>());
        signature.set(coordinator->getComponentType<components::SceneTag>());
        coordinator->setSystemSignature<TransformHierarchySystem>(signature);
    }
    
    // Helper to create a simple parent-child hierarchy
    std::pair<ecs::Entity, ecs::Entity> createSimpleHierarchy(unsigned int sceneId) {
        // Create parent
        auto parent = coordinator->createEntity();
        components::TransformComponent parentTransform;
        parentTransform.pos = glm::vec3(10.0f, 0.0f, 0.0f);
        parentTransform.quat = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
        parentTransform.size = glm::vec3(2.0f);
        coordinator->addComponent(parent, parentTransform);
        coordinator->addComponent(parent, components::SceneTag{sceneId});
        
        // Create child
        auto child = coordinator->createEntity();
        components::TransformComponent childTransform;
        childTransform.pos = glm::vec3(5.0f, 0.0f, 0.0f);
        childTransform.quat = glm::quat(1, 0, 0, 0);
        childTransform.size = glm::vec3(1.0f);
        coordinator->addComponent(child, childTransform);
        coordinator->addComponent(child, components::SceneTag{sceneId});
        
        // Update parent's children list
        auto& parentTrans = coordinator->getComponent<components::TransformComponent>(parent);
        parentTrans.children.push_back(child);
        
        return {parent, child};
    }
    
    // Helper to create multi-level hierarchy
    std::vector<ecs::Entity> createMultiLevelHierarchy(unsigned int sceneId, int levels) {
        std::vector<ecs::Entity> entities;
        
        // Create root
        auto root = coordinator->createEntity();
        components::TransformComponent rootTransform;
        rootTransform.pos = glm::vec3(0.0f, 10.0f, 0.0f);
        coordinator->addComponent(root, rootTransform);
        coordinator->addComponent(root, components::SceneTag{sceneId});
        entities.push_back(root);
        
        // Create hierarchy
        ecs::Entity parent = root;
        for (int i = 1; i < levels; ++i) {
            auto child = coordinator->createEntity();
            components::TransformComponent childTransform;
            childTransform.pos = glm::vec3(1.0f * i, 0.0f, 0.0f);
            childTransform.quat = glm::angleAxis(glm::radians(10.0f * i), glm::vec3(0, 0, 1));
            coordinator->addComponent(child, childTransform);
            coordinator->addComponent(child, components::SceneTag{sceneId});
            
            // Add to parent's children
            auto& parentTransform = coordinator->getComponent<components::TransformComponent>(parent);
            parentTransform.children.push_back(child);
            
            entities.push_back(child);
            parent = child;
        }
        
        return entities;
    }
};

TEST_F(TransformHierarchySystemTest, NoSceneRendered) {
    // Set render context with no scene
    setRenderContext(-1);
    
    // Create hierarchy
    auto [parent, child] = createSimpleHierarchy(0);
    
    // Run system - should not process any entities
    system->update();
    
    // World matrices should remain identity
    auto& parentTransform = coordinator->getComponent<components::TransformComponent>(parent);
    auto& childTransform = coordinator->getComponent<components::TransformComponent>(child);
    
    EXPECT_EQ(parentTransform.worldMatrix, glm::mat4(1.0f));
    EXPECT_EQ(childTransform.worldMatrix, glm::mat4(1.0f));
}

TEST_F(TransformHierarchySystemTest, SimpleParentChildHierarchy) {
    setRenderContext(0);
    
    auto [parent, child] = createSimpleHierarchy(0);
    
    // Get initial transforms
    auto& parentTransform = coordinator->getComponent<components::TransformComponent>(parent);
    auto& childTransform = coordinator->getComponent<components::TransformComponent>(child);
    
    // Run system
    system->update();
    
    // Parent world matrix should equal its local matrix (no parent)
    glm::mat4 expectedParentWorld = glm::translate(glm::mat4(1.0f), parentTransform.pos) *
                                    glm::toMat4(parentTransform.quat) *
                                    glm::scale(glm::mat4(1.0f), parentTransform.size);
    
    EXPECT_TRUE(MatrixNear(parentTransform.worldMatrix, expectedParentWorld));
    
    // Child world matrix should be parent world * child local
    glm::mat4 childLocal = glm::translate(glm::mat4(1.0f), childTransform.pos) *
                          glm::toMat4(childTransform.quat) *
                          glm::scale(glm::mat4(1.0f), childTransform.size);
    glm::mat4 expectedChildWorld = parentTransform.worldMatrix * childLocal;
    
    EXPECT_TRUE(MatrixNear(childTransform.worldMatrix, expectedChildWorld));
}

TEST_F(TransformHierarchySystemTest, MultiLevelHierarchy) {
    setRenderContext(0);
    
    const int levels = 5;
    auto entities = createMultiLevelHierarchy(0, levels);
    
    // Run system
    system->update();
    
    // Verify each level's world matrix
    glm::mat4 expectedWorld = glm::mat4(1.0f);
    for (size_t i = 0; i < entities.size(); ++i) {
        auto& transform = coordinator->getComponent<components::TransformComponent>(entities[i]);
        
        // Calculate expected local matrix
        glm::mat4 localMatrix = glm::translate(glm::mat4(1.0f), transform.pos) *
                               glm::toMat4(transform.quat) *
                               glm::scale(glm::mat4(1.0f), transform.size);
        
        // Expected world = accumulated transforms
        expectedWorld = expectedWorld * localMatrix;
        
        EXPECT_TRUE(MatrixNear(transform.worldMatrix, expectedWorld))
            << "Level " << i << " world matrix mismatch";
    }
}

TEST_F(TransformHierarchySystemTest, MultipleRootsInScene) {
    setRenderContext(0);
    
    // Create multiple root entities with children
    std::vector<std::pair<ecs::Entity, ecs::Entity>> hierarchies;
    for (int i = 0; i < 3; ++i) {
        auto [parent, child] = createSimpleHierarchy(0);
        
        // Give each root a different transform
        auto& parentTransform = coordinator->getComponent<components::TransformComponent>(parent);
        parentTransform.pos = glm::vec3(i * 10.0f, 0.0f, 0.0f);
        parentTransform.quat = glm::angleAxis(glm::radians(30.0f * i), glm::vec3(0, 1, 0));
        
        hierarchies.push_back({parent, child});
    }
    
    // Run system
    system->update();
    
    // Verify each hierarchy is processed independently
    for (const auto& [parent, child] : hierarchies) {
        auto& parentTransform = coordinator->getComponent<components::TransformComponent>(parent);
        auto& childTransform = coordinator->getComponent<components::TransformComponent>(child);
        
        // Parent should have world matrix = local matrix
        EXPECT_NE(parentTransform.worldMatrix, glm::mat4(1.0f));
        
        // Child should have parent's transform applied
        EXPECT_NE(childTransform.worldMatrix, glm::mat4(1.0f));
        EXPECT_NE(childTransform.worldMatrix, parentTransform.worldMatrix);
    }
}

TEST_F(TransformHierarchySystemTest, SceneFiltering) {
    setRenderContext(1);
    
    // Create entities in different scenes
    auto [parent0, child0] = createSimpleHierarchy(0);
    auto [parent1, child1] = createSimpleHierarchy(1);
    auto [parent2, child2] = createSimpleHierarchy(2);
    
    // Run system
    system->update();
    
    // Only scene 1 entities should be processed
    auto& parent0Transform = coordinator->getComponent<components::TransformComponent>(parent0);
    auto& child0Transform = coordinator->getComponent<components::TransformComponent>(child0);
    EXPECT_EQ(parent0Transform.worldMatrix, glm::mat4(1.0f));
    EXPECT_EQ(child0Transform.worldMatrix, glm::mat4(1.0f));
    
    auto& parent1Transform = coordinator->getComponent<components::TransformComponent>(parent1);
    auto& child1Transform = coordinator->getComponent<components::TransformComponent>(child1);
    EXPECT_NE(parent1Transform.worldMatrix, glm::mat4(1.0f));
    EXPECT_NE(child1Transform.worldMatrix, glm::mat4(1.0f));
    
    auto& parent2Transform = coordinator->getComponent<components::TransformComponent>(parent2);
    auto& child2Transform = coordinator->getComponent<components::TransformComponent>(child2);
    EXPECT_EQ(parent2Transform.worldMatrix, glm::mat4(1.0f));
    EXPECT_EQ(child2Transform.worldMatrix, glm::mat4(1.0f));
}

TEST_F(TransformHierarchySystemTest, ComplexTransformCombination) {
    setRenderContext(0);
    
    auto [parent, child] = createSimpleHierarchy(0);
    
    // Set complex transforms
    auto& parentTransform = coordinator->getComponent<components::TransformComponent>(parent);
    parentTransform.pos = glm::vec3(10.0f, 20.0f, 30.0f);
    parentTransform.quat = glm::angleAxis(glm::radians(45.0f), glm::normalize(glm::vec3(1, 1, 1)));
    parentTransform.size = glm::vec3(2.0f, 3.0f, 0.5f);
    
    auto& childTransform = coordinator->getComponent<components::TransformComponent>(child);
    childTransform.pos = glm::vec3(-5.0f, 10.0f, 0.0f);
    childTransform.quat = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1));
    childTransform.size = glm::vec3(0.5f, 0.5f, 2.0f);
    
    // Run system
    system->update();
    
    // Verify transforms are properly combined
    glm::vec3 scale, translation, skew;
    glm::quat rotation;
    glm::vec4 perspective;
    
    // Decompose child world matrix
    glm::decompose(childTransform.worldMatrix, scale, rotation, translation, skew, perspective);
    
    // Child's world position should be transformed by parent
    glm::vec4 childWorldPos = parentTransform.worldMatrix * glm::vec4(childTransform.pos, 1.0f);
    EXPECT_TRUE(glm::all(glm::epsilonEqual(translation, glm::vec3(childWorldPos), 0.001f)));
    
    // Scale should be combined (parent scale * child scale)
    glm::vec3 expectedScale = parentTransform.size * childTransform.size;
    EXPECT_TRUE(glm::all(glm::epsilonEqual(scale, expectedScale, 0.001f)));
}

TEST_F(TransformHierarchySystemTest, MissingChildTransform) {
    setRenderContext(0);
    
    // Create parent with transform
    auto parent = coordinator->createEntity();
    components::TransformComponent parentTransform;
    parentTransform.pos = glm::vec3(10.0f, 0.0f, 0.0f);
    coordinator->addComponent(parent, parentTransform);
    coordinator->addComponent(parent, components::SceneTag{0});
    
    // Create child without transform component
    auto child = coordinator->createEntity();
    coordinator->addComponent(child, components::SceneTag{0});
    
    // Add invalid child to parent's children list
    auto& parentTrans = coordinator->getComponent<components::TransformComponent>(parent);
    parentTrans.children.push_back(child);
    
    // Run system - should handle missing component gracefully
    EXPECT_NO_THROW(system->update());
    
    // Parent should still be processed
    EXPECT_NE(parentTrans.worldMatrix, glm::mat4(1.0f));
}

TEST_F(TransformHierarchySystemTest, BranchingHierarchy) {
    setRenderContext(0);
    
    // Create parent with multiple children
    auto parent = coordinator->createEntity();
    components::TransformComponent parentTransform;
    parentTransform.pos = glm::vec3(0.0f, 10.0f, 0.0f);
    coordinator->addComponent(parent, parentTransform);
    coordinator->addComponent(parent, components::SceneTag{0});
    
    // Create multiple children
    std::vector<ecs::Entity> children;
    for (int i = 0; i < 4; ++i) {
        auto child = coordinator->createEntity();
        components::TransformComponent childTransform;
        childTransform.pos = glm::vec3(i * 5.0f, 0.0f, 0.0f);
        childTransform.quat = glm::angleAxis(glm::radians(90.0f * i), glm::vec3(0, 1, 0));
        coordinator->addComponent(child, childTransform);
        coordinator->addComponent(child, components::SceneTag{0});
        
        children.push_back(child);
        
        // Add to parent's children
        auto& parentTrans = coordinator->getComponent<components::TransformComponent>(parent);
        parentTrans.children.push_back(child);
    }
    
    // Run system
    system->update();
    
    // Verify parent transform
    auto& parentTrans = coordinator->getComponent<components::TransformComponent>(parent);
    EXPECT_NE(parentTrans.worldMatrix, glm::mat4(1.0f));
    
    // Verify all children have different world transforms
    std::set<glm::mat4> uniqueTransforms;
    for (auto child : children) {
        auto& childTransform = coordinator->getComponent<components::TransformComponent>(child);
        EXPECT_NE(childTransform.worldMatrix, glm::mat4(1.0f));
        uniqueTransforms.insert(childTransform.worldMatrix);
    }
    
    // All children should have unique world transforms
    EXPECT_EQ(uniqueTransforms.size(), children.size());
}

TEST_F(TransformHierarchySystemTest, ZeroScaleHandling) {
    setRenderContext(0);
    
    auto [parent, child] = createSimpleHierarchy(0);
    
    // Set parent to have zero scale on one axis
    auto& parentTransform = coordinator->getComponent<components::TransformComponent>(parent);
    parentTransform.size = glm::vec3(0.0f, 1.0f, 1.0f);
    
    // Run system
    system->update();
    
    // System should still run without crashes
    auto& childTransform = coordinator->getComponent<components::TransformComponent>(child);
    
    // Check matrices are valid (no NaN or inf)
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_FALSE(std::isnan(parentTransform.worldMatrix[i][j]));
            EXPECT_FALSE(std::isinf(parentTransform.worldMatrix[i][j]));
            EXPECT_FALSE(std::isnan(childTransform.worldMatrix[i][j]));
            EXPECT_FALSE(std::isinf(childTransform.worldMatrix[i][j]));
        }
    }
}

TEST_F(TransformHierarchySystemTest, DeepHierarchyPerformance) {
    setRenderContext(0);
    
    const int depth = 100;
    auto entities = createMultiLevelHierarchy(0, depth);
    
    // Measure update time
    PerformanceTimer timer;
    timer.start();
    
    system->update();
    
    double elapsed = timer.elapsedMilliseconds();
    
    // Should handle deep hierarchy efficiently
    EXPECT_LT(elapsed, 10.0) << "Processing " << depth 
                             << " level hierarchy took " << elapsed << "ms";
    
    // Verify deepest entity has accumulated transform
    auto& deepestTransform = coordinator->getComponent<components::TransformComponent>(entities.back());
    EXPECT_NE(deepestTransform.worldMatrix, glm::mat4(1.0f));
}

TEST_F(TransformHierarchySystemTest, MultipleSceneSwitch) {
    // Create hierarchies in multiple scenes
    auto [parent0, child0] = createSimpleHierarchy(0);
    auto [parent1, child1] = createSimpleHierarchy(1);
    
    // Process scene 0
    setRenderContext(0);
    system->update();
    
    auto& parent0Transform = coordinator->getComponent<components::TransformComponent>(parent0);
    auto& child0Transform = coordinator->getComponent<components::TransformComponent>(child0);
    EXPECT_NE(parent0Transform.worldMatrix, glm::mat4(1.0f));
    EXPECT_NE(child0Transform.worldMatrix, glm::mat4(1.0f));
    
    // Store scene 0 matrices
    glm::mat4 parent0World = parent0Transform.worldMatrix;
    glm::mat4 child0World = child0Transform.worldMatrix;
    
    // Process scene 1
    setRenderContext(1);
    system->update();
    
    auto& parent1Transform = coordinator->getComponent<components::TransformComponent>(parent1);
    auto& child1Transform = coordinator->getComponent<components::TransformComponent>(child1);
    EXPECT_NE(parent1Transform.worldMatrix, glm::mat4(1.0f));
    EXPECT_NE(child1Transform.worldMatrix, glm::mat4(1.0f));
    
    // Scene 0 matrices should remain unchanged
    EXPECT_EQ(parent0Transform.worldMatrix, parent0World);
    EXPECT_EQ(child0Transform.worldMatrix, child0World);
}

TEST_F(TransformHierarchySystemTest, EmptyChildrenList) {
    setRenderContext(0);
    
    // Create entity with empty children list
    auto entity = coordinator->createEntity();
    components::TransformComponent transform;
    transform.pos = glm::vec3(5.0f, 5.0f, 5.0f);
    transform.children.clear(); // Explicitly empty
    coordinator->addComponent(entity, transform);
    coordinator->addComponent(entity, components::SceneTag{0});
    
    // Run system
    system->update();
    
    // Entity should still be processed
    auto& updatedTransform = coordinator->getComponent<components::TransformComponent>(entity);
    EXPECT_NE(updatedTransform.worldMatrix, glm::mat4(1.0f));
}

TEST_F(TransformHierarchySystemTest, IdentityTransformHierarchy) {
    setRenderContext(0);
    
    // Create hierarchy with identity transforms
    auto parent = coordinator->createEntity();
    components::TransformComponent parentTransform; // Default identity
    coordinator->addComponent(parent, parentTransform);
    coordinator->addComponent(parent, components::SceneTag{0});
    
    auto child = coordinator->createEntity();
    components::TransformComponent childTransform; // Default identity
    coordinator->addComponent(child, childTransform);
    coordinator->addComponent(child, components::SceneTag{0});
    
    auto& parentTrans = coordinator->getComponent<components::TransformComponent>(parent);
    parentTrans.children.push_back(child);
    
    // Run system
    system->update();
    
    // Both should have identity world matrices
    auto& childTrans = coordinator->getComponent<components::TransformComponent>(child);
    EXPECT_EQ(parentTrans.worldMatrix, glm::mat4(1.0f));
    EXPECT_EQ(childTrans.worldMatrix, glm::mat4(1.0f));
}

} // namespace nexo::system