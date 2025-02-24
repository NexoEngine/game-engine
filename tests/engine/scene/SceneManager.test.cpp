#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/scene/SceneManager.hpp"
#include "core/layer/Layer.hpp"
#include "components/Light.hpp"
#include "core/camera/Camera.hpp"

namespace nexo::scene {
    class MockCamera : public camera::Camera {
        public:
        MOCK_METHOD(void, onUpdate, (Timestep), (override));
        MOCK_METHOD(const glm::mat4&, getViewProjectionMatrix, (), (const, override));
        MOCK_METHOD(const glm::vec3&, getPosition, (), (const, override));
        MOCK_METHOD(camera::CameraMode, getMode, (), (const, override));
    };

    class MockLight : public components::Light {
        public:
        MockLight() : Light(components::LightType::DIRECTIONAL, {1.0f, 1.0f, 1.0f, 1.0f}) {}
    };

    class SceneManagerTest : public ::testing::Test {
        protected:
        void SetUp() override {
            Application::getInstance();
            nexo::init();
            sceneManager = std::make_unique<SceneManager>();
        }

        std::unique_ptr<SceneManager> sceneManager;
    };

    TEST_F(SceneManagerTest, CreateAndDeleteScenes) {
        SceneId scene1 = sceneManager->createScene("Scene1");
        SceneId scene2 = sceneManager->createScene("Scene2");

        EXPECT_EQ(sceneManager->getSceneName(scene1), "Scene1");
        EXPECT_EQ(sceneManager->getSceneName(scene2), "Scene2");

        sceneManager->deleteScene(scene1);
        EXPECT_EQ(sceneManager->getSceneName(scene1), ""); // Scene1 no longer exists
        EXPECT_EQ(sceneManager->getSceneName(scene2), "Scene2"); // Scene2 still exists
    }

    TEST_F(SceneManagerTest, AddAndRemoveLayersAndOverlays) {
#ifdef _WIN32
        // TODO: fix test (see #100)
        GTEST_SKIP() << "Test crashes on the CI on Windows, skipping for now.";
#endif
        SceneId scene = sceneManager->createScene("Main Scene");

        LayerId layer1 = sceneManager->addLayer(scene, "Layer1");
        LayerId overlay1 = sceneManager->addOverlay(scene, "Overlay1");

        EXPECT_EQ(sceneManager->getSceneLayers(scene).size(), 2); // Layer + Overlay

        sceneManager->removeLayer(scene, layer1);
        EXPECT_EQ(sceneManager->getSceneLayers(scene).size(), 1); // Only Overlay remains

        sceneManager->removeOverlay(scene, overlay1);
        EXPECT_EQ(sceneManager->getSceneLayers(scene).size(), 0); // All removed
    }

    TEST_F(SceneManagerTest, HandleLayerAndOverlayNames) {
        SceneId scene = sceneManager->createScene("Name Test Scene");

        LayerId layer = sceneManager->addLayer(scene, "Layer1");
        LayerId overlay = sceneManager->addOverlay(scene, "Overlay1");

        sceneManager->setLayerName(scene, layer, "RenamedLayer");
        sceneManager->setLayerName(scene, overlay, "RenamedOverlay");

        EXPECT_EQ(sceneManager->getSceneLayers(scene)[0]->name, "RenamedOverlay");
        EXPECT_EQ(sceneManager->getSceneLayers(scene)[1]->name, "RenamedLayer");
    }

    TEST_F(SceneManagerTest, AddAndRemoveEntities) {
        SceneId scene = sceneManager->createScene("Entity Scene");

        ecs::Entity entity1 = 1;
        ecs::Entity entity2 = 2;

        auto &app = getApp();
        app.m_coordinator->addComponent<components::TransformComponent>(entity2, components::TransformComponent{});
        app.m_coordinator->addComponent<components::RenderComponent>(entity2, components::RenderComponent{});

        sceneManager->addGlobalEntity(entity1, scene);
        EXPECT_TRUE(sceneManager->getSceneGlobalEntities(scene).contains(entity1));

        LayerId layer1 = sceneManager->addLayer(scene, "Layer1");
        sceneManager->addEntityToLayer(entity2, scene, layer1);
        EXPECT_TRUE(sceneManager->getLayerEntities(scene, layer1).contains(entity2));

        sceneManager->removeGlobalEntity(entity1, scene);
        EXPECT_FALSE(sceneManager->getSceneGlobalEntities(scene).contains(entity1));

        sceneManager->removeEntityFromLayer(entity2, scene, layer1);
        EXPECT_FALSE(sceneManager->getLayerEntities(scene, layer1).contains(entity2));
    }

