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
//  Author:      Claude Code
//  Date:        13/12/2025
//  Description: Test file for Scene Serialization
//
//  NOTE: This test file is designed for FUTURE scene serialization functionality.
//  Scene serialization is not yet implemented in the NEXO engine. These tests
//  demonstrate the expected behavior and API design for when serialization is added.
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/scene/Scene.hpp"
#include "core/scene/SceneManager.hpp"
#include "components/SceneComponents.hpp"
#include "components/Transform.hpp"
#include "components/Name.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "components/Uuid.hpp"
#include "ecs/Coordinator.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

namespace nexo::scene {

// ============================================================================
// MOCK SERIALIZATION FUNCTIONS
// ============================================================================
// These are placeholder implementations to demonstrate what the actual
// serialization API should look like. When scene serialization is implemented,
// replace these with actual serializer class methods.
// ============================================================================

namespace mock {

    // Mock function to serialize a scene to JSON (basic version)
    json serializeScene(const Scene& scene, const std::shared_ptr<ecs::Coordinator>& coordinator) {
        json sceneJson;
        sceneJson["name"] = scene.getName();
        sceneJson["uuid"] = scene.getUuid();
        sceneJson["id"] = scene.getId();
        sceneJson["active"] = scene.isActive();
        sceneJson["rendered"] = scene.isRendered();
        sceneJson["entities"] = json::array();

        // Serialize all entities in the scene
        for (const auto& entity : scene.getEntities()) {
            json entityJson;
            entityJson["id"] = entity;

            // Serialize NameComponent if present
            if (auto name = coordinator->tryGetComponent<components::NameComponent>(entity)) {
                entityJson["components"]["Name"] = name->get().name;
            }

            // Serialize Transform component if present (with correct field names)
            if (auto transform = coordinator->tryGetComponent<components::TransformComponent>(entity)) {
                const auto& t = transform->get();
                json transformJson;
                transformJson["position"] = {t.pos.x, t.pos.y, t.pos.z};
                transformJson["scale"] = {t.size.x, t.size.y, t.size.z};
                transformJson["rotation"] = {t.quat.x, t.quat.y, t.quat.z, t.quat.w};
                transformJson["children"] = t.children;
                entityJson["components"]["Transform"] = transformJson;
            }

            // Serialize Camera component if present (with correct field names)
            if (auto camera = coordinator->tryGetComponent<components::CameraComponent>(entity)) {
                const auto& c = camera->get();
                json cameraJson;
                cameraJson["fov"] = c.fov;
                cameraJson["nearPlane"] = c.nearPlane;
                cameraJson["farPlane"] = c.farPlane;
                cameraJson["main"] = c.main;
                entityJson["components"]["Camera"] = cameraJson;
            }

            // Serialize DirectionalLightComponent if present
            if (auto light = coordinator->tryGetComponent<components::DirectionalLightComponent>(entity)) {
                const auto& l = light->get();
                json lightJson;
                lightJson["direction"] = {l.direction.x, l.direction.y, l.direction.z};
                lightJson["color"] = {l.color.x, l.color.y, l.color.z};
                entityJson["components"]["DirectionalLight"] = lightJson;
            }

            // Serialize PointLightComponent if present
            if (auto light = coordinator->tryGetComponent<components::PointLightComponent>(entity)) {
                const auto& l = light->get();
                json lightJson;
                lightJson["color"] = {l.color.x, l.color.y, l.color.z};
                lightJson["constant"] = l.constant;
                lightJson["linear"] = l.linear;
                lightJson["quadratic"] = l.quadratic;
                entityJson["components"]["PointLight"] = lightJson;
            }

            sceneJson["entities"].push_back(entityJson);
        }

        return sceneJson;
    }

