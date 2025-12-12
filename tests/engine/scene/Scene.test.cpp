///////////////////////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      iMeaNz
//  Date:        17/03/2025
//  Description: Test file for the Scene class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/scene/Scene.hpp"
#include "components/SceneComponents.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "ecs/Coordinator.hpp"

namespace nexo::scene {

class SceneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset scene ID counter for consistent test behavior
        nextSceneId = 0;
        coordinator = std::make_shared<nexo::ecs::Coordinator>();
        coordinator->init();
        coordinator->registerComponent<components::SceneTag>();
        coordinator->registerComponent<components::TransformComponent>();
    }

    std::shared_ptr<nexo::ecs::Coordinator> coordinator;
};

TEST_F(SceneTest, Constructor) {
    // Test that scene is properly initialized with given name and coordinator
    std::string sceneName = "TestScene";
    Scene scene(sceneName, coordinator);

    EXPECT_EQ(scene.getName(), sceneName);
    EXPECT_TRUE(scene.isActive());
    EXPECT_TRUE(scene.isRendered());
    EXPECT_FALSE(scene.getUuid().empty());
}

TEST_F(SceneTest, AddEntity) {
    Scene scene("TestScene", coordinator);
    nexo::ecs::Entity entity = coordinator->createEntity();

    // Create a scene tag that the Scene can update
    components::SceneTag expectedTag{scene.getId(), true, true};

    scene.addEntity(entity);
    auto tag = coordinator->tryGetComponent<components::SceneTag>(entity);

    EXPECT_TRUE(tag);
    EXPECT_EQ(tag->get().id, expectedTag.id);
    EXPECT_EQ(tag->get().isActive, expectedTag.isActive);
    EXPECT_EQ(tag->get().isRendered, expectedTag.isRendered);
}

TEST_F(SceneTest, RemoveEntity) {
    Scene scene("TestScene", coordinator);
    nexo::ecs::Entity entity = coordinator->createEntity();

    // First add the entity
    scene.addEntity(entity);

    // Verify entity has the tag
    auto tagBefore = coordinator->tryGetComponent<components::SceneTag>(entity);
    EXPECT_TRUE(tagBefore);

    // Remove the entity
    scene.removeEntity(entity);

    // Verify tag has been removed
    auto tagAfter = coordinator->tryGetComponent<components::SceneTag>(entity);
    EXPECT_FALSE(tagAfter);
}

TEST_F(SceneTest, SetActiveStatus) {
    Scene scene("TestScene", coordinator);
    nexo::ecs::Entity entity1 = coordinator->createEntity();
    nexo::ecs::Entity entity2 = coordinator->createEntity();

    // Add entities to the scene
    scene.addEntity(entity1);
    scene.addEntity(entity2);

    // Now set the active status to false
    scene.setActiveStatus(false);

    // Check that the scene is now inactive
    EXPECT_FALSE(scene.isActive());

    // Check that the tags were updated
    auto tag1 = coordinator->tryGetComponent<components::SceneTag>(entity1);
    auto tag2 = coordinator->tryGetComponent<components::SceneTag>(entity2);

    EXPECT_TRUE(tag1);
    EXPECT_TRUE(tag2);
    EXPECT_FALSE(tag1->get().isActive);
    EXPECT_FALSE(tag2->get().isActive);
}

TEST_F(SceneTest, SetRenderStatus) {
    Scene scene("TestScene", coordinator);
    nexo::ecs::Entity entity1 = coordinator->createEntity();
    nexo::ecs::Entity entity2 = coordinator->createEntity();

    // Add entities to the scene
    scene.addEntity(entity1);
    scene.addEntity(entity2);

    // Now set the render status to false
    scene.setRenderStatus(false);

    // Check that the scene is now not rendered
    EXPECT_FALSE(scene.isRendered());

    // Check that the tags were updated
    auto tag1 = coordinator->tryGetComponent<components::SceneTag>(entity1);
    auto tag2 = coordinator->tryGetComponent<components::SceneTag>(entity2);

    EXPECT_TRUE(tag1);
    EXPECT_TRUE(tag2);
    EXPECT_FALSE(tag1->get().isRendered);
    EXPECT_FALSE(tag2->get().isRendered);
}

