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

TEST_F(SceneManagerTest, CreateEditorScene) {
    // Test creating an editor-only scene
    std::string editorSceneName = "EditorScene";
    unsigned int editorSceneId = manager->createEditorScene(editorSceneName);

    // Scene IDs should start at 0
    EXPECT_EQ(editorSceneId, 0);

    // Verify we can retrieve the editor scene
    Scene& editorScene = manager->getScene(editorSceneId);
    EXPECT_EQ(editorScene.getName(), editorSceneName);
    EXPECT_EQ(editorScene.getId(), editorSceneId);
}

TEST_F(SceneManagerTest, CreateEditorSceneWithoutCoordinator) {
    // Test creating an editor scene without setting a coordinator
    SceneManager newManager;

    // This should throw an exception since coordinator is required
    std::string sceneName = "EditorTestScene";
    EXPECT_THROW(newManager.createEditorScene(sceneName), core::SceneManagerLifecycleException);
}

TEST_F(SceneManagerTest, CreateMixedScenes) {
    // Test creating both regular and editor scenes
    unsigned int regularSceneId1 = manager->createScene("RegularScene1");
    unsigned int editorSceneId1 = manager->createEditorScene("EditorScene1");
    unsigned int regularSceneId2 = manager->createScene("RegularScene2");
    unsigned int editorSceneId2 = manager->createEditorScene("EditorScene2");

    // Scene IDs should be sequential regardless of scene type
    EXPECT_EQ(regularSceneId1, 0);
    EXPECT_EQ(editorSceneId1, 1);
    EXPECT_EQ(regularSceneId2, 2);
    EXPECT_EQ(editorSceneId2, 3);

    // Verify all scenes exist and have correct names
    EXPECT_EQ(manager->getScene(regularSceneId1).getName(), "RegularScene1");
    EXPECT_EQ(manager->getScene(editorSceneId1).getName(), "EditorScene1");
    EXPECT_EQ(manager->getScene(regularSceneId2).getName(), "RegularScene2");
    EXPECT_EQ(manager->getScene(editorSceneId2).getName(), "EditorScene2");
}

TEST_F(SceneManagerTest, DeleteEditorScene) {
    // Test deleting an editor scene
    unsigned int editorSceneId = manager->createEditorScene("EditorScene");

    // Verify the editor scene exists
    Scene& editorScene = manager->getScene(editorSceneId);
    EXPECT_EQ(editorScene.getName(), "EditorScene");

    // Delete the editor scene
    manager->deleteScene(editorSceneId);

    // Verify the scene no longer exists
    EXPECT_THROW(manager->getScene(editorSceneId), std::out_of_range);
}

// ============================================================================
// Edge Case Tests
// ============================================================================

TEST_F(SceneManagerTest, DeleteSameSceneMultipleTimes) {
    // Test deleting the same scene multiple times
    unsigned int sceneId = manager->createScene("TestScene");

    // First deletion should work
    EXPECT_NO_THROW(manager->deleteScene(sceneId));

    // Second deletion should not crash (scene doesn't exist)
    EXPECT_NO_THROW(manager->deleteScene(sceneId));

    // Third deletion should also not crash
    EXPECT_NO_THROW(manager->deleteScene(sceneId));

    // Verify the scene still doesn't exist
    EXPECT_THROW(manager->getScene(sceneId), std::out_of_range);
}

TEST_F(SceneManagerTest, RapidSceneCreationAndDeletion) {
    // Test rapidly creating and deleting scenes
    std::vector<unsigned int> sceneIds;

    // Create 100 scenes rapidly
    for (int i = 0; i < 100; i++) {
        sceneIds.push_back(manager->createScene("Scene" + std::to_string(i)));
    }

    // Verify all scenes exist
    for (auto id : sceneIds) {
        EXPECT_NO_THROW(manager->getScene(id));
    }

    // Delete all scenes rapidly
    for (auto id : sceneIds) {
        manager->deleteScene(id);
    }

    // Verify all scenes are gone
    for (auto id : sceneIds) {
        EXPECT_THROW(manager->getScene(id), std::out_of_range);
    }
}

TEST_F(SceneManagerTest, AlternatingCreateAndDelete) {
    // Test alternating between creating and deleting scenes
    unsigned int sceneId1 = manager->createScene("Scene1");
    EXPECT_NO_THROW(manager->getScene(sceneId1));

    manager->deleteScene(sceneId1);
    EXPECT_THROW(manager->getScene(sceneId1), std::out_of_range);

    unsigned int sceneId2 = manager->createScene("Scene2");
    EXPECT_NO_THROW(manager->getScene(sceneId2));

    unsigned int sceneId3 = manager->createScene("Scene3");
    EXPECT_NO_THROW(manager->getScene(sceneId3));

    manager->deleteScene(sceneId2);
    EXPECT_THROW(manager->getScene(sceneId2), std::out_of_range);
    EXPECT_NO_THROW(manager->getScene(sceneId3));

    unsigned int sceneId4 = manager->createScene("Scene4");
    EXPECT_NO_THROW(manager->getScene(sceneId4));

    manager->deleteScene(sceneId3);
    manager->deleteScene(sceneId4);
    EXPECT_THROW(manager->getScene(sceneId3), std::out_of_range);
    EXPECT_THROW(manager->getScene(sceneId4), std::out_of_range);
}

