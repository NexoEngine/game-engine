//// TransformHierarchySystem.test.cpp ///////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/13/2025
//  Description: Test file for TransformHierarchySystem
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "systems/TransformHierarchySystem.hpp"
#include "components/Transform.hpp"
#include "components/Parent.hpp"
#include "components/SceneComponents.hpp"
#include "components/RenderContext.hpp"
#include "ecs/Coordinator.hpp"

namespace nexo::system {

class TransformHierarchySystemTest : public ::testing::Test {
protected:
    std::shared_ptr<ecs::Coordinator> coordinator;
    std::shared_ptr<TransformHierarchySystem> system;
    std::vector<ecs::Entity> entities;

    void SetUp() override {
        coordinator = std::make_shared<ecs::Coordinator>();
        coordinator->init();
        ecs::System::coord = coordinator;

        // Register components
        coordinator->registerComponent<components::TransformComponent>();
        coordinator->registerComponent<components::SceneTag>();
        coordinator->registerComponent<components::RootComponent>();

        // Register singleton component
        coordinator->registerSingletonComponent<components::RenderContext>();

        // Register system
        system = coordinator->registerGroupSystem<TransformHierarchySystem>();
    }

    void TearDown() override {
        for (auto entity : entities) {
            coordinator->destroyEntity(entity);
        }
        entities.clear();
        ecs::System::coord = nullptr;
    }

    // Helper to compare mat4 with epsilon
    static bool compareMat4(const glm::mat4& a, const glm::mat4& b, float epsilon = 0.0001f) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (std::abs(a[i][j] - b[i][j]) > epsilon)
                    return false;
            }
        }
        return true;
    }

    // Helper to create a root entity in a scene
    ecs::Entity createRootEntity(unsigned int sceneId, const glm::vec3& pos = glm::vec3(0.0f),
                                  const glm::quat& rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                                  const glm::vec3& scale = glm::vec3(1.0f)) {
        ecs::Entity entity = coordinator->createEntity();
        entities.push_back(entity);

        components::TransformComponent transform;
        transform.pos = pos;
        transform.quat = rot;
        transform.size = scale;
        transform.worldMatrix = glm::mat4(0.0f);  // Initialize to zero to detect changes
        coordinator->addComponent(entity, transform);

        components::SceneTag sceneTag;
        sceneTag.id = sceneId;
        coordinator->addComponent(entity, sceneTag);

        components::RootComponent root;
        coordinator->addComponent(entity, root);

        return entity;
    }

    // Helper to add a child to a parent entity
    void addChildToParent(ecs::Entity parent, ecs::Entity child) {
        auto& parentTransform = coordinator->getComponent<components::TransformComponent>(parent);
        parentTransform.addChild(child);
    }

    // Helper to set scene for rendering
    void setRenderedScene(unsigned int sceneId) {
        auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
        renderContext.sceneRendered = static_cast<int>(sceneId);
    }
};

// =============================================================================
// Basic Hierarchy Tests
// =============================================================================

TEST_F(TransformHierarchySystemTest, SingleEntityNoParent) {
    // Create a single root entity with no children
    auto root = createRootEntity(0, glm::vec3(10.0f, 20.0f, 30.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 3.0f, 4.0f));
    setRenderedScene(0);

    system->update();

    auto& transform = coordinator->getComponent<components::TransformComponent>(root);

    // Expected: world matrix = local matrix (T * R * S)
    glm::mat4 expectedLocal = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 20.0f, 30.0f)) *
                              glm::toMat4(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) *
                              glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 4.0f));

    EXPECT_TRUE(compareMat4(transform.worldMatrix, expectedLocal));
}

