///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      iMeaNz
//  Date:        17/03/2025
//  Description: Test file for the SceneManager class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/scene/SceneManager.hpp"
#include "core/scene/Scene.hpp"
#include "core/exceptions/Exceptions.hpp"
#include "ecs/Coordinator.hpp"

// Forward declarations to avoid circular dependencies
namespace nexo::ecs {
    class Coordinator;
}

namespace nexo::scene {

// Create a mock for the Coordinator class
class MockCoordinator : public nexo::ecs::Coordinator {
public:
    // Define the Entity type to match what's used in the real code
    using Entity = std::uint32_t;

    MOCK_METHOD(Entity, createEntity, (), ());
    MOCK_METHOD(void, destroyEntity, (Entity), ());

    // Additional methods the Coordinator might need for Scene operations
};

class SceneManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset nextSceneId to ensure predictable scene IDs in tests
        nexo::scene::nextSceneId = 0;

        // Create a mock coordinator
        mockCoordinator = std::make_shared<MockCoordinator>();

        // This cast is necessary since Scene expects a shared_ptr<ecs::Coordinator>
        coordinator = std::static_pointer_cast<nexo::ecs::Coordinator>(mockCoordinator);

        // Create a fresh SceneManager for each test
        manager = std::make_unique<SceneManager>();
        manager->setCoordinator(coordinator);
    }

    std::shared_ptr<MockCoordinator> mockCoordinator;
    std::shared_ptr<nexo::ecs::Coordinator> coordinator;
    std::unique_ptr<SceneManager> manager;
};

TEST_F(SceneManagerTest, Constructor) {
    // Test that SceneManager is properly constructed
    SceneManager newManager;
    // No assertions needed, just verifying it constructs without crashing
    SUCCEED();
}

TEST_F(SceneManagerTest, SetCoordinator) {
    // Test that we can set a coordinator
    SceneManager newManager;
    newManager.setCoordinator(coordinator);
    // No assertions needed, just verifying it sets without crashing
    SUCCEED();
}

TEST_F(SceneManagerTest, CreateScene) {
    // Test creating a scene
    std::string sceneName = "TestScene";
    unsigned int sceneId = manager->createScene(sceneName);

    // Scene IDs should start at 0
    EXPECT_EQ(sceneId, 0);

    // Verify we can retrieve the scene we just created
    Scene& scene = manager->getScene(sceneId);
    EXPECT_EQ(scene.getName(), sceneName);
    EXPECT_EQ(scene.getId(), sceneId);
}

TEST_F(SceneManagerTest, CreateMultipleScenes) {
    // Test creating multiple scenes
    std::string sceneName1 = "TestScene1";
    std::string sceneName2 = "TestScene2";
    std::string sceneName3 = "TestScene3";

    unsigned int sceneId1 = manager->createScene(sceneName1);
    unsigned int sceneId2 = manager->createScene(sceneName2);
    unsigned int sceneId3 = manager->createScene(sceneName3);

    // Scene IDs should be sequential
    EXPECT_EQ(sceneId1, 0);
    EXPECT_EQ(sceneId2, 1);
    EXPECT_EQ(sceneId3, 2);

    // Verify we can retrieve all scenes we created
    Scene& scene1 = manager->getScene(sceneId1);
    Scene& scene2 = manager->getScene(sceneId2);
    Scene& scene3 = manager->getScene(sceneId3);

    EXPECT_EQ(scene1.getName(), sceneName1);
    EXPECT_EQ(scene2.getName(), sceneName2);
    EXPECT_EQ(scene3.getName(), sceneName3);

    EXPECT_EQ(scene1.getId(), sceneId1);
    EXPECT_EQ(scene2.getId(), sceneId2);
    EXPECT_EQ(scene3.getId(), sceneId3);
}

TEST_F(SceneManagerTest, DeleteScene) {
    // Test deleting a scene
    std::string sceneName = "TestScene";
    unsigned int sceneId = manager->createScene(sceneName);

    // Verify the scene exists
    Scene& scene = manager->getScene(sceneId);
    EXPECT_EQ(scene.getName(), sceneName);

    // Delete the scene
    manager->deleteScene(sceneId);

    // Verify the scene no longer exists - should throw std::out_of_range
    EXPECT_THROW(manager->getScene(sceneId), std::out_of_range);
}

TEST_F(SceneManagerTest, DeleteSceneAndCreateNew) {
    // Test deleting a scene and creating a new one
    std::string sceneName1 = "TestScene1";
    unsigned int sceneId1 = manager->createScene(sceneName1);

    // Delete the scene
    manager->deleteScene(sceneId1);

    // Create a new scene
    std::string sceneName2 = "TestScene2";
    unsigned int sceneId2 = manager->createScene(sceneName2);

    // The new scene should have a new ID despite the deletion
    // Note: This tests the current implementation. If the behavior is supposed
    // to reuse IDs, this test would need to be adjusted.
    EXPECT_EQ(sceneId2, 1);

    // Verify the new scene exists
    Scene& scene2 = manager->getScene(sceneId2);
    EXPECT_EQ(scene2.getName(), sceneName2);
    EXPECT_EQ(scene2.getId(), sceneId2);
}