TEST_F(SceneTest, SceneDestructor) {
    nexo::ecs::Entity entity1;
    nexo::ecs::Entity entity2;

    {
        Scene scene("TestScene", coordinator);

        // Add entities to the scene
        entity1 = coordinator->createEntity();
        entity2 = coordinator->createEntity();

        scene.addEntity(entity1);
        scene.addEntity(entity2);

        // Scene will be destructed when this scope ends
    }

    // After scene destruction, entities should be destroyed
    // Testing this depends on how coordinator handles destroyed entities
    // This could be checking for an exception or a specific error state
    EXPECT_THROW(coordinator->getComponent<components::SceneTag>(entity1), ecs::ComponentNotFound);
    EXPECT_THROW(coordinator->getComponent<components::SceneTag>(entity2), ecs::ComponentNotFound);
}

TEST_F(SceneTest, NameGetterSetter) {
    Scene scene("InitialName", coordinator);

    EXPECT_EQ(scene.getName(), "InitialName");

    scene.setName("NewName");

    EXPECT_EQ(scene.getName(), "NewName");
}

TEST_F(SceneTest, SceneIdGeneration) {
    // Reset the nextSceneId to ensure consistent test behavior
    nextSceneId = 0;

    Scene scene1("Scene1", coordinator);
    Scene scene2("Scene2", coordinator);

    EXPECT_EQ(scene1.getId(), 0);
    EXPECT_EQ(scene2.getId(), 1);
}

TEST_F(SceneTest, UuidGeneration) {
    Scene scene1("Scene1", coordinator);
    Scene scene2("Scene2", coordinator);

    // Each scene should have a unique UUID
    EXPECT_NE(scene1.getUuid(), scene2.getUuid());
    EXPECT_FALSE(scene1.getUuid().empty());
    EXPECT_FALSE(scene2.getUuid().empty());
}

TEST_F(SceneTest, AddMultipleEntities) {
    Scene scene("TestScene", coordinator);

    std::vector<nexo::ecs::Entity> entities;
    for (int i = 0; i < 5; i++) {
        entities.push_back(coordinator->createEntity());
    }

    for (auto entity : entities) {
        scene.addEntity(entity);

        // Verify each entity has a SceneTag component
        auto tag = coordinator->tryGetComponent<components::SceneTag>(entity);
        EXPECT_TRUE(tag);
        EXPECT_EQ(tag->get().id, scene.getId());
    }
}

TEST_F(SceneTest, ToggleActiveStatus) {
    Scene scene("TestScene", coordinator);
    nexo::ecs::Entity entity = coordinator->createEntity();

    // Add entity to scene
    scene.addEntity(entity);

    // Toggle active status multiple times
    scene.setActiveStatus(false);
    EXPECT_FALSE(scene.isActive());

    auto tag1 = coordinator->tryGetComponent<components::SceneTag>(entity);
    EXPECT_TRUE(tag1);
    EXPECT_FALSE(tag1->get().isActive);

    scene.setActiveStatus(true);
    EXPECT_TRUE(scene.isActive());

    auto tag2 = coordinator->tryGetComponent<components::SceneTag>(entity);
    EXPECT_TRUE(tag2);
    EXPECT_TRUE(tag2->get().isActive);
}

TEST_F(SceneTest, ToggleRenderStatus) {
    Scene scene("TestScene", coordinator);
    nexo::ecs::Entity entity = coordinator->createEntity();

    // Add entity to scene
    scene.addEntity(entity);

    // Toggle render status multiple times
    scene.setRenderStatus(false);
    EXPECT_FALSE(scene.isRendered());

    auto tag1 = coordinator->tryGetComponent<components::SceneTag>(entity);
    EXPECT_TRUE(tag1);
    EXPECT_FALSE(tag1->get().isRendered);

    scene.setRenderStatus(true);
    EXPECT_TRUE(scene.isRendered());

    auto tag2 = coordinator->tryGetComponent<components::SceneTag>(entity);
    EXPECT_TRUE(tag2);
    EXPECT_TRUE(tag2->get().isRendered);
}