    // Mock function to deserialize a scene from JSON (basic version)
    void deserializeScene(Scene& scene, const json& sceneJson,
                         const std::shared_ptr<ecs::Coordinator>& coordinator) {
        // Set scene properties
        if (sceneJson.contains("name")) {
            scene.setName(sceneJson["name"].get<std::string>());
        }
        if (sceneJson.contains("active")) {
            scene.setActiveStatus(sceneJson["active"].get<bool>());
        }
        if (sceneJson.contains("rendered")) {
            scene.setRenderStatus(sceneJson["rendered"].get<bool>());
        }

        // Deserialize entities
        if (sceneJson.contains("entities") && sceneJson["entities"].is_array()) {
            for (const auto& entityJson : sceneJson["entities"]) {
                ecs::Entity entity = coordinator->createEntity();

                // Deserialize NameComponent
                if (entityJson.contains("components") && entityJson["components"].contains("Name")) {
                    components::NameComponent name;
                    name.name = entityJson["components"]["Name"].get<std::string>();
                    coordinator->addComponent(entity, name);
                }

                // Deserialize Transform component (with correct field names)
                if (entityJson.contains("components") && entityJson["components"].contains("Transform")) {
                    const auto& tJson = entityJson["components"]["Transform"];
                    components::TransformComponent transform;

                    if (tJson.contains("position")) {
                        auto pos = tJson["position"];
                        transform.pos = {pos[0], pos[1], pos[2]};
                    }
                    if (tJson.contains("rotation")) {
                        auto rot = tJson["rotation"];
                        transform.quat = {rot[0], rot[1], rot[2], rot[3]};
                    }
                    if (tJson.contains("scale")) {
                        auto scl = tJson["scale"];
                        transform.size = {scl[0], scl[1], scl[2]};
                    }
                    if (tJson.contains("children")) {
                        transform.children = tJson["children"].get<std::vector<ecs::Entity>>();
                    }

                    coordinator->addComponent(entity, transform);
                }

                // Deserialize Camera component (with correct field names)
                if (entityJson.contains("components") && entityJson["components"].contains("Camera")) {
                    const auto& cJson = entityJson["components"]["Camera"];
                    // Create with default width/height since they're required
                    components::CameraComponent camera;
                    camera.width = 800;
                    camera.height = 600;

                    if (cJson.contains("fov")) camera.fov = cJson["fov"];
                    if (cJson.contains("nearPlane")) camera.nearPlane = cJson["nearPlane"];
                    if (cJson.contains("farPlane")) camera.farPlane = cJson["farPlane"];
                    if (cJson.contains("main")) camera.main = cJson["main"];

                    coordinator->addComponent(entity, camera);
                }

                // Deserialize DirectionalLightComponent
                if (entityJson.contains("components") && entityJson["components"].contains("DirectionalLight")) {
                    const auto& lJson = entityJson["components"]["DirectionalLight"];
                    components::DirectionalLightComponent light;

                    if (lJson.contains("direction")) {
                        auto dir = lJson["direction"];
                        light.direction = {dir[0], dir[1], dir[2]};
                    }
                    if (lJson.contains("color")) {
                        auto col = lJson["color"];
                        light.color = {col[0], col[1], col[2]};
                    }

                    coordinator->addComponent(entity, light);
                }

                // Deserialize PointLightComponent
                if (entityJson.contains("components") && entityJson["components"].contains("PointLight")) {
                    const auto& lJson = entityJson["components"]["PointLight"];
                    components::PointLightComponent light;

                    if (lJson.contains("color")) {
                        auto col = lJson["color"];
                        light.color = {col[0], col[1], col[2]};
                    }
                    if (lJson.contains("constant")) light.constant = lJson["constant"];
                    if (lJson.contains("linear")) light.linear = lJson["linear"];
                    if (lJson.contains("quadratic")) light.quadratic = lJson["quadratic"];

                    coordinator->addComponent(entity, light);
                }

                // Add entity to scene
                scene.addEntity(entity);
            }
        }
    }

    // Mock function to save scene to file
    bool saveSceneToFile(const Scene& scene, const std::shared_ptr<ecs::Coordinator>& coordinator,
                        const std::string& filepath) {
        try {
            json sceneJson = serializeScene(scene, coordinator);
            std::ofstream file(filepath);
            if (!file.is_open()) {
                return false;
            }
            file << sceneJson.dump(4);  // Pretty print with 4 space indent
            file.close();
            return true;
        } catch (...) {
            return false;
        }
    }