TEST_F(TransformHierarchySystemTest, ParentChildPair) {
    // Create parent
    auto parent = createRootEntity(0, glm::vec3(10.0f, 0.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));

    // Create child
    auto child = coordinator->createEntity();
    entities.push_back(child);

    components::TransformComponent childTransform;
    childTransform.pos = glm::vec3(5.0f, 0.0f, 0.0f);
    childTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    childTransform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(child, childTransform);

    components::SceneTag sceneTag;
    sceneTag.id = 0;
    coordinator->addComponent(child, sceneTag);

    // Link parent-child
    addChildToParent(parent, child);

    setRenderedScene(0);
    system->update();

    auto& parentTransformAfter = coordinator->getComponent<components::TransformComponent>(parent);
    auto& childTransformAfter = coordinator->getComponent<components::TransformComponent>(child);

    // Parent local matrix: T(10,0,0) * S(2,2,2)
    glm::mat4 parentLocal = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f)) *
                            glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));

    // Child local matrix: T(5,0,0)
    glm::mat4 childLocal = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f));

    // Child world matrix = parent world * child local
    glm::mat4 expectedChildWorld = parentLocal * childLocal;

    EXPECT_TRUE(compareMat4(parentTransformAfter.worldMatrix, parentLocal));
    EXPECT_TRUE(compareMat4(childTransformAfter.worldMatrix, expectedChildWorld));
}

TEST_F(TransformHierarchySystemTest, ThreeLevelHierarchy) {
    // Create grandparent
    auto grandparent = createRootEntity(0, glm::vec3(10.0f, 0.0f, 0.0f));

    // Create parent
    auto parent = coordinator->createEntity();
    entities.push_back(parent);
    components::TransformComponent parentTransform;
    parentTransform.pos = glm::vec3(5.0f, 0.0f, 0.0f);
    parentTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    parentTransform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(parent, parentTransform);
    components::SceneTag sceneTag1;
    sceneTag1.id = 0;
    coordinator->addComponent(parent, sceneTag1);

    // Create child
    auto child = coordinator->createEntity();
    entities.push_back(child);
    components::TransformComponent childTransform;
    childTransform.pos = glm::vec3(3.0f, 0.0f, 0.0f);
    childTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    childTransform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(child, childTransform);
    components::SceneTag sceneTag2;
    sceneTag2.id = 0;
    coordinator->addComponent(child, sceneTag2);

    // Link hierarchy: grandparent -> parent -> child
    addChildToParent(grandparent, parent);
    addChildToParent(parent, child);

    setRenderedScene(0);
    system->update();

    auto& grandparentTransformAfter = coordinator->getComponent<components::TransformComponent>(grandparent);
    auto& parentTransformAfter = coordinator->getComponent<components::TransformComponent>(parent);
    auto& childTransformAfter = coordinator->getComponent<components::TransformComponent>(child);

    // Calculate expected matrices
    glm::mat4 grandparentWorld = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f));
    glm::mat4 parentLocal = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f));
    glm::mat4 parentWorld = grandparentWorld * parentLocal;
    glm::mat4 childLocal = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
    glm::mat4 childWorld = parentWorld * childLocal;

    EXPECT_TRUE(compareMat4(grandparentTransformAfter.worldMatrix, grandparentWorld));
    EXPECT_TRUE(compareMat4(parentTransformAfter.worldMatrix, parentWorld));
    EXPECT_TRUE(compareMat4(childTransformAfter.worldMatrix, childWorld));

    // Verify that the child is at world position (18, 0, 0)
    glm::vec3 childWorldPos = glm::vec3(childWorld[3]);
    EXPECT_NEAR(childWorldPos.x, 18.0f, 0.0001f);
}

// =============================================================================
// Matrix Calculations Tests
// =============================================================================

TEST_F(TransformHierarchySystemTest, TranslationInheritance) {
    auto parent = createRootEntity(0, glm::vec3(10.0f, 20.0f, 30.0f));

    auto child = coordinator->createEntity();
    entities.push_back(child);
    components::TransformComponent childTransform;
    childTransform.pos = glm::vec3(5.0f, 10.0f, 15.0f);
    childTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    childTransform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(child, childTransform);
    components::SceneTag sceneTag;
    sceneTag.id = 0;
    coordinator->addComponent(child, sceneTag);

    addChildToParent(parent, child);
    setRenderedScene(0);
    system->update();

    auto& childTransformAfter = coordinator->getComponent<components::TransformComponent>(child);

    // Child world position should be parent + child local
    glm::vec3 childWorldPos = glm::vec3(childTransformAfter.worldMatrix[3]);
    EXPECT_NEAR(childWorldPos.x, 15.0f, 0.0001f);
    EXPECT_NEAR(childWorldPos.y, 30.0f, 0.0001f);
    EXPECT_NEAR(childWorldPos.z, 45.0f, 0.0001f);
}