TEST_F(SceneTest, AddRemoveMultipleTimes) {
    Scene scene("TestScene", coordinator);
    nexo::ecs::Entity entity = coordinator->createEntity();

    // Add entity
    scene.addEntity(entity);
    auto tag1 = coordinator->tryGetComponent<components::SceneTag>(entity);
    EXPECT_TRUE(tag1);

    // Remove entity
    scene.removeEntity(entity);
    auto tag2 = coordinator->tryGetComponent<components::SceneTag>(entity);
    EXPECT_FALSE(tag2);

    // Add entity again
    scene.addEntity(entity);
    auto tag3 = coordinator->tryGetComponent<components::SceneTag>(entity);
    EXPECT_TRUE(tag3);
}

TEST_F(SceneTest, EmptySceneDestruction) {
    // Create a scene without adding entities to it
    {
        Scene scene("EmptyScene", coordinator);
        // Scene will be destructed when this scope ends
    }
    // No assertions needed - just making sure it doesn't crash
}

TEST_F(SceneTest, SetActiveStatusNoEntities) {
    Scene scene("TestScene", coordinator);

    // Should not crash when there are no entities
    scene.setActiveStatus(false);
    EXPECT_FALSE(scene.isActive());

    scene.setActiveStatus(true);
    EXPECT_TRUE(scene.isActive());
}

TEST_F(SceneTest, SetRenderStatusNoEntities) {
    Scene scene("TestScene", coordinator);

    // Should not crash when there are no entities
    scene.setRenderStatus(false);
    EXPECT_FALSE(scene.isRendered());

    scene.setRenderStatus(true);
    EXPECT_TRUE(scene.isRendered());
}

TEST_F(SceneTest, ConstructorWithEditorOnlyFlag) {
    // Test creating a scene with editorOnly=true parameter
    std::string sceneName = "EditorScene";
    Scene editorScene(sceneName, coordinator, true);

    EXPECT_EQ(editorScene.getName(), sceneName);
    EXPECT_TRUE(editorScene.isActive());
    EXPECT_TRUE(editorScene.isRendered());
    EXPECT_FALSE(editorScene.getUuid().empty());
}

TEST_F(SceneTest, ConstructorWithEditorOnlyFalse) {
    // Test creating a scene with editorOnly=false parameter explicitly
    std::string sceneName = "RuntimeScene";
    Scene runtimeScene(sceneName, coordinator, false);

    EXPECT_EQ(runtimeScene.getName(), sceneName);
    EXPECT_TRUE(runtimeScene.isActive());
    EXPECT_TRUE(runtimeScene.isRendered());
    EXPECT_FALSE(runtimeScene.getUuid().empty());
}

TEST_F(SceneTest, GetEntitiesExplicit) {
    // Explicitly test getEntities() method
    Scene scene("TestScene", coordinator);

    // Initially, the scene should have no entities
    EXPECT_TRUE(scene.getEntities().empty());

    // Add some entities
    nexo::ecs::Entity entity1 = coordinator->createEntity();
    nexo::ecs::Entity entity2 = coordinator->createEntity();
    nexo::ecs::Entity entity3 = coordinator->createEntity();

    scene.addEntity(entity1);
    scene.addEntity(entity2);
    scene.addEntity(entity3);

    // Get the entities set
    const std::set<nexo::ecs::Entity>& entities = scene.getEntities();

    // Verify the set contains all three entities
    EXPECT_EQ(entities.size(), 3);
    EXPECT_TRUE(entities.find(entity1) != entities.end());
    EXPECT_TRUE(entities.find(entity2) != entities.end());
    EXPECT_TRUE(entities.find(entity3) != entities.end());

    // Remove one entity
    scene.removeEntity(entity2);

    // Verify the set now contains only two entities
    const std::set<nexo::ecs::Entity>& updatedEntities = scene.getEntities();
    EXPECT_EQ(updatedEntities.size(), 2);
    EXPECT_TRUE(updatedEntities.find(entity1) != updatedEntities.end());
    EXPECT_FALSE(updatedEntities.find(entity2) != updatedEntities.end());
    EXPECT_TRUE(updatedEntities.find(entity3) != updatedEntities.end());
}