    // Mock function to load scene from file
    bool loadSceneFromFile(Scene& scene, const std::shared_ptr<ecs::Coordinator>& coordinator,
                          const std::string& filepath) {
        try {
            std::ifstream file(filepath);
            if (!file.is_open()) {
                return false;
            }
            json sceneJson;
            file >> sceneJson;
            file.close();

            deserializeScene(scene, sceneJson, coordinator);
            return true;
        } catch (...) {
            return false;
        }
    }

} // namespace mock

// ============================================================================
// TEST FIXTURE
// ============================================================================

class SceneSerializerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset scene ID counter for consistent test behavior
        nextSceneId = 0;

        // Create coordinator and register all components
        coordinator = std::make_shared<ecs::Coordinator>();
        coordinator->init();

        // Register all components needed for serialization tests
        coordinator->registerComponent<components::SceneTag>();
        coordinator->registerComponent<components::TransformComponent>();
        coordinator->registerComponent<components::NameComponent>();
        coordinator->registerComponent<components::CameraComponent>();
        coordinator->registerComponent<components::DirectionalLightComponent>();
        coordinator->registerComponent<components::PointLightComponent>();
    }

    void TearDown() override {
        // Clean up any test files created
        std::remove(testFilePath.c_str());
    }

    std::shared_ptr<ecs::Coordinator> coordinator;
    const std::string testFilePath = (std::filesystem::temp_directory_path() / "nexo_test_scene.json").string();
};

// ============================================================================
// SERIALIZATION TESTS
// ============================================================================

TEST_F(SceneSerializerTest, SerializeEmptyScene) {
    // Test serializing an empty scene with no entities
    Scene scene("EmptyScene", coordinator);

    json sceneJson = mock::serializeScene(scene, coordinator);

    EXPECT_EQ(sceneJson["name"], "EmptyScene");
    EXPECT_EQ(sceneJson["id"], 0);
    EXPECT_EQ(sceneJson["active"], true);
    EXPECT_EQ(sceneJson["rendered"], true);
    EXPECT_FALSE(sceneJson["uuid"].get<std::string>().empty());
    EXPECT_TRUE(sceneJson["entities"].is_array());
    EXPECT_EQ(sceneJson["entities"].size(), 0);
}

TEST_F(SceneSerializerTest, SerializeSceneWithSingleEntity) {
    // Test serializing a scene with a single entity
    Scene scene("SingleEntityScene", coordinator);
    ecs::Entity entity = coordinator->createEntity();

    // Add a name component
    components::NameComponent name;
    name.name = "TestEntity";
    coordinator->addComponent(entity, name);

    scene.addEntity(entity);

    json sceneJson = mock::serializeScene(scene, coordinator);

    EXPECT_EQ(sceneJson["entities"].size(), 1);
    EXPECT_EQ(sceneJson["entities"][0]["components"]["Name"], "TestEntity");
}

TEST_F(SceneSerializerTest, SerializeSceneWithMultipleEntities) {
    // Test serializing a scene with multiple entities
    Scene scene("MultiEntityScene", coordinator);

    for (int i = 0; i < 5; i++) {
        ecs::Entity entity = coordinator->createEntity();
        components::NameComponent name;
        name.name = "Entity_" + std::to_string(i);
        coordinator->addComponent(entity, name);
        scene.addEntity(entity);
    }

    json sceneJson = mock::serializeScene(scene, coordinator);

    EXPECT_EQ(sceneJson["entities"].size(), 5);
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(sceneJson["entities"][i]["components"]["Name"],
                 "Entity_" + std::to_string(i));
    }
}

