//// Scene.test.cpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/11/2024
//  Description: Test file for the scenes
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/event/Event.hpp"
#include "core/event/Listener.hpp"
#include "core/scene/Scene.hpp"
#include "core/layer/Layer.hpp"
#include "core/camera/Camera.hpp"
#include "components/Light.hpp"

namespace nexo::event {
    class TestEvent : public Event<TestEvent> {
        public:
        TestEvent() = default;
        explicit TestEvent(int data) : data(data) {}
        int data;
    };
}

namespace nexo::layer {
    class MockLayer : LAYER_LISTENS_TO(MockLayer, event::TestEvent) {
        public:
        explicit MockLayer(const LayerId id, const std::string &name)
            : Layer(id, name) {}

        MOCK_METHOD(void, onUpdate, (Timestep timestep), (override));
        MOCK_METHOD(void, onRender, (std::shared_ptr<renderer::RendererContext> &, const scene::SceneContext &), (override));
        MOCK_METHOD(void, entityDestroyed, (ecs::Entity), (override));
        MOCK_METHOD(void, handleEvent, (event::TestEvent &), (override));
    };
}

namespace nexo::camera {
    class MockCamera : public Camera {
        public:
        MOCK_METHOD(void, onUpdate, (Timestep), (override));
        MOCK_METHOD(const glm::mat4&, getViewProjectionMatrix, (), (const, override));
        MOCK_METHOD(const glm::vec3&, getPosition, (), (const, override));
        MOCK_METHOD(camera::CameraMode, getMode, (), (const, override));
    };
}

namespace nexo::components {
    class MockLight : public Light {
        public:
        MockLight() : Light(LightType::DIRECTIONAL, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f) {}
    };
}


namespace nexo::scene {
    class SceneTest : public ::testing::Test {
    protected:
        void SetUp() override {
            auto &app = nexo::Application::getInstance();
            nexo::init();
            scene = std::make_shared<Scene>(1, "TestScene");
        }

        std::shared_ptr<Scene> scene;
    };

    TEST_F(SceneTest, AddAndRemoveLayers) {
        scene->addLayer(1, "Layer1");
        scene->addLayer(2, "Layer2");

        EXPECT_EQ(scene->getLayer(1)->name, "Layer1");
        EXPECT_EQ(scene->getLayer(2)->name, "Layer2");

        scene->removeLayer(1);
        EXPECT_EQ(scene->getLayer(1), nullptr);
        EXPECT_EQ(scene->getLayer(2)->name, "Layer2");
    }

    TEST_F(SceneTest, AddAndRemoveOverlays) {
        scene->addOverlay(1, "Overlay1");
        scene->addOverlay(2, "Overlay2");

        EXPECT_EQ(scene->getLayer(1)->name, "Overlay1");
        EXPECT_EQ(scene->getLayer(2)->name, "Overlay2");

        scene->removeOverlay(2);
        EXPECT_EQ(scene->getLayer(2), nullptr);
        EXPECT_EQ(scene->getLayer(1)->name, "Overlay1");
    }

    TEST_F(SceneTest, AddAndRemoveEntities) {
        auto &app = getApp();
        ecs::Entity entity1 = 1;
        ecs::Entity entity2 = 2;

        scene->addGlobalEntity(entity1);
        EXPECT_TRUE(scene->getGlobalEntities().contains(entity1));

        scene->removeGlobalEntity(entity1);
        EXPECT_FALSE(scene->getGlobalEntities().contains(entity1));

        app.m_coordinator->addComponent<components::TransformComponent>(entity2, components::TransformComponent{});
        app.m_coordinator->addComponent<components::RenderComponent>(entity2, components::RenderComponent{});

        scene->addLayer(1, "Layer1");
        scene->addEntityToLayer(entity2, 1);
        EXPECT_TRUE(scene->getLayer(1)->getEntities().contains(entity2));

        scene->removeEntityFromLayer(entity2, 1);
        EXPECT_FALSE(scene->getLayer(1)->getEntities().contains(entity2));
    }

    TEST_F(SceneTest, EntityDestroyedPropagation) {
        ecs::Entity entity = 1;

        scene->addLayer(1, "Mock layer");

        scene->addGlobalEntity(entity);
        EXPECT_TRUE(scene->getGlobalEntities().contains(entity));

        scene->entityDestroyed(entity);

        EXPECT_FALSE(scene->getGlobalEntities().contains(entity));
    }

    TEST_F(SceneTest, LayerRenderAndActiveStatus) {
        scene->addLayer(1, "Layer1");

        scene->setLayerRenderStatus(false, 1);
        EXPECT_FALSE(scene->getLayerRenderStatus(1));

        scene->setLayerActiveStatus(false, 1);
        EXPECT_FALSE(scene->getLayerActiveStatus(1));
    }

    TEST_F(SceneTest, AttachAndDetachCameraToLayer) {
        auto mockCamera = std::make_shared<camera::MockCamera>();
        scene->addLayer(1, "Layer1");

        scene->attachCameraToLayer(mockCamera, 1);
        EXPECT_EQ(scene->getCameraLayer(1), mockCamera);

        scene->detachCameraFromLayer(1);
        EXPECT_EQ(scene->getCameraLayer(1), nullptr);
    }

    TEST_F(SceneTest, AddAndRemoveLights) {
        auto light1 = std::make_shared<components::MockLight>();
        auto light2 = std::make_shared<components::MockLight>();

        unsigned int index1 = scene->addLight(light1);
        unsigned int index2 = scene->addLight(light2);

        EXPECT_EQ(index1, 0);
        EXPECT_EQ(index2, 1);

        scene->removeLight(index1);
        EXPECT_EQ(scene->getAmbientLight(), 0.5f); // Ensure the state is consistent
    }
}