TEST_F(TransformHierarchySystemTest, RotationInheritance) {
    // Parent rotated 90 degrees around Z axis
    glm::quat parentRotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    auto parent = createRootEntity(0, glm::vec3(0.0f, 0.0f, 0.0f), parentRotation);

    auto child = coordinator->createEntity();
    entities.push_back(child);
    components::TransformComponent childTransform;
    childTransform.pos = glm::vec3(1.0f, 0.0f, 0.0f);  // Offset in X
    childTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    childTransform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(child, childTransform);
    components::SceneTag sceneTag;
    sceneTag.id = 0;
    coordinator->addComponent(child, sceneTag);

    addChildToParent(parent, child);
    setRenderedScene(0);
    system->update();

    auto& childTransformAfter = coordinator->getComponent<components::TransformComponent>(child);

    // After 90 degree rotation around Z, child at (1,0,0) should be at (0,1,0)
    glm::vec3 childWorldPos = glm::vec3(childTransformAfter.worldMatrix[3]);
    EXPECT_NEAR(childWorldPos.x, 0.0f, 0.0001f);
    EXPECT_NEAR(childWorldPos.y, 1.0f, 0.0001f);
    EXPECT_NEAR(childWorldPos.z, 0.0f, 0.0001f);
}

TEST_F(TransformHierarchySystemTest, ScaleInheritance) {
    auto parent = createRootEntity(0, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));

    auto child = coordinator->createEntity();
    entities.push_back(child);
    components::TransformComponent childTransform;
    childTransform.pos = glm::vec3(5.0f, 0.0f, 0.0f);
    childTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    childTransform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(child, childTransform);
    components::SceneTag sceneTag;
    sceneTag.id = 0;
    coordinator->addComponent(child, sceneTag);

    addChildToParent(parent, child);
    setRenderedScene(0);
    system->update();

    auto& childTransformAfter = coordinator->getComponent<components::TransformComponent>(child);

    // Child position should be scaled by parent: 5 * 2 = 10
    glm::vec3 childWorldPos = glm::vec3(childTransformAfter.worldMatrix[3]);
    EXPECT_NEAR(childWorldPos.x, 10.0f, 0.0001f);
}

TEST_F(TransformHierarchySystemTest, CombinedTransformations) {
    // Parent with translation, rotation, and scale
    glm::quat parentRotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    auto parent = createRootEntity(0, glm::vec3(10.0f, 5.0f, 0.0f), parentRotation, glm::vec3(2.0f, 2.0f, 2.0f));

    auto child = coordinator->createEntity();
    entities.push_back(child);
    components::TransformComponent childTransform;
    childTransform.pos = glm::vec3(3.0f, 0.0f, 0.0f);
    childTransform.quat = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    childTransform.size = glm::vec3(1.5f, 1.5f, 1.5f);
    coordinator->addComponent(child, childTransform);
    components::SceneTag sceneTag;
    sceneTag.id = 0;
    coordinator->addComponent(child, sceneTag);

    addChildToParent(parent, child);
    setRenderedScene(0);
    system->update();

    auto& childTransformAfter = coordinator->getComponent<components::TransformComponent>(child);

    // Calculate expected matrices
    glm::mat4 parentWorld = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 5.0f, 0.0f)) *
                            glm::toMat4(parentRotation) *
                            glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));

    glm::mat4 childLocal = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f)) *
                           glm::toMat4(childTransform.quat) *
                           glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));

    glm::mat4 childWorld = parentWorld * childLocal;

    EXPECT_TRUE(compareMat4(childTransformAfter.worldMatrix, childWorld));
}

// =============================================================================
// Scene Filtering Tests
// =============================================================================