TEST_F(SceneSerializerTest, SerializeSceneWithTransformComponent) {
    // Test serializing transform component data
    Scene scene("TransformScene", coordinator);
    ecs::Entity entity = coordinator->createEntity();

    components::TransformComponent transform;
    transform.pos = {1.0f, 2.0f, 3.0f};
    transform.quat = glm::quat(0.707f, 0.0f, 0.707f, 0.0f);
    transform.size = {2.0f, 2.0f, 2.0f};
    coordinator->addComponent(entity, transform);

    scene.addEntity(entity);

    json sceneJson = mock::serializeScene(scene, coordinator);

    EXPECT_EQ(sceneJson["entities"].size(), 1);
    auto& transformJson = sceneJson["entities"][0]["components"]["Transform"];

    EXPECT_FLOAT_EQ(transformJson["position"][0], 1.0f);
    EXPECT_FLOAT_EQ(transformJson["position"][1], 2.0f);
    EXPECT_FLOAT_EQ(transformJson["position"][2], 3.0f);

    EXPECT_FLOAT_EQ(transformJson["scale"][0], 2.0f);
    EXPECT_FLOAT_EQ(transformJson["scale"][1], 2.0f);
    EXPECT_FLOAT_EQ(transformJson["scale"][2], 2.0f);
}

TEST_F(SceneSerializerTest, SerializeSceneWithCameraComponent) {
    // Test serializing camera component data
    Scene scene("CameraScene", coordinator);
    ecs::Entity entity = coordinator->createEntity();

    components::CameraComponent camera;
    camera.width = 1920;
    camera.height = 1080;
    camera.fov = 60.0f;
    camera.nearPlane = 0.1f;
    camera.farPlane = 1000.0f;
    camera.main = true;
    coordinator->addComponent(entity, camera);

    scene.addEntity(entity);

    json sceneJson = mock::serializeScene(scene, coordinator);

    auto& cameraJson = sceneJson["entities"][0]["components"]["Camera"];
    EXPECT_FLOAT_EQ(cameraJson["fov"], 60.0f);
    EXPECT_FLOAT_EQ(cameraJson["nearPlane"], 0.1f);
    EXPECT_FLOAT_EQ(cameraJson["farPlane"], 1000.0f);
    EXPECT_TRUE(cameraJson["main"]);
}

TEST_F(SceneSerializerTest, SerializeSceneWithDirectionalLight) {
    // Test serializing directional light component
    Scene scene("LightScene", coordinator);
    ecs::Entity entity = coordinator->createEntity();

    components::DirectionalLightComponent light;
    light.direction = {0.0f, -1.0f, 0.0f};
    light.color = {1.0f, 1.0f, 1.0f};
    coordinator->addComponent(entity, light);

    scene.addEntity(entity);

    json sceneJson = mock::serializeScene(scene, coordinator);

    auto& lightJson = sceneJson["entities"][0]["components"]["DirectionalLight"];
    EXPECT_FLOAT_EQ(lightJson["direction"][0], 0.0f);
    EXPECT_FLOAT_EQ(lightJson["direction"][1], -1.0f);
    EXPECT_FLOAT_EQ(lightJson["direction"][2], 0.0f);
}

TEST_F(SceneSerializerTest, SerializeSceneWithPointLight) {
    // Test serializing point light component
    Scene scene("PointLightScene", coordinator);
    ecs::Entity entity = coordinator->createEntity();

    components::PointLightComponent light;
    light.color = {1.0f, 0.8f, 0.6f};
    light.constant = 1.0f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    coordinator->addComponent(entity, light);

    scene.addEntity(entity);

    json sceneJson = mock::serializeScene(scene, coordinator);

    auto& lightJson = sceneJson["entities"][0]["components"]["PointLight"];
    EXPECT_FLOAT_EQ(lightJson["constant"], 1.0f);
    EXPECT_FLOAT_EQ(lightJson["linear"], 0.09f);
    EXPECT_FLOAT_EQ(lightJson["quadratic"], 0.032f);
}

TEST_F(SceneSerializerTest, SerializeSceneWithMultipleComponents) {
    // Test serializing an entity with multiple components
    Scene scene("MultiComponentScene", coordinator);
    ecs::Entity entity = coordinator->createEntity();

    components::NameComponent name;
    name.name = "ComplexEntity";
    coordinator->addComponent(entity, name);

    components::TransformComponent transform;
    transform.pos = {1.0f, 2.0f, 3.0f};
    coordinator->addComponent(entity, transform);

    components::CameraComponent camera;
    camera.width = 800;
    camera.height = 600;
    camera.fov = 75.0f;
    coordinator->addComponent(entity, camera);

    scene.addEntity(entity);

    json sceneJson = mock::serializeScene(scene, coordinator);

    EXPECT_EQ(sceneJson["entities"][0]["components"]["Name"], "ComplexEntity");
    EXPECT_TRUE(sceneJson["entities"][0]["components"].contains("Transform"));
    EXPECT_TRUE(sceneJson["entities"][0]["components"].contains("Camera"));
}