TEST_F(SceneManagerTest, SceneIdBoundaryConditions) {
    // Test scene ID behavior at boundary conditions
    nexo::scene::nextSceneId = 0;
    manager.reset(new SceneManager());
    manager->setCoordinator(coordinator);

    // Create scene with ID 0
    unsigned int sceneId0 = manager->createScene("Scene0");
    EXPECT_EQ(sceneId0, 0);

    // Create many more scenes to test incrementing IDs
    for (unsigned int i = 1; i < 100; i++) {
        unsigned int sceneId = manager->createScene("Scene" + std::to_string(i));
        EXPECT_EQ(sceneId, i);
    }
}

TEST_F(SceneManagerTest, GetSceneWithMaxIntId) {
    // Test getting a scene with maximum integer ID
    unsigned int maxId = std::numeric_limits<unsigned int>::max();
    EXPECT_THROW(manager->getScene(maxId), std::out_of_range);
}

TEST_F(SceneManagerTest, DeleteSceneWithMaxIntId) {
    // Test deleting a scene with maximum integer ID
    unsigned int maxId = std::numeric_limits<unsigned int>::max();
    EXPECT_NO_THROW(manager->deleteScene(maxId));
}

TEST_F(SceneManagerTest, GetSceneZeroId) {
    // Test getting scene with ID 0 when no scenes exist
    EXPECT_THROW(manager->getScene(0), std::out_of_range);

    // Create a scene with ID 0
    unsigned int sceneId = manager->createScene("Scene0");
    EXPECT_EQ(sceneId, 0);
    EXPECT_NO_THROW(manager->getScene(0));

    // Delete it
    manager->deleteScene(0);
    EXPECT_THROW(manager->getScene(0), std::out_of_range);
}

TEST_F(SceneManagerTest, MultipleCreationDestructionCycles) {
    // Test multiple cycles of creating and destroying SceneManager
    for (int cycle = 0; cycle < 5; cycle++) {
        // Create new manager
        manager.reset(new SceneManager());
        manager->setCoordinator(coordinator);

        // Create some scenes
        std::vector<unsigned int> sceneIds;
        for (int i = 0; i < 10; i++) {
            sceneIds.push_back(manager->createScene("Cycle" + std::to_string(cycle) + "_Scene" + std::to_string(i)));
        }

        // Verify all scenes exist
        for (auto id : sceneIds) {
            EXPECT_NO_THROW(manager->getScene(id));
        }

        // Delete half the scenes
        for (size_t i = 0; i < sceneIds.size() / 2; i++) {
            manager->deleteScene(sceneIds[i]);
        }

        // Manager will be destroyed at the end of this iteration
    }
}

TEST_F(SceneManagerTest, CreateSceneWithEmptyName) {
    // Test creating a scene with an empty name
    unsigned int sceneId = manager->createScene("");
    Scene& scene = manager->getScene(sceneId);
    EXPECT_EQ(scene.getName(), "");
}

TEST_F(SceneManagerTest, CreateSceneWithVeryLongName) {
    // Test creating a scene with a very long name
    std::string longName(10000, 'X');
    unsigned int sceneId = manager->createScene(longName);
    Scene& scene = manager->getScene(sceneId);
    EXPECT_EQ(scene.getName(), longName);
}

TEST_F(SceneManagerTest, CreateMultipleScenesWithSameName) {
    // Test creating multiple scenes with the same name (should be allowed)
    unsigned int sceneId1 = manager->createScene("SameName");
    unsigned int sceneId2 = manager->createScene("SameName");
    unsigned int sceneId3 = manager->createScene("SameName");

    // All scenes should exist with different IDs
    EXPECT_NE(sceneId1, sceneId2);
    EXPECT_NE(sceneId2, sceneId3);
    EXPECT_NE(sceneId1, sceneId3);

    // All scenes should have the same name
    EXPECT_EQ(manager->getScene(sceneId1).getName(), "SameName");
    EXPECT_EQ(manager->getScene(sceneId2).getName(), "SameName");
    EXPECT_EQ(manager->getScene(sceneId3).getName(), "SameName");
}