    TEST_F(SceneManagerTest, GetAllRenderedEntities) {
        SceneId scene1 = sceneManager->createScene("Scene1");
        ecs::Entity entity1 = 1;
        ecs::Entity entity2 = 2;
        ecs::Entity entity3 = 3;
        ecs::Entity entity4 = 4;

        auto &app = getApp();
        app.m_coordinator->addComponent<components::TransformComponent>(entity1, components::TransformComponent{});
        app.m_coordinator->addComponent<components::RenderComponent>(entity1, components::RenderComponent{});
        app.m_coordinator->addComponent<components::TransformComponent>(entity2, components::TransformComponent{});
        app.m_coordinator->addComponent<components::RenderComponent>(entity2, components::RenderComponent{});
        app.m_coordinator->addComponent<components::TransformComponent>(entity3, components::TransformComponent{});
        app.m_coordinator->addComponent<components::RenderComponent>(entity3, components::RenderComponent{});
        app.m_coordinator->addComponent<components::TransformComponent>(entity4, components::TransformComponent{});
        app.m_coordinator->addComponent<components::RenderComponent>(entity4, components::RenderComponent{});

        LayerId layer1 = sceneManager->addLayer(scene1, "Layer1");
        sceneManager->addEntityToLayer(entity1, scene1, layer1);
        sceneManager->addEntityToLayer(entity2, scene1, layer1);
        LayerId layer2 = sceneManager->addLayer(scene1, "Layer2");
        sceneManager->addEntityToLayer(entity3, scene1, layer2);
        sceneManager->addEntityToLayer(entity4, scene1, layer2);

        sceneManager->setLayerRenderStatus(scene1, layer1, false);
        sceneManager->setLayerRenderStatus(scene1, layer2, false);
        auto renderedEntities = sceneManager->getAllSceneRenderedEntities(scene1);
        EXPECT_TRUE(renderedEntities.empty());

        sceneManager->setLayerRenderStatus(scene1, layer1, true);
        renderedEntities = sceneManager->getAllSceneRenderedEntities(scene1);
        EXPECT_EQ(renderedEntities.size(), 2);
        EXPECT_TRUE(std::find(renderedEntities.begin(), renderedEntities.end(), entity1) != renderedEntities.end());
        EXPECT_TRUE(std::find(renderedEntities.begin(), renderedEntities.end(), entity2) != renderedEntities.end());

        sceneManager->setLayerRenderStatus(scene1, layer1, false);
        sceneManager->setLayerRenderStatus(scene1, layer2, true);
        renderedEntities = sceneManager->getAllSceneRenderedEntities(scene1);
        EXPECT_EQ(renderedEntities.size(), 2);
        EXPECT_TRUE(std::find(renderedEntities.begin(), renderedEntities.end(), entity3) != renderedEntities.end());
        EXPECT_TRUE(std::find(renderedEntities.begin(), renderedEntities.end(), entity4) != renderedEntities.end());
    }

    TEST_F(SceneManagerTest, AddAndRemoveLights) {
        SceneId scene = sceneManager->createScene("Lighting Scene");

        auto light1 = std::make_shared<MockLight>();
        auto light2 = std::make_shared<MockLight>();

        unsigned int lightIndex1 = sceneManager->addLightToScene(scene, light1);
        unsigned int lightIndex2 = sceneManager->addLightToScene(scene, light2);

        EXPECT_EQ(lightIndex1, 0);
        EXPECT_EQ(lightIndex2, 1);

        sceneManager->removeLightFromScene(scene, lightIndex1);
        EXPECT_EQ(sceneManager->getSceneAmbientLightValue(scene), glm::vec3(0.2f)); // State consistency
    }

    TEST_F(SceneManagerTest, AttachAndDetachCameraToLayer) {
        SceneId scene = sceneManager->createScene("Camera Scene");
        auto mockCamera = std::make_shared<MockCamera>();

        LayerId layer = sceneManager->addLayer(scene, "Layer1");
        sceneManager->attachCameraToLayer(scene, mockCamera, layer);

        EXPECT_EQ(sceneManager->getCameraLayer(scene, layer), mockCamera);

        sceneManager->detachCameraFromLayer(scene, layer);
        EXPECT_EQ(sceneManager->getCameraLayer(scene, layer), nullptr);
    }

    TEST_F(SceneManagerTest, SceneAndLayerStatuses) {
        SceneId scene = sceneManager->createScene("Status Scene");

        LayerId layer = sceneManager->addLayer(scene, "Layer1");
        sceneManager->setSceneRenderStatus(scene, false);
        EXPECT_FALSE(sceneManager->isSceneRendered(scene));

        sceneManager->setLayerRenderStatus(scene, layer, false);
        EXPECT_FALSE(sceneManager->isLayerRendered(scene, layer));

        sceneManager->setSceneActiveStatus(scene, false);
        EXPECT_FALSE(sceneManager->isSceneActive(scene));

        sceneManager->setLayerActiveStatus(scene, layer, false);
        EXPECT_FALSE(sceneManager->isLayerActive(scene, layer));
    }

    TEST_F(SceneManagerTest, EntityDestroyedPropagation) {
        SceneId scene = sceneManager->createScene("Entity Scene");

        ecs::Entity entity = 1;
        LayerId layer = sceneManager->addLayer(scene, "Layer1");

        sceneManager->addGlobalEntity(entity, scene);
        sceneManager->addEntityToLayer(entity, scene, layer);

        sceneManager->entityDestroyed(entity);
        EXPECT_FALSE(sceneManager->getSceneGlobalEntities(scene).contains(entity));
        EXPECT_FALSE(sceneManager->getLayerEntities(scene, layer).contains(entity));
    }
}