TEST_F(SceneSerializerTest, SerializeInactiveScene) {
    // Test serializing a scene that is not active
    Scene scene("InactiveScene", coordinator);
    scene.setActiveStatus(false);
    scene.setRenderStatus(false);

    json sceneJson = mock::serializeScene(scene, coordinator);

    EXPECT_FALSE(sceneJson["active"]);
    EXPECT_FALSE(sceneJson["rendered"]);
}

// ============================================================================
// DESERIALIZATION TESTS
// ============================================================================

TEST_F(SceneSerializerTest, DeserializeEmptyScene) {
    // Test deserializing an empty scene
    Scene scene("TestScene", coordinator);

    json sceneJson = {
        {"name", "DeserializedEmpty"},
        {"uuid", "test-uuid-123"},
        {"id", 5},
        {"active", true},
        {"rendered", true},
        {"entities", json::array()}
    };

    mock::deserializeScene(scene, sceneJson, coordinator);

    EXPECT_EQ(scene.getName(), "DeserializedEmpty");
    EXPECT_TRUE(scene.isActive());
    EXPECT_TRUE(scene.isRendered());
    EXPECT_EQ(scene.getEntities().size(), 0);
}

TEST_F(SceneSerializerTest, DeserializeSceneWithSingleEntity) {
    // Test deserializing a scene with one entity
    Scene scene("TestScene", coordinator);

    json sceneJson = {
        {"name", "SingleEntity"},
        {"entities", json::array({
            {
                {"id", 0},
                {"components", {
                    {"Name", "LoadedEntity"}
                }}
            }
        })}
    };

    mock::deserializeScene(scene, sceneJson, coordinator);

    EXPECT_EQ(scene.getEntities().size(), 1);

    auto entity = *scene.getEntities().begin();
    auto name = coordinator->tryGetComponent<components::NameComponent>(entity);
    EXPECT_TRUE(name.has_value());
    EXPECT_EQ(name->get().name, "LoadedEntity");
}

TEST_F(SceneSerializerTest, DeserializeSceneWithMultipleEntities) {
    // Test deserializing a scene with multiple entities
    Scene scene("TestScene", coordinator);

    json sceneJson = {
        {"name", "MultiEntity"},
        {"entities", json::array({
            {{"components", {{"Name", "Entity1"}}}},
            {{"components", {{"Name", "Entity2"}}}},
            {{"components", {{"Name", "Entity3"}}}}
        })}
    };

    mock::deserializeScene(scene, sceneJson, coordinator);

    EXPECT_EQ(scene.getEntities().size(), 3);
}

TEST_F(SceneSerializerTest, DeserializeTransformComponent) {
    // Test deserializing transform component data
    Scene scene("TestScene", coordinator);

    json sceneJson = {
        {"name", "TransformTest"},
        {"entities", json::array({
            {
                {"components", {
                    {"Transform", {
                        {"position", {1.5f, 2.5f, 3.5f}},
                        {"rotation", {0.707f, 0.0f, 0.707f, 0.0f}},
                        {"scale", {0.5f, 1.0f, 1.5f}},
                        {"children", json::array()}
                    }}
                }}
            }
        })}
    };

    mock::deserializeScene(scene, sceneJson, coordinator);

    auto entity = *scene.getEntities().begin();
    auto transform = coordinator->tryGetComponent<components::TransformComponent>(entity);

    EXPECT_TRUE(transform.has_value());
    EXPECT_FLOAT_EQ(transform->get().pos.x, 1.5f);
    EXPECT_FLOAT_EQ(transform->get().pos.y, 2.5f);
    EXPECT_FLOAT_EQ(transform->get().pos.z, 3.5f);
    EXPECT_FLOAT_EQ(transform->get().size.z, 1.5f);
}