TEST_F(TransformHierarchySystemTest, OnlyUpdateCurrentScene) {
    // Create entities in different scenes
    auto scene0Root = createRootEntity(0, glm::vec3(100.0f, 0.0f, 0.0f));
    auto scene1Root = createRootEntity(1, glm::vec3(200.0f, 0.0f, 0.0f));

    setRenderedScene(0);
    system->update();

    auto& scene0Transform = coordinator->getComponent<components::TransformComponent>(scene0Root);
    auto& scene1Transform = coordinator->getComponent<components::TransformComponent>(scene1Root);

    // Scene 0 should be updated, scene 1 should not
    EXPECT_NE(scene0Transform.worldMatrix, glm::mat4(0.0f));
    EXPECT_EQ(scene1Transform.worldMatrix, glm::mat4(0.0f));
}

TEST_F(TransformHierarchySystemTest, MultipleScenesDontInterfere) {
    // Create hierarchies in different scenes
    auto scene0Root = createRootEntity(0, glm::vec3(10.0f, 0.0f, 0.0f));
    auto scene1Root = createRootEntity(1, glm::vec3(20.0f, 0.0f, 0.0f));

    // Add children to both
    auto scene0Child = coordinator->createEntity();
    entities.push_back(scene0Child);
    components::TransformComponent scene0ChildTransform;
    scene0ChildTransform.pos = glm::vec3(5.0f, 0.0f, 0.0f);
    scene0ChildTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    scene0ChildTransform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(scene0Child, scene0ChildTransform);
    components::SceneTag sceneTag0;
    sceneTag0.id = 0;
    coordinator->addComponent(scene0Child, sceneTag0);
    addChildToParent(scene0Root, scene0Child);

    auto scene1Child = coordinator->createEntity();
    entities.push_back(scene1Child);
    components::TransformComponent scene1ChildTransform;
    scene1ChildTransform.pos = glm::vec3(8.0f, 0.0f, 0.0f);
    scene1ChildTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    scene1ChildTransform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(scene1Child, scene1ChildTransform);
    components::SceneTag sceneTag1;
    sceneTag1.id = 1;
    coordinator->addComponent(scene1Child, sceneTag1);
    addChildToParent(scene1Root, scene1Child);

    // Update scene 0
    setRenderedScene(0);
    system->update();

    auto& scene0RootTransform = coordinator->getComponent<components::TransformComponent>(scene0Root);
    auto& scene0ChildTransformAfter = coordinator->getComponent<components::TransformComponent>(scene0Child);
    auto& scene1RootTransform = coordinator->getComponent<components::TransformComponent>(scene1Root);
    auto& scene1ChildTransformAfter = coordinator->getComponent<components::TransformComponent>(scene1Child);

    // Scene 0 entities should be updated
    EXPECT_NE(scene0RootTransform.worldMatrix, glm::mat4(0.0f));
    EXPECT_NE(scene0ChildTransformAfter.worldMatrix, glm::mat4(0.0f));

    // Scene 1 entities should NOT be updated
    EXPECT_EQ(scene1RootTransform.worldMatrix, glm::mat4(0.0f));
    EXPECT_EQ(scene1ChildTransformAfter.worldMatrix, glm::mat4(0.0f));

    // Now update scene 1
    setRenderedScene(1);
    system->update();

    auto& scene1RootTransformUpdated = coordinator->getComponent<components::TransformComponent>(scene1Root);
    auto& scene1ChildTransformUpdated = coordinator->getComponent<components::TransformComponent>(scene1Child);

    // Scene 1 entities should now be updated
    EXPECT_NE(scene1RootTransformUpdated.worldMatrix, glm::mat4(0.0f));
    EXPECT_NE(scene1ChildTransformUpdated.worldMatrix, glm::mat4(0.0f));
}

TEST_F(TransformHierarchySystemTest, NoSceneRendered) {
    auto root = createRootEntity(0, glm::vec3(10.0f, 0.0f, 0.0f));

    // Set sceneRendered to -1 (no scene)
    auto& renderContext = coordinator->getSingletonComponent<components::RenderContext>();
    renderContext.sceneRendered = -1;

    system->update();

    auto& transform = coordinator->getComponent<components::TransformComponent>(root);

    // Should not be updated
    EXPECT_EQ(transform.worldMatrix, glm::mat4(0.0f));
}

