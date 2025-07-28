///////////////////////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
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

} // namespace nexo::scene