TEST_F(SceneSerializerTest, DeserializeInactiveScene) {
    // Test deserializing a scene with inactive state
    Scene scene("TestScene", coordinator);

    json sceneJson = {
        {"name", "InactiveTest"},
        {"active", false},
        {"rendered", false},
        {"entities", json::array()}
    };

    mock::deserializeScene(scene, sceneJson, coordinator);

    EXPECT_FALSE(scene.isActive());
    EXPECT_FALSE(scene.isRendered());
}

// ============================================================================
// ROUND-TRIP TESTS
// ============================================================================

TEST_F(SceneSerializerTest, RoundTripEmptyScene) {
    // Test that an empty scene survives serialization and deserialization
    Scene originalScene("RoundTripEmpty", coordinator);
    originalScene.setActiveStatus(false);

    json sceneJson = mock::serializeScene(originalScene, coordinator);

    Scene loadedScene("Temp", coordinator);
    mock::deserializeScene(loadedScene, sceneJson, coordinator);

    EXPECT_EQ(loadedScene.getName(), originalScene.getName());
    EXPECT_EQ(loadedScene.isActive(), originalScene.isActive());
    EXPECT_EQ(loadedScene.getEntities().size(), originalScene.getEntities().size());
}

TEST_F(SceneSerializerTest, RoundTripSceneWithEntities) {
    // Test round-trip with entities and components
    Scene originalScene("RoundTripEntities", coordinator);

    // Create entity with multiple components
    ecs::Entity entity = coordinator->createEntity();

    components::NameComponent name;
    name.name = "TestEntity";
    coordinator->addComponent(entity, name);

    components::TransformComponent transform;
    transform.pos = {1.0f, 2.0f, 3.0f};
    transform.size = {1.5f, 1.5f, 1.5f};
    coordinator->addComponent(entity, transform);

    originalScene.addEntity(entity);

    // Serialize and deserialize
    json sceneJson = mock::serializeScene(originalScene, coordinator);

    Scene loadedScene("Temp", coordinator);
    mock::deserializeScene(loadedScene, sceneJson, coordinator);

    // Verify scene properties
    EXPECT_EQ(loadedScene.getName(), "RoundTripEntities");
    EXPECT_EQ(loadedScene.getEntities().size(), 1);

    // Verify entity components
    auto loadedEntity = *loadedScene.getEntities().begin();
    auto loadedName = coordinator->tryGetComponent<components::NameComponent>(loadedEntity);
    auto loadedTransform = coordinator->tryGetComponent<components::TransformComponent>(loadedEntity);

    EXPECT_TRUE(loadedName.has_value());
    EXPECT_EQ(loadedName->get().name, "TestEntity");

    EXPECT_TRUE(loadedTransform.has_value());
    EXPECT_FLOAT_EQ(loadedTransform->get().pos.x, 1.0f);
    EXPECT_FLOAT_EQ(loadedTransform->get().size.x, 1.5f);
}

// ============================================================================
// FILE I/O TESTS
// ============================================================================

TEST_F(SceneSerializerTest, SaveSceneToFile) {
    // Test saving a scene to a file
    Scene scene("FileSaveTest", coordinator);

    ecs::Entity entity = coordinator->createEntity();
    components::NameComponent name;
    name.name = "SavedEntity";
    coordinator->addComponent(entity, name);
    scene.addEntity(entity);

    bool success = mock::saveSceneToFile(scene, coordinator, testFilePath);

    EXPECT_TRUE(success);

    // Verify file exists
    std::ifstream file(testFilePath);
    EXPECT_TRUE(file.good());
    file.close();
}

TEST_F(SceneSerializerTest, LoadSceneFromFile) {
    // Test loading a scene from a file
    Scene originalScene("FileLoadTest", coordinator);

    ecs::Entity entity = coordinator->createEntity();
    components::NameComponent name;
    name.name = "LoadedEntity";
    coordinator->addComponent(entity, name);
    originalScene.addEntity(entity);

    // Save to file
    mock::saveSceneToFile(originalScene, coordinator, testFilePath);

    // Load from file
    Scene loadedScene("Temp", coordinator);
    bool success = mock::loadSceneFromFile(loadedScene, coordinator, testFilePath);

    EXPECT_TRUE(success);
    EXPECT_EQ(loadedScene.getName(), "FileLoadTest");
    EXPECT_EQ(loadedScene.getEntities().size(), 1);
}