TEST_F(SceneManagerTest, ModifySceneAfterRetrieval) {
    // Test modifying a scene after retrieving it from the manager
    unsigned int sceneId = manager->createScene("OriginalScene");

    Scene& scene1 = manager->getScene(sceneId);
    scene1.setName("ModifiedScene");
    scene1.setActiveStatus(false);
    scene1.setRenderStatus(false);

    // Get the scene again and verify modifications persisted
    Scene& scene2 = manager->getScene(sceneId);
    EXPECT_EQ(scene2.getName(), "ModifiedScene");
    EXPECT_FALSE(scene2.isActive());
    EXPECT_FALSE(scene2.isRendered());
}

TEST_F(SceneManagerTest, StressTestSceneCreation) {
    // Stress test with creating many scenes
    const size_t sceneCount = 10000;
    std::vector<unsigned int> sceneIds;

    for (size_t i = 0; i < sceneCount; i++) {
        sceneIds.push_back(manager->createScene("StressScene" + std::to_string(i)));
    }

    // Verify all scenes exist
    EXPECT_EQ(sceneIds.size(), sceneCount);

    // Sample verify some scenes
    for (size_t i = 0; i < sceneCount; i += 1000) {
        EXPECT_NO_THROW(manager->getScene(sceneIds[i]));
    }

    // Delete all scenes
    for (auto id : sceneIds) {
        manager->deleteScene(id);
    }
}

TEST_F(SceneManagerTest, DeleteAllScenesInReverseOrder) {
    // Test deleting scenes in reverse order of creation
    std::vector<unsigned int> sceneIds;

    for (int i = 0; i < 20; i++) {
        sceneIds.push_back(manager->createScene("Scene" + std::to_string(i)));
    }

    // Delete in reverse order
    for (auto it = sceneIds.rbegin(); it != sceneIds.rend(); ++it) {
        unsigned int id = *it;
        EXPECT_NO_THROW(manager->getScene(id));
        manager->deleteScene(id);
        EXPECT_THROW(manager->getScene(id), std::out_of_range);
    }
}

TEST_F(SceneManagerTest, DeleteEveryOtherScene) {
    // Test deleting every other scene
    std::vector<unsigned int> sceneIds;

    for (int i = 0; i < 20; i++) {
        sceneIds.push_back(manager->createScene("Scene" + std::to_string(i)));
    }

    // Delete every other scene
    for (size_t i = 0; i < sceneIds.size(); i += 2) {
        manager->deleteScene(sceneIds[i]);
    }

    // Verify deleted scenes are gone and others remain
    for (size_t i = 0; i < sceneIds.size(); i++) {
        if (i % 2 == 0) {
            EXPECT_THROW(manager->getScene(sceneIds[i]), std::out_of_range);
        } else {
            EXPECT_NO_THROW(manager->getScene(sceneIds[i]));
        }
    }
}

TEST_F(SceneManagerTest, CreateEditorAndRegularScenesInterleaved) {
    // Test creating editor and regular scenes in an interleaved pattern
    std::vector<unsigned int> sceneIds;

    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            sceneIds.push_back(manager->createScene("RegularScene" + std::to_string(i)));
        } else {
            sceneIds.push_back(manager->createEditorScene("EditorScene" + std::to_string(i)));
        }
    }

    // Verify all scenes exist
    for (auto id : sceneIds) {
        EXPECT_NO_THROW(manager->getScene(id));
    }

    // IDs should be sequential
    for (size_t i = 0; i < sceneIds.size(); i++) {
        EXPECT_EQ(sceneIds[i], i);
    }
}

TEST_F(SceneManagerTest, SceneManagerResetAfterMultipleScenes) {
    // Test resetting the SceneManager after creating multiple scenes
    std::vector<unsigned int> sceneIds;

    for (int i = 0; i < 10; i++) {
        sceneIds.push_back(manager->createScene("Scene" + std::to_string(i)));
    }

    // Reset the manager
    manager.reset(new SceneManager());
    manager->setCoordinator(coordinator);

    // Old scene IDs should not be accessible
    for (auto id : sceneIds) {
        EXPECT_THROW(manager->getScene(id), std::out_of_range);
    }

    // Should be able to create new scenes
    unsigned int newSceneId = manager->createScene("NewScene");
    EXPECT_NO_THROW(manager->getScene(newSceneId));
}

TEST_F(SceneManagerTest, GetSceneReturnsReference) {
    // Test that getScene returns a reference that can be modified
    unsigned int sceneId = manager->createScene("TestScene");

    Scene& sceneRef1 = manager->getScene(sceneId);
    Scene& sceneRef2 = manager->getScene(sceneId);

    // Modify through first reference
    sceneRef1.setName("ModifiedName");

    // Should see change through second reference
    EXPECT_EQ(sceneRef2.getName(), "ModifiedName");
}

} // namespace nexo::scene