TEST_F(SceneTest, AddChildEntityToSceneSimple) {
    // Test addChildEntityToScene with a single parent-child relationship
    Scene scene("TestScene", coordinator);

    // Create parent and child entities
    nexo::ecs::Entity parentEntity = coordinator->createEntity();
    nexo::ecs::Entity childEntity = coordinator->createEntity();

    // Add transform components to both
    components::TransformComponent parentTransform;
    components::TransformComponent childTransform;

    // Setup parent-child relationship
    parentTransform.children.push_back(childEntity);

    coordinator->addComponent<components::TransformComponent>(parentEntity, parentTransform);
    coordinator->addComponent<components::TransformComponent>(childEntity, childTransform);

    // Add parent entity to scene - should automatically add child
    scene.addEntity(parentEntity);

    // Verify both parent and child are in the scene
    const std::set<nexo::ecs::Entity>& entities = scene.getEntities();
    EXPECT_EQ(entities.size(), 2);
    EXPECT_TRUE(entities.find(parentEntity) != entities.end());
    EXPECT_TRUE(entities.find(childEntity) != entities.end());

    // Verify both have scene tags
    auto parentTag = coordinator->tryGetComponent<components::SceneTag>(parentEntity);
    auto childTag = coordinator->tryGetComponent<components::SceneTag>(childEntity);

    EXPECT_TRUE(parentTag);
    EXPECT_TRUE(childTag);
    EXPECT_EQ(parentTag->get().id, scene.getId());
    EXPECT_EQ(childTag->get().id, scene.getId());
}

TEST_F(SceneTest, AddChildEntityToSceneNested) {
    // Test addChildEntityToScene with nested parent-child-grandchild relationships
    Scene scene("TestScene", coordinator);

    // Create parent, child, and grandchild entities
    nexo::ecs::Entity parentEntity = coordinator->createEntity();
    nexo::ecs::Entity childEntity = coordinator->createEntity();
    nexo::ecs::Entity grandchildEntity = coordinator->createEntity();

    // Add transform components
    components::TransformComponent parentTransform;
    components::TransformComponent childTransform;
    components::TransformComponent grandchildTransform;

    // Setup nested hierarchy
    parentTransform.children.push_back(childEntity);
    childTransform.children.push_back(grandchildEntity);

    coordinator->addComponent<components::TransformComponent>(parentEntity, parentTransform);
    coordinator->addComponent<components::TransformComponent>(childEntity, childTransform);
    coordinator->addComponent<components::TransformComponent>(grandchildEntity, grandchildTransform);

    // Add parent entity to scene - should recursively add all children
    scene.addEntity(parentEntity);

    // Verify all three entities are in the scene
    const std::set<nexo::ecs::Entity>& entities = scene.getEntities();
    EXPECT_EQ(entities.size(), 3);
    EXPECT_TRUE(entities.find(parentEntity) != entities.end());
    EXPECT_TRUE(entities.find(childEntity) != entities.end());
    EXPECT_TRUE(entities.find(grandchildEntity) != entities.end());

    // Verify all have scene tags with correct scene ID
    auto parentTag = coordinator->tryGetComponent<components::SceneTag>(parentEntity);
    auto childTag = coordinator->tryGetComponent<components::SceneTag>(childEntity);
    auto grandchildTag = coordinator->tryGetComponent<components::SceneTag>(grandchildEntity);

    EXPECT_TRUE(parentTag);
    EXPECT_TRUE(childTag);
    EXPECT_TRUE(grandchildTag);
    EXPECT_EQ(parentTag->get().id, scene.getId());
    EXPECT_EQ(childTag->get().id, scene.getId());
    EXPECT_EQ(grandchildTag->get().id, scene.getId());
}