TEST_F(SceneSerializerTest, LoadSceneFromNonExistentFile) {
    // Test loading from a file that doesn't exist
    Scene scene("Test", coordinator);

    bool success = mock::loadSceneFromFile(scene, coordinator, "/nonexistent/path/scene.json");

    EXPECT_FALSE(success);
}

TEST_F(SceneSerializerTest, SaveSceneToInvalidPath) {
    // Test saving to an invalid path
    Scene scene("InvalidPathTest", coordinator);

    bool success = mock::saveSceneToFile(scene, coordinator, "/invalid/path/that/does/not/exist/scene.json");

    EXPECT_FALSE(success);
}

TEST_F(SceneSerializerTest, RoundTripThroughFile) {
    // Test complete save and load cycle
    Scene originalScene("FileRoundTrip", coordinator);

    for (int i = 0; i < 3; i++) {
        ecs::Entity entity = coordinator->createEntity();
        components::NameComponent name;
        name.name = "Entity_" + std::to_string(i);
        coordinator->addComponent(entity, name);

        components::TransformComponent transform;
        transform.pos = {static_cast<float>(i), static_cast<float>(i * 2), static_cast<float>(i * 3)};
        coordinator->addComponent(entity, transform);

        originalScene.addEntity(entity);
    }

    // Save
    bool saveSuccess = mock::saveSceneToFile(originalScene, coordinator, testFilePath);
    EXPECT_TRUE(saveSuccess);

    // Load
    Scene loadedScene("Temp", coordinator);
    bool loadSuccess = mock::loadSceneFromFile(loadedScene, coordinator, testFilePath);
    EXPECT_TRUE(loadSuccess);

    // Verify
    EXPECT_EQ(loadedScene.getName(), "FileRoundTrip");
    EXPECT_EQ(loadedScene.getEntities().size(), 3);
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST_F(SceneSerializerTest, SerializeVeryLargeScene) {
    // Test serializing a scene with many entities
    Scene scene("LargeScene", coordinator);

    const int entityCount = 100;  // Reduced from 1000 to avoid slow tests
    for (int i = 0; i < entityCount; i++) {
        ecs::Entity entity = coordinator->createEntity();
        components::NameComponent name;
        name.name = "Entity_" + std::to_string(i);
        coordinator->addComponent(entity, name);
        scene.addEntity(entity);
    }

    json sceneJson = mock::serializeScene(scene, coordinator);

    EXPECT_EQ(sceneJson["entities"].size(), entityCount);
}

TEST_F(SceneSerializerTest, DeserializeInvalidJSON) {
    // Test deserializing from invalid JSON
    Scene scene("Test", coordinator);

    json invalidJson = {
        {"name", "Invalid"},
        {"entities", "not_an_array"}  // Wrong type
    };

    // Should not crash, but may not load entities
    EXPECT_NO_THROW(mock::deserializeScene(scene, invalidJson, coordinator));
}

TEST_F(SceneSerializerTest, DeserializeMissingFields) {
    // Test deserializing JSON with missing optional fields
    Scene scene("Test", coordinator);

    json minimalJson = {
        {"name", "Minimal"}
        // Missing entities, active, rendered, etc.
    };

    EXPECT_NO_THROW(mock::deserializeScene(scene, minimalJson, coordinator));
    EXPECT_EQ(scene.getName(), "Minimal");
}

TEST_F(SceneSerializerTest, SerializeSceneWithSpecialCharactersInName) {
    // Test serializing scene with special characters in entity names
    Scene scene("SpecialCharsScene", coordinator);

    ecs::Entity entity = coordinator->createEntity();
    components::NameComponent name;
    name.name = "Entity with spaces and special chars!";
    coordinator->addComponent(entity, name);
    scene.addEntity(entity);

    json sceneJson = mock::serializeScene(scene, coordinator);

    EXPECT_EQ(sceneJson["entities"][0]["components"]["Name"],
              "Entity with spaces and special chars!");
}

} // namespace nexo::scene