// =============================================================================
// Edge Cases Tests
// =============================================================================

TEST_F(TransformHierarchySystemTest, EntityWithoutTransformComponent) {
    // Create root with child that has no transform
    auto parent = createRootEntity(0, glm::vec3(10.0f, 0.0f, 0.0f));

    auto child = coordinator->createEntity();
    entities.push_back(child);
    // Don't add TransformComponent to child
    components::SceneTag sceneTag;
    sceneTag.id = 0;
    coordinator->addComponent(child, sceneTag);

    // Add child to parent's children list (though child has no transform)
    auto& parentTransform = coordinator->getComponent<components::TransformComponent>(parent);
    parentTransform.children.push_back(child);

    setRenderedScene(0);

    // Should not crash
    EXPECT_NO_THROW(system->update());

    // Parent should still be updated correctly
    auto& parentTransformAfter = coordinator->getComponent<components::TransformComponent>(parent);
    EXPECT_NE(parentTransformAfter.worldMatrix, glm::mat4(0.0f));
}

TEST_F(TransformHierarchySystemTest, DeepHierarchy) {
    // Create a 5-level hierarchy
    std::vector<ecs::Entity> hierarchy;

    // Level 0 (root)
    auto root = createRootEntity(0, glm::vec3(1.0f, 0.0f, 0.0f));
    hierarchy.push_back(root);

    // Levels 1-4
    for (int i = 0; i < 4; ++i) {
        auto entity = coordinator->createEntity();
        entities.push_back(entity);

        components::TransformComponent transform;
        transform.pos = glm::vec3(1.0f, 0.0f, 0.0f);
        transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        transform.size = glm::vec3(1.0f, 1.0f, 1.0f);
        coordinator->addComponent(entity, transform);

        components::SceneTag sceneTag;
        sceneTag.id = 0;
        coordinator->addComponent(entity, sceneTag);

        addChildToParent(hierarchy.back(), entity);
        hierarchy.push_back(entity);
    }

    setRenderedScene(0);
    system->update();

    // Verify each level has the correct world position
    for (size_t i = 0; i < hierarchy.size(); ++i) {
        auto& transform = coordinator->getComponent<components::TransformComponent>(hierarchy[i]);
        glm::vec3 worldPos = glm::vec3(transform.worldMatrix[3]);

        // Each level adds 1.0 to X
        float expectedX = static_cast<float>(i + 1);
        EXPECT_NEAR(worldPos.x, expectedX, 0.0001f);
    }
}

TEST_F(TransformHierarchySystemTest, MultipleChildrenUnderSameParent) {
    auto parent = createRootEntity(0, glm::vec3(10.0f, 0.0f, 0.0f));

    // Create 3 children
    std::vector<ecs::Entity> children;
    for (int i = 0; i < 3; ++i) {
        auto child = coordinator->createEntity();
        entities.push_back(child);

        components::TransformComponent transform;
        transform.pos = glm::vec3(static_cast<float>(i + 1), 0.0f, 0.0f);
        transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        transform.size = glm::vec3(1.0f, 1.0f, 1.0f);
        coordinator->addComponent(child, transform);

        components::SceneTag sceneTag;
        sceneTag.id = 0;
        coordinator->addComponent(child, sceneTag);

        addChildToParent(parent, child);
        children.push_back(child);
    }

    setRenderedScene(0);
    system->update();

    // Verify all children have correct world positions
    for (int i = 0; i < 3; ++i) {
        auto& transform = coordinator->getComponent<components::TransformComponent>(children[i]);
        glm::vec3 worldPos = glm::vec3(transform.worldMatrix[3]);

        float expectedX = 10.0f + static_cast<float>(i + 1);
        EXPECT_NEAR(worldPos.x, expectedX, 0.0001f);
    }
}