TEST_F(SceneManagerTest, GetNonExistentScene) {
    // Test getting a scene that doesn't exist
    EXPECT_THROW(manager->getScene(999), std::out_of_range);
}

TEST_F(SceneManagerTest, DeleteNonExistentScene) {
    // Test deleting a scene that doesn't exist
    // This should not throw an exception based on the current implementation
    // If the behavior is supposed to throw, this test would need to be adjusted
    EXPECT_NO_THROW(manager->deleteScene(999));
}

TEST_F(SceneManagerTest, CreateAndModifyScene) {
    // Test creating a scene and then modifying it
    std::string originalName = "OriginalName";
    std::string newName = "NewName";
    unsigned int sceneId = manager->createScene(originalName);

    // Get the scene and modify it
    Scene& scene = manager->getScene(sceneId);
    EXPECT_EQ(scene.getName(), originalName);

    scene.setName(newName);

    // Verify the modification persisted
    Scene& updatedScene = manager->getScene(sceneId);
    EXPECT_EQ(updatedScene.getName(), newName);
}

TEST_F(SceneManagerTest, SceneLifecycle) {
    // Test the entire lifecycle of a scene

    // Create scene
    std::string sceneName = "LifecycleScene";
    unsigned int sceneId = manager->createScene(sceneName);

    // Verify creation
    Scene& scene = manager->getScene(sceneId);
    EXPECT_EQ(scene.getName(), sceneName);

    // Modify scene
    scene.setActiveStatus(false);
    EXPECT_FALSE(scene.isActive());

    scene.setRenderStatus(false);
    EXPECT_FALSE(scene.isRendered());

    scene.setName("ModifiedLifecycleScene");
    EXPECT_EQ(scene.getName(), "ModifiedLifecycleScene");

    // Re-get scene to ensure modifications persisted
    Scene& modifiedScene = manager->getScene(sceneId);
    EXPECT_EQ(modifiedScene.getName(), "ModifiedLifecycleScene");
    EXPECT_FALSE(modifiedScene.isActive());
    EXPECT_FALSE(modifiedScene.isRendered());

    // Delete scene
    manager->deleteScene(sceneId);

    // Verify deletion
    EXPECT_THROW(manager->getScene(sceneId), std::out_of_range);
}

TEST_F(SceneManagerTest, CreateSceneWithoutCoordinator) {
    // Test creating a scene without setting a coordinator
    SceneManager newManager;

    // Since the coordinator is necessary for Scene construction,
    // this should throw an exception or handle the null coordinator gracefully
    // depending on the implementation
    std::string sceneName = "TestScene";
    EXPECT_THROW(newManager.createScene(sceneName), core::SceneManagerLifecycleException);
}

TEST_F(SceneManagerTest, CreateDeleteMultipleScenes) {
    // Test creating and deleting multiple scenes in various orders

    // Create several scenes
    unsigned int sceneId1 = manager->createScene("Scene1");
    unsigned int sceneId2 = manager->createScene("Scene2");
    unsigned int sceneId3 = manager->createScene("Scene3");
    unsigned int sceneId4 = manager->createScene("Scene4");

    // Delete scenes in a mixed order
    manager->deleteScene(sceneId2);
    manager->deleteScene(sceneId4);

    // Verify the correct scenes still exist
    EXPECT_NO_THROW(manager->getScene(sceneId1));
    EXPECT_THROW(manager->getScene(sceneId2), std::out_of_range);
    EXPECT_NO_THROW(manager->getScene(sceneId3));
    EXPECT_THROW(manager->getScene(sceneId4), std::out_of_range);

    // Create some new scenes
    unsigned int sceneId5 = manager->createScene("Scene5");
    unsigned int sceneId6 = manager->createScene("Scene6");

    // Verify all expected scenes exist
    EXPECT_NO_THROW(manager->getScene(sceneId1));
    EXPECT_THROW(manager->getScene(sceneId2), std::out_of_range);
    EXPECT_NO_THROW(manager->getScene(sceneId3));
    EXPECT_THROW(manager->getScene(sceneId4), std::out_of_range);
    EXPECT_NO_THROW(manager->getScene(sceneId5));
    EXPECT_NO_THROW(manager->getScene(sceneId6));

    // Verify the scene IDs are as expected
    EXPECT_EQ(sceneId5, 4);
    EXPECT_EQ(sceneId6, 5);
}

TEST_F(SceneManagerTest, CreateSceneAfterReset) {
    // Test creating a scene after resetting the SceneManager
    unsigned int sceneId1 = manager->createScene("Scene1");
    EXPECT_EQ(sceneId1, 0);

    // Reset the SceneManager
    manager.reset(new SceneManager());
    manager->setCoordinator(coordinator);

    // Create a new scene - should start from ID 1 since nextSceneId is static
    unsigned int sceneId2 = manager->createScene("Scene2");
    EXPECT_EQ(sceneId2, 1);
}

} // namespace nexo::scene