TEST_F(SceneTest, AddChildEntityToSceneMultipleChildren) {
    // Test addChildEntityToScene with a parent having multiple children
    Scene scene("TestScene", coordinator);

    // Create parent and three child entities
    nexo::ecs::Entity parentEntity = coordinator->createEntity();
    nexo::ecs::Entity child1 = coordinator->createEntity();
    nexo::ecs::Entity child2 = coordinator->createEntity();
    nexo::ecs::Entity child3 = coordinator->createEntity();

    // Add transform components
    components::TransformComponent parentTransform;
    components::TransformComponent child1Transform;
    components::TransformComponent child2Transform;
    components::TransformComponent child3Transform;

    // Setup parent with multiple children
    parentTransform.children.push_back(child1);
    parentTransform.children.push_back(child2);
    parentTransform.children.push_back(child3);

    coordinator->addComponent<components::TransformComponent>(parentEntity, parentTransform);
    coordinator->addComponent<components::TransformComponent>(child1, child1Transform);
    coordinator->addComponent<components::TransformComponent>(child2, child2Transform);
    coordinator->addComponent<components::TransformComponent>(child3, child3Transform);

    // Add parent entity to scene - should add all children
    scene.addEntity(parentEntity);

    // Verify all four entities are in the scene
    const std::set<nexo::ecs::Entity>& entities = scene.getEntities();
    EXPECT_EQ(entities.size(), 4);
    EXPECT_TRUE(entities.find(parentEntity) != entities.end());
    EXPECT_TRUE(entities.find(child1) != entities.end());
    EXPECT_TRUE(entities.find(child2) != entities.end());
    EXPECT_TRUE(entities.find(child3) != entities.end());

    // Verify all have scene tags
    auto parentTag = coordinator->tryGetComponent<components::SceneTag>(parentEntity);
    auto child1Tag = coordinator->tryGetComponent<components::SceneTag>(child1);
    auto child2Tag = coordinator->tryGetComponent<components::SceneTag>(child2);
    auto child3Tag = coordinator->tryGetComponent<components::SceneTag>(child3);

    EXPECT_TRUE(parentTag);
    EXPECT_TRUE(child1Tag);
    EXPECT_TRUE(child2Tag);
    EXPECT_TRUE(child3Tag);
}

TEST_F(SceneTest, AddChildEntityToSceneDuplicateProtection) {
    // Test that addChildEntityToScene doesn't add duplicate entities
    Scene scene("TestScene", coordinator);

    // Create entities
    nexo::ecs::Entity parentEntity = coordinator->createEntity();
    nexo::ecs::Entity childEntity = coordinator->createEntity();

    // Add transform components
    components::TransformComponent parentTransform;
    components::TransformComponent childTransform;

    parentTransform.children.push_back(childEntity);

    coordinator->addComponent<components::TransformComponent>(parentEntity, parentTransform);
    coordinator->addComponent<components::TransformComponent>(childEntity, childTransform);

    // First add child entity directly to the scene
    scene.addEntity(childEntity);

    // Verify child is in scene
    EXPECT_EQ(scene.getEntities().size(), 1);

    // Now add parent entity - should not duplicate child
    scene.addEntity(parentEntity);

    // Verify we have exactly 2 entities (parent and child, no duplicates)
    const std::set<nexo::ecs::Entity>& entities = scene.getEntities();
    EXPECT_EQ(entities.size(), 2);
    EXPECT_TRUE(entities.find(parentEntity) != entities.end());
    EXPECT_TRUE(entities.find(childEntity) != entities.end());
}

} // namespace nexo::scene