TEST_F(TransformHierarchySystemTest, EmptyChildrenList) {
    // Create root with no children
    auto root = createRootEntity(0, glm::vec3(10.0f, 0.0f, 0.0f));

    setRenderedScene(0);

    // Should not crash
    EXPECT_NO_THROW(system->update());

    auto& transform = coordinator->getComponent<components::TransformComponent>(root);
    EXPECT_NE(transform.worldMatrix, glm::mat4(0.0f));
}

// =============================================================================
// Update Order Tests
// =============================================================================

TEST_F(TransformHierarchySystemTest, ParentUpdatedBeforeChildren) {
    // Create hierarchy with specific transformations
    glm::quat parentRotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    auto parent = createRootEntity(0, glm::vec3(10.0f, 0.0f, 0.0f), parentRotation, glm::vec3(2.0f, 2.0f, 2.0f));

    auto child = coordinator->createEntity();
    entities.push_back(child);
    components::TransformComponent childTransform;
    childTransform.pos = glm::vec3(5.0f, 0.0f, 0.0f);
    childTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    childTransform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(child, childTransform);
    components::SceneTag sceneTag;
    sceneTag.id = 0;
    coordinator->addComponent(child, sceneTag);
    addChildToParent(parent, child);

    setRenderedScene(0);
    system->update();

    auto& parentTransformAfter = coordinator->getComponent<components::TransformComponent>(parent);
    auto& childTransformAfter = coordinator->getComponent<components::TransformComponent>(child);

    // Verify parent world matrix is calculated correctly
    glm::mat4 expectedParentWorld = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f)) *
                                    glm::toMat4(parentRotation) *
                                    glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));

    EXPECT_TRUE(compareMat4(parentTransformAfter.worldMatrix, expectedParentWorld));

    // Verify child world matrix uses parent's world matrix
    glm::mat4 childLocal = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f));
    glm::mat4 expectedChildWorld = expectedParentWorld * childLocal;

    EXPECT_TRUE(compareMat4(childTransformAfter.worldMatrix, expectedChildWorld));
}

TEST_F(TransformHierarchySystemTest, SiblingsUpdatedCorrectly) {
    auto parent = createRootEntity(0, glm::vec3(10.0f, 0.0f, 0.0f));

    // Create two siblings
    auto sibling1 = coordinator->createEntity();
    entities.push_back(sibling1);
    components::TransformComponent sibling1Transform;
    sibling1Transform.pos = glm::vec3(1.0f, 0.0f, 0.0f);
    sibling1Transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    sibling1Transform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(sibling1, sibling1Transform);
    components::SceneTag sceneTag1;
    sceneTag1.id = 0;
    coordinator->addComponent(sibling1, sceneTag1);
    addChildToParent(parent, sibling1);

    auto sibling2 = coordinator->createEntity();
    entities.push_back(sibling2);
    components::TransformComponent sibling2Transform;
    sibling2Transform.pos = glm::vec3(2.0f, 0.0f, 0.0f);
    sibling2Transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    sibling2Transform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(sibling2, sibling2Transform);
    components::SceneTag sceneTag2;
    sceneTag2.id = 0;
    coordinator->addComponent(sibling2, sceneTag2);
    addChildToParent(parent, sibling2);

    setRenderedScene(0);
    system->update();

    auto& sibling1TransformAfter = coordinator->getComponent<components::TransformComponent>(sibling1);
    auto& sibling2TransformAfter = coordinator->getComponent<components::TransformComponent>(sibling2);

    // Both siblings should be updated correctly with their own local transforms
    glm::vec3 sibling1WorldPos = glm::vec3(sibling1TransformAfter.worldMatrix[3]);
    glm::vec3 sibling2WorldPos = glm::vec3(sibling2TransformAfter.worldMatrix[3]);

    EXPECT_NEAR(sibling1WorldPos.x, 11.0f, 0.0001f);
    EXPECT_NEAR(sibling2WorldPos.x, 12.0f, 0.0001f);
}

// =============================================================================
// Complex Transformation Tests
// =============================================================================

TEST_F(TransformHierarchySystemTest, NestedRotations) {
    // Parent rotated 90 degrees around Z
    glm::quat parentRotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    auto parent = createRootEntity(0, glm::vec3(0.0f, 0.0f, 0.0f), parentRotation);

    // Child also rotated 90 degrees around Z
    auto child = coordinator->createEntity();
    entities.push_back(child);
    components::TransformComponent childTransform;
    childTransform.pos = glm::vec3(1.0f, 0.0f, 0.0f);
    childTransform.quat = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    childTransform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(child, childTransform);
    components::SceneTag sceneTag;
    sceneTag.id = 0;
    coordinator->addComponent(child, sceneTag);
    addChildToParent(parent, child);

    setRenderedScene(0);
    system->update();

    auto& childTransformAfter = coordinator->getComponent<components::TransformComponent>(child);

    // Calculate expected matrices
    glm::mat4 parentWorld = glm::toMat4(parentRotation);
    glm::mat4 childLocal = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
                           glm::toMat4(childTransform.quat);
    glm::mat4 expectedChildWorld = parentWorld * childLocal;

    EXPECT_TRUE(compareMat4(childTransformAfter.worldMatrix, expectedChildWorld));
}

TEST_F(TransformHierarchySystemTest, NestedScales) {
    // Parent with scale 2x
    auto parent = createRootEntity(0, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));

    // Child with scale 3x
    auto child = coordinator->createEntity();
    entities.push_back(child);
    components::TransformComponent childTransform;
    childTransform.pos = glm::vec3(1.0f, 0.0f, 0.0f);
    childTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    childTransform.size = glm::vec3(3.0f, 3.0f, 3.0f);
    coordinator->addComponent(child, childTransform);
    components::SceneTag sceneTag;
    sceneTag.id = 0;
    coordinator->addComponent(child, sceneTag);
    addChildToParent(parent, child);

    setRenderedScene(0);
    system->update();

    auto& childTransformAfter = coordinator->getComponent<components::TransformComponent>(child);

    // Calculate expected world matrix
    glm::mat4 parentWorld = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    glm::mat4 childLocal = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
                           glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));
    glm::mat4 expectedChildWorld = parentWorld * childLocal;

    EXPECT_TRUE(compareMat4(childTransformAfter.worldMatrix, expectedChildWorld));
}

TEST_F(TransformHierarchySystemTest, NegativeScale) {
    // Parent with negative scale (mirror)
    auto parent = createRootEntity(0, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 1.0f, 1.0f));

    auto child = coordinator->createEntity();
    entities.push_back(child);
    components::TransformComponent childTransform;
    childTransform.pos = glm::vec3(5.0f, 0.0f, 0.0f);
    childTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    childTransform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(child, childTransform);
    components::SceneTag sceneTag;
    sceneTag.id = 0;
    coordinator->addComponent(child, sceneTag);
    addChildToParent(parent, child);

    setRenderedScene(0);
    system->update();

    auto& childTransformAfter = coordinator->getComponent<components::TransformComponent>(child);

    // Child position should be mirrored: -5 instead of +5
    glm::vec3 childWorldPos = glm::vec3(childTransformAfter.worldMatrix[3]);
    EXPECT_NEAR(childWorldPos.x, -5.0f, 0.0001f);
}

TEST_F(TransformHierarchySystemTest, IdentityTransformPropagation) {
    // All identity transforms
    auto parent = createRootEntity(0);

    auto child = coordinator->createEntity();
    entities.push_back(child);
    components::TransformComponent childTransform;
    childTransform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    childTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    childTransform.size = glm::vec3(1.0f, 1.0f, 1.0f);
    coordinator->addComponent(child, childTransform);
    components::SceneTag sceneTag;
    sceneTag.id = 0;
    coordinator->addComponent(child, sceneTag);
    addChildToParent(parent, child);

    setRenderedScene(0);
    system->update();

    auto& parentTransformAfter = coordinator->getComponent<components::TransformComponent>(parent);
    auto& childTransformAfter = coordinator->getComponent<components::TransformComponent>(child);

    // Both should be identity
    EXPECT_TRUE(compareMat4(parentTransformAfter.worldMatrix, glm::mat4(1.0f)));
    EXPECT_TRUE(compareMat4(childTransformAfter.worldMatrix, glm::mat4(1.0f)));
}

}  // namespace nexo::system
